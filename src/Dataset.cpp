/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.55
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/april.h>
#include <april/RenderSystem.h>
#include <april/Texture.h>
#include <april/Window.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hdir.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hresource.h>
#include <hlxml/Document.h>
#include <hlxml/Exception.h>
#include <hlxml/Node.h>
#include <hlxml/Property.h>

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "Images.h"
#include "Objects.h"
#include "Texture.h"

namespace aprilui
{
	void _registerDataset(chstr name, Dataset* dataset);
	void _unregisterDataset(chstr name, Dataset* dataset);
	
	Dataset::Dataset(chstr filename, chstr name, bool useNameBasePath) : EventReceiver()
	{
		this->mFocusedObject = NULL;
		this->mRoot = NULL;
		this->mFilename = normalize_path(filename);
		this->mFilePath = this->_makeFilePath(this->mFilename, name, useNameBasePath);
		this->mName = name;
		this->mNullImage = NULL;
		if (this->mName == "")
		{
			this->mName = this->mFilename.rsplit(".", 1, false).remove_first().rsplit("/", 1, false).remove_last();
		}
		this->mLoaded = false;
		aprilui::_registerDataset(this->mName, this);
	}
	
	Dataset::~Dataset()
	{
		if (this->mNullImage != NULL) delete this->mNullImage;
		if (this->isLoaded())
		{
			if (this->mFocusedObject != NULL)
			{
				april::window->terminateKeyboardHandling();
			}
			this->unload();
		}
		aprilui::_unregisterDataset(this->mName, this);
	}

	int Dataset::getFocusedObjectIndex()
	{
		return (this->mFocusedObject != NULL && this->mFocusedObject->isEnabled() &&
			this->mFocusedObject->isVisible() ? this->mFocusedObject->getFocusIndex() : -1);
	}

	bool Dataset::trySetFocusedObjectByIndex(int value, bool strict)
	{
		if (value < 0)
		{
			this->removeFocus();
			return true;
		}
		foreach_m (Object*, it, this->mObjects)
		{
			if (it->second->getFocusIndex() == value && (!strict && it->second->isEnabled() && it->second->isVisible() ||
				strict && it->second->isDerivedEnabled() && it->second->isDerivedVisible()))
			{
				this->focus(it->second);
				return true;
			}
		}
#ifdef _DEBUG
		hlog::warnf(aprilui::logTag, "Object with Focus Index '%d' does not exist, is not visible or is not enabled!", value);
#endif
		return false;
	}

	harray<int> Dataset::findPossibleFocusIndices(bool strict)
	{
		harray<int> result;
		int focusIndex;
		foreach_m (Object*, it, this->mObjects)
		{
			focusIndex = it->second->getFocusIndex();
			if (focusIndex >= 0 && (!strict && it->second->isEnabled() && it->second->isVisible() ||
				 strict && it->second->isDerivedEnabled() && it->second->isDerivedVisible()))
			{
				result += focusIndex;
			}
		}
		return result;
	}

	harray<int> Dataset::findAllFocusIndices()
	{
		harray<int> result;
		int focusIndex;
		foreach_m (Object*, it, this->mObjects)
		{
			focusIndex = it->second->getFocusIndex();
			if (focusIndex >= 0)
			{
				result += focusIndex;
			}
		}
		return result;
	}

	hstr Dataset::_makeFilePath(chstr filename, chstr name, bool useNameBasePath)
	{
		if (name != "" && useNameBasePath)
		{
			hstr extension = "." + filename.rsplit(".", -1, false).remove_last();
			if (filename.ends_with(name + extension))
			{
				return normalize_path(filename.replace(name + extension, ""));
			}
		}
		return normalize_path(get_basedir(filename));
	}
	
	void Dataset::destroyObject(chstr name)
	{
		this->destroyObject(this->getObject(name));
	}
	
	void Dataset::destroyObject(Object* object)
	{
		if (!this->mObjects.has_key(object->getName()))
		{
			throw ResourceNotExistsException(object->getName(), "Object", this);
		}
		harray<Object*> children = object->getChildren();
		foreach (Object*, it, children)
		{
			this->destroyObject(*it);
		}
		if (object->getParent() != NULL)
		{
			object->detach();
		}
		if (object->isFocused())
		{
			object->setFocused(false);
		}
		this->mObjects.remove_key(object->getName());
		delete object;
	}
	
	void Dataset::_destroyTexture(Texture* texture)
	{
		hstr filename = texture->getFilename();
		if (!this->mTextures.has_key(filename))
		{
			throw ResourceNotExistsException(filename, "Texture", this);
		}
		this->mTextures.remove_key(filename);
		delete texture;
	}
	
	void Dataset::_destroyImage(Image* image)
	{
		hstr name = image->getName();
		if (!this->mImages.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		this->mImages.remove_key(name);
		delete image;
	}
	
	void Dataset::_destroyTexture(chstr name)
	{
		if (!this->mTextures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		delete this->mTextures[name];
		this->mTextures.remove_key(name);
	}
	
	void Dataset::_destroyImage(chstr name)
	{
		if (!this->mImages.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		delete this->mImages[name];
		this->mImages.remove_key(name);
	}

	hstr Dataset::_makeLocalizedTextureName(chstr filename)
	{
		hstr localization = aprilui::getLocalization();
		if (localization != "")
		{
			hstr locpath = get_basedir(filename) + "/" + localization + "/" + get_basename(filename);
			locpath = april::rendersys->findTextureFilename(locpath);
			if (locpath != "")
			{
				return locpath;
			}
		}
		return filename;
	}
	
	void Dataset::parseTexture(hlxml::Node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		hstr filepath = normalize_path(this->mFilePath + "/" + filename);
		hstr textureName = get_basename(filename);
		if (this->mTextures.has_key(textureName))
		{
			throw ObjectExistsException(textureName, filename);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		bool dynamicLoad = node->pbool("dynamic_load", getDefaultDynamicLoading());

		hstr locpath = this->_makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->createTexture(locpath, !dynamicLoad);
		if (aprilTexture == NULL)
		{
			throw file_not_found(locpath);
		}
		Texture* texture = new Texture(filepath, aprilTexture);
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if		(filter == "linear")	texture->setFilter(april::Texture::FILTER_LINEAR);
			else if	(filter == "nearest")	texture->setFilter(april::Texture::FILTER_NEAREST);
			else throw hl_exception("texture filter '" + filter + "' not supported");
		}
		if (node->pbool("wrap", true))
		{
			texture->setAddressMode(april::Texture::ADDRESS_WRAP);
		}
		else
		{
			texture->setAddressMode(april::Texture::ADDRESS_CLAMP);
		}
		this->mTextures[textureName] = texture;
		// extract image definitions
		if (node->iterChildren() == NULL) // if there are no images defined, create one that fills the whole area
		{
			if (this->mImages.has_key(textureName))
			{
				throw ResourceExistsException(filename, "Texture", this);
			}
			Image* img = new Image(texture, filename, grect(0, 0, (float)texture->getWidth(), (float)texture->getHeight()));
			this->mImages[textureName] = img;
			img->_setDataset(this);
		}
		else
		{
			Image* image;
			foreach_xmlnode (child, node)
			{
				if (*child == "Image")
				{
					hstr name = (prefixImages ? textureName + "/" + child->pstr("name") : child->pstr("name"));
					if (this->mImages.has_key(name))
					{
						throw ResourceExistsException(name, "Image", this);
					}
					grect rect(child->pfloat("x"), child->pfloat("y"), child->pfloat("w"), child->pfloat("h"));
					
					bool vertical = child->pbool("vertical", false);
					float tile_w = child->pfloat("tile_w", 1.0f);
					float tile_h = child->pfloat("tile_h", 1.0f);
					
					if (tile_w != 1.0f || tile_h != 1.0f)
					{
						image = new TiledImage(texture, name, rect, vertical, tile_w, tile_h);
					}
					else if (child->pexists("color"))
					{
						april::Color color(child->pstr("color"));
						image = new ColoredImage(texture, name, rect, vertical, color);
					}
					else
					{
						bool invertX = child->pbool("invertx", false);
						bool invertY = child->pbool("inverty", false);
						image = new Image(texture, name, rect, vertical, invertX, invertY);	
					}
					hstr mode = child->pstr("blend_mode", "default");
					if (mode == "add")
					{
						image->setBlendMode(april::ADD);
					}
					else if (mode == "subtract")
					{
						image->setBlendMode(april::SUBTRACT);
					}
					this->mImages[name] = image;
					image->_setDataset(this);
				}
			}
		}
	}
	
	void Dataset::parseRamTexture(hlxml::Node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		hstr filepath = normalize_path(mFilePath + "/" + filename);
		int slash = filename.find('/') + 1;
		hstr textureName = filename(slash, filename.rfind('.') - slash);
		if (this->mTextures.has_key(textureName))
		{
			throw ResourceExistsException(filename, "RamTexture", this);
		}
		bool dynamicLoad = node->pbool("dynamic_load", getDefaultDynamicLoading());
		hstr locpath = this->_makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->createRamTexture(locpath, !dynamicLoad);
		if (!aprilTexture)
		{
			throw file_not_found(locpath);
		}
		this->mTextures[textureName] = new Texture(filepath, aprilTexture);
	}
	
	void Dataset::parseCompositeImage(hlxml::Node* node)
	{
		hstr name = node->pstr("name");
		hstr refname;
		if (this->mImages.has_key(name))
		{
			throw ResourceExistsException(name, "CompositeImage", this);
		}
		CompositeImage* image = new CompositeImage(name, node->pfloat("w"), node->pfloat("h"));
		foreach_xmlnode (child, node)
		{
			if (*child == "ImageRef")
			{
				refname = child->pstr("name");
				image->addImageRef(this->getImage(refname), grect(child->pfloat("x"), child->pfloat("y"), child->pfloat("w"), child->pfloat("h")));
			}
		}
		this->mImages[name] = image;
		image->_setDataset(this);
	}
	
	Object* Dataset::parseObject(hlxml::Node* node, Object* parent)
	{
		return this->recursiveObjectParse(node, parent, "", "", gvec2());
	}
	
	void Dataset::parseTextureGroup(hlxml::Node* node)
	{
		harray<hstr> names = node->pstr("names").split(",", -1, true);
		foreach (hstr, it, names)
		{
			foreach (hstr, it2, names)
			{
				if ((*it) != (*it2))
				{
					this->getTexture(*it)->addDynamicLink(this->getTexture(*it2));
				}
			}
		}
	}
	
	Object* Dataset::recursiveObjectParse(hlxml::Node* node, Object* parent)
	{
		return this->recursiveObjectParse(node, parent, "", "", gvec2());
	}

	Object* Dataset::recursiveObjectParse(hlxml::Node* node, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		hstr className;
		hstr objectName;
		grect rect(0.0f, 0.0f, 1.0f, 1.0f);
		
		if (*node == "Include")
		{
			this->parseObjectInclude(this->mFilePath + "/" + node->pstr("path"), parent,
				node->pstr("name_prefix", "") + namePrefix, nameSuffix + node->pstr("name_suffix", ""),
				gvec2(node->pfloat("x", 0.0f), node->pfloat("y", 0.0f)));
			return NULL;
		}

		className = node->pstr("type");
		
		if (*node == "Object" || *node == "Animator")
		{
			if (node->pexists("name"))
			{
				objectName = namePrefix + node->pstr("name") + nameSuffix;
			}
			else
			{
				objectName = aprilui::generateName(className);
			}
			node->setProperty("name", objectName);
			if (*node == "Object")
			{
				rect.x = node->pfloat("x") + offset.x;
				rect.y = node->pfloat("y") + offset.y;
				rect.w = node->pfloat("w", -1.0f);
				rect.h = node->pfloat("h", -1.0f);
			}
		}
		else
		{
			return NULL;
		}
		if (this->mObjects.has_key(objectName))
		{
			throw ResourceExistsException(objectName, "Object", this);
		}

		Object* object = NULL;
		if (*node == "Object")
		{
			object = aprilui::createObject(className, objectName, rect);
		}
		else if (*node == "Animator")
		{
			object = aprilui::createAnimator(className, objectName);
		}
		if (object == NULL)
		{
			object = this->parseExternalObjectClass(node, objectName, rect);
		}

		if (object == NULL)
		{
			throw hlxml::XMLUnknownClassException(className, node);
		}
		object->_setDataset(this);
		object->notifyEvent("RegisterInDataset", this);
		this->mObjects[objectName] = object;
		if (this->mRoot == NULL)
		{
			this->mRoot = object;
		}
		if (parent != NULL)
		{
			parent->addChild(object);
		}
		hstr name;

		foreach_xmlproperty (prop, node)
		{
			name = prop->name();
			if (name == "x" || name == "y" || name == "w" || name == "h")
			{
				continue; // TODO - should be done better, maybe reading parameters from a list, then removing them so they aren't set more than once
			}
			object->setProperty(name, prop->value());
		}
		
		hlxml::Node::Type type;
		foreach_xmlnode (child, node)
		{
			type = child->getType();
			if (type != hlxml::Node::TYPE_TEXT && type != hlxml::Node::TYPE_COMMENT)
			{
				this->recursiveObjectParse(child, object, namePrefix, nameSuffix, gvec2());
			}
		}
		return object;
	}
	
	void Dataset::parseGlobalInclude(chstr path)
	{
		hstr originalFilePath = this->mFilePath;
		this->mFilePath = this->_makeFilePath(path);
		if (!path.contains("*"))
		{
			this->readFile(path);
			this->mFilePath = originalFilePath;
			return;
		}
		hstr extension = get_basename(path).replace("*", "");
		harray<hstr> contents = hdir::resource_files(this->mFilePath, true).sorted();
		foreach (hstr, it, contents)
		{
			if ((*it).ends_with(extension))
			{
				this->readFile((*it));
			}
		}
		this->mFilePath = originalFilePath;
	}
	
	void Dataset::parseObjectIncludeFile(chstr filename, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		// parse dataset xml file, error checking first
		hstr path = normalize_path(filename);
		hlog::write(aprilui::logTag, "Parsing object include file: " + path);
		hlxml::Document* doc = hlxml::open(path);
		hlxml::Node* current = doc->root();
		foreach_xmlnode (node, current)
		{
			if (*node == "Object" || *node == "Animator")
			{
				this->recursiveObjectParse(node, parent, namePrefix, nameSuffix, offset);
			}
		}
		hlxml::close(doc);
	}
	
	void Dataset::parseObjectInclude(chstr path, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		if (!path.contains("*"))
		{
			this->parseObjectIncludeFile(path, parent, namePrefix, nameSuffix, offset);
			return;
		}
		hstr basedir = get_basedir(path);
		hstr filename = path(basedir.size() + 1, -1);
		hstr left;
		hstr right;
		filename.split("*", left, right);
		harray<hstr> contents = hdir::resource_files(basedir).sorted();
		foreach (hstr, it, contents)
		{
			if ((*it).starts_with(left) && (*it).ends_with(right))
			{
				this->parseObjectIncludeFile(basedir + "/" + (*it), parent, "", "", gvec2());
			}
		}
	}
	
	void Dataset::readFile(chstr filename)
	{
		// parse dataset xml file, error checking first
		hstr path = normalize_path(filename);
		hlog::write(aprilui::logTag, "Parsing dataset file: " + path);
		hlxml::Document* doc = hlxml::open(path);
		hlxml::Node* current = doc->root();

		this->parseExternalXMLNode(current);

		foreach_xmlnode (node, current)
		{
			if		(*node == "Texture")		parseTexture(node);
			else if	(*node == "RamTexture")		parseRamTexture(node);
			else if	(*node == "CompositeImage")	parseCompositeImage(node);
			else if	(*node == "Object")			parseObject(node);
			else if	(*node == "Include")		parseGlobalInclude(get_basedir(path) + "/" + node->pstr("path"));
			else if	(*node == "TextureGroup")	parseTextureGroup(node);
			else
			{
				this->parseExternalXMLNode(node);
			}
		}
		hlxml::close(doc);
	}

	void Dataset::load()
	{
		this->_loadTexts(this->_makeTextsPath());
		this->readFile(this->mFilename);
		this->mLoaded = true;
		this->update(0.0f);
	}

	hstr Dataset::_makeTextsPath()
	{
		hstr filepathPrefix = this->mFilePath + "/" + (this->mTextsPath != "" ? this->mTextsPath : aprilui::getDefaultTextsPath()) + "/";
		hstr filepath = normalize_path(filepathPrefix + aprilui::getLocalization());
		if (!hdir::resource_exists(filepath))
		{
			filepath = normalize_path(filepathPrefix + aprilui::getDefaultLocalization());
		}
		return filepath;
	}
	
	void Dataset::_loadTexts(chstr path)
	{
		hlog::write(aprilui::logTag, "Loading texts: " + path);
		harray<hstr> files = hdir::resource_files(path, true);
		harray<hstr> lines;
		harray<hstr> values;
		bool keyMode = true;
		hstr key;
		hresource f;
		foreach (hstr, it, files)
		{
			f.open(*it);
			lines = f.read_lines();
			f.close();
			if (lines.size() == 0)
			{
				continue;
			}
			// ignore file header, silly utf-8 encoded text files have 2-3 char markers
			hstr firstLine = lines.first();
			if (firstLine.size() > 0)
			{
				int i = 0;
				while (i < firstLine.size() && !is_between((int)firstLine[i], 0, 127))
				{
					i++;
				}
				lines[0] = (i < firstLine.size() ? firstLine(i, firstLine.size() - i) : "");
			}
			// now parse the entries
			foreach (hstr, it2, lines)
			{
				if (keyMode)
				{
					if ((*it2) == "{")
					{
						values.clear();
						keyMode = false;
					}
					else
					{
						key = (*it2).split("#").first().trim(' ');
					}
				}
				else if ((*it2) == "}")
				{
					keyMode = true;
					if (key != "")
					{
						mTexts[key] = values.join('\n');
					}
				}
				else
				{
					values += (*it2);
				}
			}
		}
	}
	
	void Dataset::unload()
	{
		if (!this->mLoaded)
		{
			throw GenericException("Unable to unload dataset '" + this->getName() + "', data not loaded!");
		}
		aprilui::Object* obj;
		foreach_m (Object*, it, this->mObjects)
		{
			obj = it->second;
			if (obj->getParent()) obj->detach();
			obj->removeChildren(false);
			delete obj;
		}
		this->mObjects.clear();
		
		foreach_m (Image*, it, this->mImages)
		{
			delete it->second;
		}
		this->mImages.clear();
		
		foreach_m (Texture*, it, this->mTextures)
		{
			delete it->second;
		}
		this->mTextures.clear();
		
		this->mCallbacks.clear();
		this->mTexts.clear();
		this->mRoot = NULL;
		this->mFocusedObject = NULL;
		this->mLoaded = false;
	}
	
	void Dataset::registerManualObject(Object* object)
	{
		hstr name = object->getName();
		if (this->mObjects.has_key(name))
		{
			throw ResourceExistsException(name, "Object", this);
		}
		this->mObjects[name] = object;
		object->_setDataset(this);
		object->notifyEvent("RegisterInDataset", this);
	}
	
	void Dataset::unregisterManualObject(Object* object)
	{
		hstr name = object->getName();
		if (!this->mObjects.has_key(name))
		{
			throw ResourceNotExistsException(name, "Object", this);
		}
		this->mObjects.remove_key(name);
		object->_setDataset(NULL);
		object->notifyEvent("UnregisterFromDataset", this);
	}
	
	void Dataset::registerManualImage(Image* image)
	{
		hstr name = image->getName();
		if (this->mImages.has_key(name))
		{
			throw ResourceExistsException(name, "Image", this);
		}
		this->mImages[name] = image;
		image->_setDataset(this);
	}
	
	void Dataset::unregisterManualImage(Image* image)
	{
		hstr name = image->getName();
		if (!this->mImages.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		this->mImages.remove_key(name);
		image->_setDataset(NULL);
	}
	
	void Dataset::registerManualTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (this->mTextures.has_key(name))
		{
			throw ResourceExistsException(name, "Texture", this);
		}
		this->mTextures[name] = texture;
	}

	void Dataset::unregisterManualTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (!this->mTextures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		this->mTextures.remove_key(name);
	}
	
	bool Dataset::isAnimated()
	{
		aprilui::Animator* object;
		foreach_m (Object*, it, this->mObjects)
		{
			object = dynamic_cast<aprilui::Animator*>(it->second);
			if (object != NULL && object->isAnimated())
			{
				return true;
			}
		}
		return false;
	}
	
	bool Dataset::isWaitingAnimation()
	{
		foreach_m (Object*, it, this->mObjects)
		{
			if (it->second->isWaitingAnimation())
			{
				return true;
			}
		}
		return false;
	}
	
	Object* Dataset::getObject(chstr name)
	{
		if (!this->mObjects.has_key(name))
		{
			throw ResourceNotExistsException(name, "Object", this);
		}
		return this->mObjects[name];
	}
	
	bool Dataset::hasObject(chstr name)
	{
		return this->tryGetObject(name) != NULL;
	}
	
	bool Dataset::hasImage(chstr name)
	{
		return this->mImages.has_key(name);
	}
	
	bool Dataset::hasTexture(chstr name)
	{
		return this->mTextures.has_key(name);
	}
	
	Object* Dataset::tryGetObject(chstr name)
	{
		return this->mObjects.try_get_by_key(name, NULL);
	}
	
	Texture* Dataset::getTexture(chstr name)
	{
		if (!this->mTextures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		return this->mTextures[name];
	}
	
	Image* Dataset::getImage(chstr name)
	{
		Image* image = NULL;
		if (name == "null")
		{
			if (!this->mNullImage)
			{
				this->mNullImage = new NullImage();
				this->mNullImage->_setDataset(this);
			}
			return this->mNullImage;
		}
		
		bool hasKey = this->mImages.has_key(name);
		if (!hasKey && name.starts_with("0x")) // create new image with a color. don't overuse this, it's meant to be handy when needed only ;)
		{
			image = new ColorImage(name);
			this->mImages[name] = image;
			image->_setDataset(this);
		}
		else if (hasKey)
		{
			image = this->mImages[name];
		}
		if (image == NULL)
		{
			int dot = name.find('.');
			if (dot < 0)
			{
				throw ResourceNotExistsException(name, "Image", this);
			}
			Dataset* dataset;
			try
			{
				dataset = aprilui::getDatasetByName(name(0, dot));
			}
			catch (_GenericException)
			{
				throw ResourceNotExistsException(name, "Image", this);
			}
			image = dataset->getImage(name(dot + 1, 100));
		}
		return image;
	}

	bool Dataset::_findTextEntry(chstr textKey, hstr* text)
	{
		int dot = textKey.find('.');
		if (dot < 0)
		{
			if (!this->mTexts.has_key(textKey))
			{
				if (text != NULL)
				{
					*text = "ERROR: Text '" + textKey + "' not found!";
				}
				return false;
			}
			if (text != NULL)
			{
				*text = this->mTexts[textKey];
			}
			return true;
		}
		Dataset* dataset = aprilui::getDatasetByName(textKey(0, dot));
		return dataset->_findTextEntry(textKey(dot + 1, 100), text);
	}
	
	hstr Dataset::getTextEntry(chstr textKey)
	{
		hstr text;
		this->_findTextEntry(textKey, &text);
		return text;
	}
	
	bool Dataset::hasTextEntry(chstr textKey)
	{
		return this->_findTextEntry(textKey, NULL);
	}
	
	hstr Dataset::getText(chstr compositeTextKey)
	{
		return this->_parseCompositeTextKey(compositeTextKey);
	}
	
	harray<hstr> Dataset::getTextEntries(harray<hstr> keys)
	{
		harray<hstr> result;
		foreach (hstr, it, keys)
		{
			result += this->getTextEntry(*it);
		}
		return result;
	}

	void Dataset::registerCallback(chstr name, void (*callback)())
	{
		this->mCallbacks[name] = callback;
	}
	
	void Dataset::triggerCallback(chstr name)
	{
		if (this->mCallbacks.has_key(name))
		{
			(*this->mCallbacks[name])();
		}
	}
	
	void Dataset::draw()
	{
		if (this->mRoot != NULL)
		{
			this->mRoot->draw();
		}
	}
	
	bool Dataset::onMouseDown(april::Key keyCode)
	{
		return (this->mRoot != NULL && this->mRoot->onMouseDown(keyCode));
	}
	
	bool Dataset::onMouseUp(april::Key keyCode)
	{
		return (this->mRoot != NULL && this->mRoot->onMouseUp(keyCode));
	}
	
	bool Dataset::onMouseMove()
	{
		return (this->mRoot != NULL && this->mRoot->onMouseMove());
	}
	
	bool Dataset::onMouseScroll(float x, float y)
	{
		return (this->mRoot != NULL && this->mRoot->onMouseScroll(x, y));
	}
	
	bool Dataset::onKeyDown(april::Key keyCode)
	{
		return (this->mRoot != NULL && this->mRoot->onKeyDown(keyCode));
	}
	
	bool Dataset::onKeyUp(april::Key keyCode)
	{
		return (this->mRoot != NULL && this->mRoot->onKeyUp(keyCode));
	}
	
	bool Dataset::onChar(unsigned int charCode)
	{
		return (this->mRoot != NULL && this->mRoot->onChar(charCode));
	}
	
	void Dataset::updateTextures(float k)
	{
		foreach_m (Texture*, it, this->mTextures)
		{
			it->second->update(k);
		}
	}
	
	void Dataset::processEvents()
	{
		QueuedCallback callback;
		while (mCallbackQueue.size())
		{
			callback = mCallbackQueue.front();
			mCallbackQueue.pop_front();
			callback.event->execute(callback.args);
			delete callback.args;
		}
	}
	
	void Dataset::queueCallback(Event* event, EventArgs* args)
	{
		QueuedCallback callback;
		callback.event = event;
		callback.args = args;
		this->mCallbackQueue.push_back(callback);
	}
	
	void Dataset::removeCallbackFromQueue(Event* event)
	{
		int index = 0;
		foreach (QueuedCallback, it, this->mCallbackQueue)
		{
			if (it->event == event)
			{
				break;
			}
			index += 1;
		}
		if (index != this->mCallbackQueue.size())
		{
			QueuedCallback callback = this->mCallbackQueue[index];
			delete callback.args;
			this->mCallbackQueue.remove_at(index);
		}
	}
	
	void Dataset::update(float k)
	{
		this->updateTextures(k);
		if (this->mRoot != NULL)
		{
			this->mRoot->update(k);
		}
		foreach_m (aprilui::Object*, it, this->mObjects)
		{
			it->second->clearChildUnderCursor();
		}
	}

	void Dataset::notifyEvent(chstr name, void* params)
	{
		foreach_m (aprilui::Object*, it, this->mObjects)
		{
			it->second->notifyEvent(name, params);
		}
	}
	
	void Dataset::unloadUnusedTextures()
	{
		foreach_m (Texture*, it, this->mTextures)
		{
			if (it->second->isDynamic() && it->second->getUnusedTime() > 1.0f)
			{
				it->second->unload();
			}
		}
	}
	
	void Dataset::reloadTexts()
	{
		this->mTexts.clear();
		this->_loadTexts(this->_makeTextsPath());
	}
	
	void Dataset::reloadTextures()
	{
		foreach_m (aprilui::Texture*, it, this->mTextures)
		{
			it->second->reload(this->_makeLocalizedTextureName(it->second->getOriginalFilename()));
		}
	}

	void Dataset::focus(Object* object)
	{
		this->removeFocus();
		this->mFocusedObject = object;
		this->mFocusedObject->triggerEvent("GainFocus");
	}

	void Dataset::removeFocus()
	{
		if (this->mFocusedObject != NULL)
		{
			april::window->terminateKeyboardHandling();
			// in case of a recursive call, the focused object has to be removed first.
			aprilui::Object* object = this->mFocusedObject;
			this->mFocusedObject = NULL;
			object->triggerEvent("LoseFocus");
		}
	}
	
	hstr Dataset::_parseCompositeTextKey(chstr key)
	{
		ustr chars = key.u_str();
		if (chars.size() == 0 || chars[0] != '{')
		{
			if ((int)chars.find_first_of('{') >= 0 || (int)chars.find_first_of('}') >= 0)
			{
				hstr text = hsprintf("Malformed formatted text key '%s'!", key.c_str());
				hlog::error(aprilui::logTag, text);
				return text;
			}
			if (!this->hasTextEntry(key))
			{
				hlog::warnf(aprilui::logTag, "Text key '%s' does not exist!", key.c_str());
			}
			return this->getTextEntry(key);
		}
		int index = chars.find_first_of('}');
		if (index < 0)
		{
			hlog::errorf(aprilui::logTag, "Could not parse formatted key '%s'.", key.c_str());
			return key;
		}
		harray<ustr> args;
		ustr format = chars.substr(1, index - 1);
		ustr argString = chars.substr(index + 1, chars.size() - index - 1);
		// trimming
		if (argString.size() > 0)
		{
			const unsigned int* cstr = argString.c_str();
			while (cstr[0] == ' ')
			{
				cstr++;
			}
			argString = cstr;
			// r-trimming
			if (argString.size() > 0)
			{
				cstr = argString.c_str();
				int i = argString.size() - 1;
				while (i >= 0 && cstr[i] == ' ')
				{
					i--;
				}
				argString = argString.substr(0, i + 1);
			}
		}
		// trimming finished
		if (!this->_processCompositeTextKeyArgs(argString, args))
		{
			hlog::writef(aprilui::logTag, "- while processing args: '%s' with args '%s'.", format.c_str(), argString.c_str());
			return key;
		}
		ustr preprocessedFormat;
		harray<ustr> preprocessedArgs;
		if (!this->_preprocessCompositeTextKeyFormat(format, args, preprocessedFormat, preprocessedArgs))
		{
			hlog::writef(aprilui::logTag, "- while preprocessing format: '%s' with args '%s'.", format.c_str(), argString.c_str());
			return key;
		}
		hstr result;
		if (!this->_processCompositeTextKeyFormat(preprocessedFormat, preprocessedArgs, result))
		{
			hlog::writef(aprilui::logTag, "- while processing format: '%s' with args '%s'.", format.c_str(), argString.c_str());
			return key;
		}
		return result;
	}


	bool Dataset::_processCompositeTextKeyArgs(ustr argString, harray<ustr>& args)
	{
		args.clear();
		// splittings args
		int openIndex;
		int closeIndex;
		while (argString.size() > 0)
		{
			openIndex = argString.find_first_of('{');
			closeIndex = argString.find_first_of('}');
			if (openIndex < 0 && closeIndex < 0)
			{
				args += this->_getArgEntries(argString);
				break;
			}
			if (openIndex < 0 || closeIndex < 0)
			{
				hlog::error(aprilui::logTag, "'{' without '}' or '}' without '{'.");
				return false;
			}
			if (closeIndex < openIndex)
			{
				hlog::error(aprilui::logTag, "'}' before '{'.");
				return false;
			}
			// getting all args before the {
			args += this->_getArgEntries(argString.substr(0, openIndex));
			// getting args inside of {}
			args += argString.substr(openIndex + 1, closeIndex - openIndex - 1);
			// rest of the args
			argString = argString.substr(closeIndex + 1, argString.size() - closeIndex - 1);
		}
		return true;
	}

	bool Dataset::_preprocessCompositeTextKeyFormat(ustr format, harray<ustr> args, ustr& preprocessedFormat, harray<ustr>& preprocessedArgs)
	{
		preprocessedFormat.clear();
		preprocessedArgs.clear();
		// preprocessing of format string and args
		int index;
		ustr arg;
		harray<int> indexes;
		while (format.size() > 0)
		{
			index = format.find_first_of('%');
			if (index < 0)
			{
				preprocessedFormat += format;
				break;
			}
			if (index >= (int)format.size() - 1)
			{
				hlog::error(aprilui::logTag, "Last character is '%'!");
				return false;
			}
			if (format[index + 1] == '%') // escaped "%", continue processing
			{
				preprocessedFormat += format.substr(0, index + 2);
				format = format.substr(index + 2, format.size() - index - 2);
				continue;
			}
			if (format[index + 1] == 's') // %s, not processing that now
			{
				if (args.size() == 0)
				{
					hlog::error(aprilui::logTag, "Not enough args!");
					return false;
				}
				preprocessedFormat += format.substr(0, index + 2);
				format = format.substr(index + 2, format.size() - index - 2);
				preprocessedArgs += args.remove_first();
				continue;
			}
			if (format[index + 1] == 'f')
			{
				if (args.size() == 0)
				{
					hlog::error(aprilui::logTag, "Not enough args!");
					return false;
				}
				arg = args.remove_first();
				preprocessedFormat += format.substr(0, index) + arg;
				format = format.substr(index + 2, format.size() - index - 2);
				if (!this->_getCompositeTextKeyFormatIndexes(arg, indexes))
				{
					return false;
				}
				if (indexes.size() > args.size())
				{
					hlog::error(aprilui::logTag, "Not enough args!");
					return false;
				}
				preprocessedArgs += args.remove_first(indexes.size());
			}
		}
		preprocessedArgs += args; // remaining args
		return true;
	}

	bool Dataset::_processCompositeTextKeyFormat(ustr format, harray<ustr> args, hstr& result)
	{
		result = "";
		ustr preResult;
		// preprocessing of format string and args
		harray<int> indexes;
		if (!this->_getCompositeTextKeyFormatIndexes(format, indexes))
		{
			return false;
		}
		if (args.size() < indexes.size())
		{
			hlog::error(aprilui::logTag, "Not enough args!");
			return false;
		}
		if (indexes.size() > args.size())
		{
			hlog::error(aprilui::logTag, "Too many args!");
			return false;
		}
		foreach (int, it, indexes)
		{
			preResult += format.substr(0, (*it));
			preResult += args.remove_first();
			format = format.substr((*it) + 2, format.size() - (*it) - 2);
		}
		preResult += format;
		int index = preResult.find_first_of('%');
		while (index >= 0 && index < (int)preResult.size() - 1)
		{
			if (preResult[index + 1] == '%')
			{
				preResult.erase(index + 1, 1);
			}
			index = preResult.find_first_of('%', index + 1);
		}
		result = hstr::from_unicode(preResult.c_str());
		return true;
	}

	bool Dataset::_getCompositeTextKeyFormatIndexes(ustr format, harray<int>& indexes)
	{
		indexes.clear();
		// finding formatting indexes
		int index;
		int currentIndex = 0;
		while (format.size() > 0)
		{
			index = format.find_first_of('%');
			if (index < 0)
			{
				break;
			}
			if (index >= (int)format.size() - 1)
			{
				hlog::error(aprilui::logTag, "Last character is '%'!");
				return false;
			}
			if (format[index + 1] == '%') // escaped "%", use just one "%".
			{
				format = format.substr(index + 2, format.size() - index - 2);
				currentIndex += index + 2;
				continue;
			}
			if (format[index + 1] != 's')
			{
				hlog::errorf(aprilui::logTag, "Unsupported formatting '%%%c'!", format[index + 1]);
				return false;
			}
			indexes += currentIndex + index;
			format = format.substr(index + 2, format.size() - index - 2);
			currentIndex = 0;
		}
		return true;
	}


	harray<Dataset::ustr> Dataset::_getArgEntries(ustr string)
	{
		harray<hstr> keys;
		int index;
		while (true)
		{
			index = string.find_first_of(' ');
			if (index < 0)
			{
				break;
			}
			keys += hstr::from_unicode(string.substr(0, index).c_str());
			string = string.substr(index + 1);
		}
		keys += hstr::from_unicode(string.c_str());
		keys.remove_all("");
		harray<ustr> result;
		foreach (hstr, it, keys)
		{
			result += this->getTextEntry(*it).u_str();
		}
		return result;
	}

}
