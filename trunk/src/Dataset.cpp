/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.82
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
	
	NullImage nullImage;
	
	Dataset::Dataset(chstr filename, chstr name, bool useNameBasePath) : EventReceiver()
	{
		mFocusedObject = NULL;
		mRoot = NULL;
		mFilename = normalize_path(filename);
		mFilePath = _makeFilePath(mFilename, name, useNameBasePath);
		mName = name;
		if (mName == "")
		{
			mName = mFilename.rsplit(".", 1, false).pop_first().rsplit("/", 1, false).pop_last();
		}
		mLoaded = false;
		_registerDataset(mName, this);
	}
	
	Dataset::~Dataset()
	{
		if (isLoaded())
		{
			if (mFocusedObject != NULL)
			{
				april::window->terminateKeyboardHandling();
			}
			unload();
		}
		_unregisterDataset(mName, this);
	}

	hstr Dataset::_makeFilePath(chstr filename, chstr name, bool useNameBasePath)
	{
		if (name != "" && useNameBasePath)
		{
			hstr extension = "." + filename.rsplit(".", -1, false).pop_last();
			if (filename.ends_with(name + extension))
			{
				return normalize_path(filename.replace(name + extension, ""));
			}
		}
		return normalize_path(get_basedir(filename));
	}
	
	void Dataset::destroyObject(chstr name, bool recursive)
	{
		destroyObject(getObject(name), recursive);
	}
	
	void Dataset::destroyObject(Object* object, bool recursive)
	{
		if (!mObjects.has_key(object->getName()))
		{
			throw ResourceNotExistsException(object->getName(), "Object", this);
		}
		if (recursive)
		{
			harray<Object*> children = object->getChildren();
			foreach (Object*, it, children)
			{
				destroyObject((*it), true);
			}
		}
		else
		{
			while (object->getChildren().size() > 0)
			{
				object->removeChild(object->getChildren().first());
			}
		}
		if (object->getParent() != NULL)
		{
			object->detach();
		}
		if (mFocusedObject == object)
		{
			mFocusedObject = NULL;
		}
		mObjects.remove_key(object->getName());
		delete object;
	}
	
	void Dataset::_destroyTexture(Texture* texture)
	{
		if (!mTextures.has_key(texture->getFilename()))
		{
			throw ResourceNotExistsException(texture->getFilename(), "Texture", this);
		}
		mTextures.remove_key(texture->getFilename());
		delete texture;
	}
	
	void Dataset::_destroyImage(Image* image)
	{
		if (!mImages.has_key(image->getName()))
		{
			throw ResourceNotExistsException(image->getName(), "Image", this);
		}
		mImages.remove_key(image->getName());
		delete image;
	}
	
	void Dataset::_destroyTexture(chstr name)
	{
		if (!mTextures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		Texture* texture = mTextures[name];
		mTextures.remove_key(name);
		delete texture;
	}
	
	void Dataset::_destroyImage(chstr name)
	{
		if (!mImages.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		Image* image = mImages[name];
		mImages.remove_key(name);
		delete image;
	}

	hstr Dataset::_makeLocalizedTextureName(chstr filename)
	{
		hstr localization = getLocalization();
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
		hstr filepath = normalize_path(mFilePath + "/" + filename);
		hstr textureName = get_basename(filename);
		if (mTextures.has_key(textureName))
		{
			throw ObjectExistsException(textureName, filename);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		bool dynamicLoad = node->pbool("dynamic_load", false);

		hstr locpath = _makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->loadTexture(locpath, aprilui::getForcedDynamicLoading() || dynamicLoad);
		if (aprilTexture == NULL)
		{
			throw file_not_found(locpath);
		}
		Texture* texture = new Texture(filepath, aprilTexture);
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if	  (filter == "linear")  texture->setFilter(april::Texture::FILTER_LINEAR);
			else if (filter == "nearest") texture->setFilter(april::Texture::FILTER_NEAREST);
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
		mTextures[textureName] = texture;
		// extract image definitions
		if (node->iterChildren() == NULL) // if there are no images defined, create one that fills the whole area
		{
			if (mImages.has_key(textureName))
			{
				throw ResourceExistsException(filename, "Texture", this);
			}
			mImages[textureName] = new Image(texture, filename, grect(0, 0, (float)texture->getWidth(), (float)texture->getHeight()));
		}
		else
		{
			Image* image;
			foreach_xmlnode (child, node)
			{
				if (*child == "Image")
				{
					hstr name = (prefixImages ? textureName + "/" + child->pstr("name") : child->pstr("name"));
					if (mImages.has_key(name))
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
					mImages[name] = image;
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
		if (mTextures.has_key(textureName))
		{
			throw ResourceExistsException(filename, "RamTexture", this);
		}
		bool dynamicLoad = node->pbool("dynamic_load", false);
		hstr locpath = _makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->loadRamTexture(locpath, aprilui::getForcedDynamicLoading() || dynamicLoad);
		if (!aprilTexture)
		{
			throw file_not_found(locpath);
		}
		mTextures[textureName] = new Texture(filepath, aprilTexture);
	}
	
	void Dataset::parseCompositeImage(hlxml::Node* node)
	{
		hstr name = node->pstr("name");
		hstr refname;
		if (mImages.has_key(name))
		{
			throw ResourceExistsException(name, "CompositeImage", this);
		}
		CompositeImage* image = new CompositeImage(name, node->pfloat("w"), node->pfloat("h"));
		foreach_xmlnode (child, node)
		{
			if (*child == "ImageRef")
			{
				refname = child->pstr("name");
				image->addImageRef(getImage(refname),
					grect(child->pfloat("x"), child->pfloat("y"), child->pfloat("w"), child->pfloat("h")));
			}
		}
		mImages[name] = image;
	}
	
	Object* Dataset::parseObject(hlxml::Node* node, Object* parent)
	{
		return recursiveObjectParse(node, parent);
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
					getTexture(*it)->addDynamicLink(getTexture(*it2));
				}
			}
		}
	}
	
	Object* Dataset::recursiveObjectParse(hlxml::Node* node, Object* parent)
	{
		hstr objectName;
		grect rect(0, 0, 1, 1);
		hstr className;
		
		if (*node == "Include")
		{
			parseObjectInclude(mFilePath + "/" + node->pstr("path"), parent);
			return NULL;
		}

		className = node->pstr("type");
		
		if (*node == "Object")
		{
			if (node->pexists("name"))
			{
				objectName = node->pstr("name");
			}
			else
			{
				objectName = generateName(className);
				node->setProperty("name", objectName);
			}
			rect.x = node->pfloat("x");
			rect.y = node->pfloat("y");
			rect.w = node->pfloat("w", -1.0f);
			rect.h = node->pfloat("h", -1.0f);
		}
		else if (*node == "Animator")
		{
			objectName = node->pstr("name", generateName("Animator"));
		}
		else
		{
			return NULL;
		}
		if (mObjects.has_key(objectName))
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
			object = parseExternalObjectClass(node, objectName, rect);
		}

		if (object == NULL)
		{
			throw hlxml::XMLUnknownClassException(className, node);
		}
		object->_setDataset(this);
		mObjects[objectName] = object;
		if (mRoot == NULL)
		{
			mRoot = object;
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
				recursiveObjectParse(child, object);
			}
		}
		return object;
	}
	
	void Dataset::parseGlobalInclude(chstr path)
	{
		hstr originalFilePath = mFilePath;
		mFilePath = _makeFilePath(path);
		if (!path.contains("*"))
		{
			readFile(path);
			mFilePath = originalFilePath;
			return;
		}
		hstr extension = get_basename(path).replace("*", "");
		harray<hstr> contents = hdir::resource_files(mFilePath, true).sorted();
		foreach (hstr, it, contents)
		{
			if ((*it).ends_with(extension))
			{
				readFile((*it));
			}
		}
		mFilePath = originalFilePath;
	}
	
	void Dataset::parseObjectIncludeFile(chstr filename, Object* parent)
	{
		// parse dataset xml file, error checking first
		hstr path = normalize_path(filename);

		log("parsing object include file " + path);
		hlxml::Document* doc = hlxml::open(path);
		hlxml::Node* current = doc->root();
		
		foreach_xmlnode (p, current)
		{
			if (*p == "Object" || *p == "Animator")
			{
				recursiveObjectParse(p, parent);
			}
		}
		hlxml::close(doc);
	}
	
	void Dataset::parseObjectInclude(chstr path, Object* parent)
	{
		if (!path.contains("*"))
		{
			parseObjectIncludeFile(path, parent);
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
				parseObjectIncludeFile(basedir + "/" + (*it), parent);
			}
		}
	}
	
	void Dataset::readFile(chstr filename)
	{
		// parse dataset xml file, error checking first
		hstr path = normalize_path(filename);
		aprilui::log("parsing dataset file '" + path + "'");
		hlxml::Document* doc = hlxml::open(path);
		hlxml::Node* current = doc->root();

		parseExternalXMLNode(current);

		foreach_xmlnode (p, current)
		{
			if		(*p == "Texture")			parseTexture(p);
			else if	(*p == "RamTexture")		parseRamTexture(p);
			else if	(*p == "CompositeImage")	parseCompositeImage(p);
			else if	(*p == "Object")			parseObject(p);
			else if	(*p == "Include")			parseGlobalInclude(get_basedir(path) + "/" + p->pstr("path"));
			else if	(*p == "TextureGroup")		parseTextureGroup(p);
			else
			{
				parseExternalXMLNode(p);
			}
		}
		hlxml::close(doc);
	}

	void Dataset::load()
	{
		_loadTexts(_makeTextsPath());
		readFile(mFilename);
		mLoaded = true;
		update(0.0f);
	}

	hstr Dataset::_makeTextsPath()
	{
		hstr filepathPrefix = mFilePath + "/" + (mTextsPath != "" ? mTextsPath : getDefaultTextsPath()) + "/";
		hstr filepath = normalize_path(filepathPrefix + getLocalization());
		if (!hdir::resource_exists(filepath))
		{
			filepath = normalize_path(filepathPrefix + getDefaultLocalization());
		}
		return filepath;
	}
	
	void Dataset::_loadTexts(chstr path)
	{
		aprilui::log("loading texts from '" + path + "'");
		harray<hstr> files = hdir::resource_files(path, true);
		harray<hstr> lines;
		harray<hstr> values;
		bool keyMode = true;
		hstr key;
		hresource f;
		foreach (hstr, it, files)
		{
			f.open(*it);
			if (!f.is_open())
			{
				throw hl_exception("Failed to load file " + (*it));
			}
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
		if (!mLoaded)
		{
			throw GenericException("Unable to unload dataset '" + getName() + "', data not loaded!");
		}
		foreach_m (Object*, it, mObjects)
		{
			delete it->second;
		}
		mObjects.clear();
		foreach_m (Image*, it, mImages)
		{
			delete it->second;
		}
		mImages.clear();
		foreach_m (Texture*, it, mTextures)
		{
			delete it->second;
		}
		mTextures.clear();
		mCallbacks.clear();
		mTexts.clear();
		mRoot = NULL;
		mLoaded = false;
	}
	
	void Dataset::registerManualObject(Object* object)
	{
		hstr name = object->getName();
		if (mObjects.has_key(name))
		{
			throw ResourceExistsException(name, "Object", this);
		}
		mObjects[name] = object;
		object->_setDataset(this);
	}
	
	void Dataset::unregisterManualObject(Object* object)
	{
		hstr name = object->getName();
		if (!mObjects.has_key(name))
		{
			throw ResourceNotExistsException(name, "Object", this);
		}
		mObjects.remove_key(name);
		object->_setDataset(NULL);
	}
	
	void Dataset::registerManualImage(Image* image)
	{
		hstr name = image->getName();
		if (mImages.has_key(name))
		{
			throw ResourceExistsException(name, "Image", this);
		}
		mImages[name] = image;
	}
	
	void Dataset::unregisterManualImage(Image* image)
	{
		hstr name = image->getName();
		if (!mImages.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		mImages.remove_key(name);
	}
	
	void Dataset::registerManualTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (mTextures.has_key(name))
		{
			throw ResourceExistsException(name, "Texture", this);
		}
		mTextures[name] = texture;
	}

	void Dataset::unregisterManualTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (!mTextures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		mTextures.remove_key(name);
	}
	
	bool Dataset::isAnimated()
	{
		aprilui::Animator* object;
		foreach_m (Object*, it, mObjects)
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
		foreach_m (Object*, it, mObjects)
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
		if (!mObjects.has_key(name))
		{
			throw ResourceNotExistsException(name, "Object", this);
		}
		return mObjects[name];
	}
	
	bool Dataset::hasObject(chstr name)
	{
		return tryGetObject(name) != NULL;
	}
	
	bool Dataset::hasImage(chstr name)
	{
		return mImages.has_key(name);
	}
	
	Object* Dataset::tryGetObject(chstr name)
	{
		return mObjects.try_get_by_key(name, NULL);
	}
	
	Texture* Dataset::getTexture(chstr name)
	{
		if (!mTextures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		return mTextures[name];
	}
	
	Image* Dataset::getImage(chstr name)
	{
		Image* image;
		if (name == "null")
		{
			return &nullImage;
		}
		
		if (!mImages.has_key(name) && name.starts_with("0x")) // create new image with a color. don't overuse this, it's meant to be handy when needed only ;)
		{
			image = new ColorImage(name);
			mImages[name] = image;
		}
		else
		{
			image = mImages[name];
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
				dataset = getDatasetByName(name(0, dot));
			}
			catch (_GenericException)
			{
				throw ResourceNotExistsException(name, "Image", this);
			}
			image = dataset->getImage(name(dot + 1, 100));
		}
		return image;
	}
	
	hstr Dataset::getTextEntry(chstr textKey)
	{
		return mTexts.try_get_by_key(textKey, "");
	}
	
	bool Dataset::hasTextEntry(chstr textKey)
	{
		return mTexts.has_key(textKey);
	}
	
	hstr Dataset::getText(chstr compositeTextKey)
	{
		return _parseCompositeTextKey(compositeTextKey);
	}
	
	harray<hstr> Dataset::getTextEntries(harray<hstr> keys)
	{
		harray<hstr> output;
		foreach (hstr, it, keys)
		{
			output += getTextEntry(*it);
		}
		return output;
	}

	void Dataset::registerCallback(chstr name, void (*callback)())
	{
		mCallbacks[name] = callback;
	}
	
	void Dataset::triggerCallback(chstr name)
	{
		if (mCallbacks.has_key(name))
		{
			(*mCallbacks[name])();
		}
	}
	
	void Dataset::draw()
	{
		if (mRoot != NULL)
		{
			mRoot->draw();
		}
	}
	
	bool Dataset::onMouseDown(int button)
	{
		return (mRoot != NULL && mRoot->onMouseDown(button));
	}
	
	bool Dataset::onMouseUp(int button)
	{
		return (mRoot != NULL && mRoot->onMouseUp(button));
	}
	
	void Dataset::onMouseMove()
	{
		if (mRoot != NULL)
		{
			mRoot->onMouseMove();
		}
	}
	
	void Dataset::onMouseScroll(float x, float y)
	{
		if (mRoot != NULL)
		{
			mRoot->onMouseScroll(x, y);
		}
	}
	
	void Dataset::onKeyDown(unsigned int keycode)
	{
		if (mRoot != NULL)
		{
			mRoot->onKeyDown(keycode);
		}
	}
	
	void Dataset::onKeyUp(unsigned int keycode)
	{
		if (mRoot != NULL)
		{
			mRoot->onKeyUp(keycode);
		}
	}
	
	void Dataset::onChar(unsigned int charcode)
	{
		if (mRoot != NULL)
		{
			mRoot->onChar(charcode);
		}
	}
	
	void Dataset::updateTextures(float k)
	{
		foreach_m (Texture*, it, mTextures)
		{
			it->second->update(k);
		}
	}
	
	void Dataset::unloadUnusedTextures()
	{
		foreach_m (Texture*, it, mTextures)
		{
			if (it->second->isDynamic() && it->second->getUnusedTime() > 1.0f)
			{
				it->second->unload();
			}
		}
	}
	
	void Dataset::update(float k)
	{
		updateTextures(k);
		if (mRoot != NULL)
		{
			mRoot->update(k);
		}
		foreach_m (aprilui::Object*, it, mObjects)
		{
			it->second->clearChildUnderCursor();
		}
	}

	void Dataset::notifyEvent(chstr name, void* params)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			it->second->notifyEvent(name, params);
		}
	}
	
	void Dataset::reloadTexts()
	{
		mTexts.clear();
		_loadTexts(_makeTextsPath());
	}
	
	void Dataset::reloadTextures()
	{
		foreach_m (aprilui::Texture*, it, mTextures)
		{
			it->second->reload(_makeLocalizedTextureName(it->second->getOriginalFilename()));
		}
	}

	void Dataset::removeFocus()
	{
		if (mFocusedObject != NULL)
		{
			april::window->terminateKeyboardHandling();
		}
		mFocusedObject = NULL;
	}
	
	hstr Dataset::_parseCompositeTextKey(chstr key)
	{
		if (!key.starts_with("{"))
		{
			if (!hasTextEntry(key))
			{
				aprilui::log(hsprintf("WARNING: Text key '%s' does not exist!", key.c_str()));
			}
			return getTextEntry(key);
		}
		int index = key.find_first_of('}');
		if (index < 0)
		{
			aprilui::log(hsprintf("WARNING: Error while trying to parse formatted key '%s'.", key.c_str()));
			return key;
		}
		harray<hstr> args;
		hstr format = key(1, index - 1);
		hstr argString = key(index + 1, key.size() - index - 1).trim(' ');
		if (!_processCompositeTextKeyArgs(argString, args))
		{
			aprilui::log(hsprintf("- while processing args: '%s' with args '%s'.", format.c_str(), argString.c_str()));
			return key;
		}
		hstr preprocessedFormat;
		harray<hstr> preprocessedArgs;
		if (!_preprocessCompositeTextKeyFormat(format, args, preprocessedFormat, preprocessedArgs))
		{
			aprilui::log(hsprintf("- while preprocessing format: '%s' with args '%s'.", format.c_str(), argString.c_str()));
			return key;
		}
		hstr result;
		if (!_processCompositeTextKeyFormat(preprocessedFormat, preprocessedArgs, result))
		{
			aprilui::log(hsprintf("- while processing format: '%s' with args '%s'.", format.c_str(), argString.c_str()));
			return key;
		}
		return result;
	}


	bool Dataset::_processCompositeTextKeyArgs(chstr argString, harray<hstr>& args)
	{
		args.clear();
		// splittings args
		hstr string = argString;
		harray<hstr> keys;
		int openIndex;
		int closeIndex;
		while (string.size() > 0)
		{
			openIndex = string.find_first_of('{');
			closeIndex = string.find_first_of('}');
			if (openIndex < 0 && closeIndex < 0)
			{
				args += getTextEntries(string.split(" ", -1, true));
				break;
			}
			if (openIndex < 0 || closeIndex < 0)
			{
				aprilui::log("WARNING: '{' without '}' or '}' without '{'");
				return false;
			}
			if (closeIndex < openIndex)
			{
				aprilui::log("WARNING: '}' before '{'");
				return false;
			}
			// getting all args before the {
			args += getTextEntries(string(0, openIndex).split(" ", -1, true));
			// getting args inside of {}
			args += string(openIndex + 1, closeIndex - openIndex - 1);
			// rest of the args
			string = string(closeIndex + 1, string.size() - closeIndex - 1);
		}
		return true;
	}

	bool Dataset::_preprocessCompositeTextKeyFormat(chstr format, harray<hstr> args, hstr& preprocessedFormat, harray<hstr>& preprocessedArgs)
	{
		preprocessedFormat = "";
		preprocessedArgs.clear();
		// preprocessing of format string and args
		hstr string = format;
		int index;
		hstr arg;
		harray<int> indexes;
		while (string.size() > 0)
		{
			index = string.find_first_of('%');
			if (index < 0)
			{
				preprocessedFormat += string;
				break;
			}
			if (index >= string.size() - 1)
			{
				aprilui::log("WARNING: Last character is '%'");
				return false;
			}
			if (string[index + 1] == '%') // escaped "%", continue processing
			{
				preprocessedFormat += string(0, index + 2);
				string = string(index + 2, string.size() - index - 2);
				continue;
			}
			if (string[index + 1] == 's') // %s, not processing that now
			{
				if (args.size() == 0)
				{
					aprilui::log("WARNING: Not enough args");
					return false;
				}
				preprocessedFormat += string(0, index + 2);
				string = string(index + 2, string.size() - index - 2);
				preprocessedArgs += args.pop_first();
				continue;
			}
			if (string[index + 1] == 'f')
			{
				if (args.size() == 0)
				{
					aprilui::log("WARNING: Not enough args");
					return false;
				}
				hstr arg = args.pop_first();
				preprocessedFormat += string(0, index) + arg;
				string = string(index + 2, string.size() - index - 2);
				if (!_getCompositeTextKeyFormatIndexes(arg, indexes))
				{
					return false;
				}
				if (indexes.size() > args.size())
				{
					aprilui::log("WARNING: Not enough args");
					return false;
				}
				preprocessedArgs += args.pop_first(indexes.size());
			}
		}
		preprocessedArgs += args; // remaining args
		return true;
	}

	bool Dataset::_processCompositeTextKeyFormat(chstr format, harray<hstr> args, hstr& result)
	{
		result = "";
		// preprocessing of format string and args
		hstr string = format;
		harray<int> indexes;
		if (!_getCompositeTextKeyFormatIndexes(format, indexes))
		{
			return false;
		}
		if (args.size() < indexes.size())
		{
			aprilui::log("WARNING: Not enough args");
			return false;
		}
		if (indexes.size() > args.size())
		{
			aprilui::log("WARNING: Too many args");
			return false;
		}
		foreach (int, it, indexes)
		{
			result += string(0, (*it));
			result += args.pop_first();
			string = string((*it) + 2, string.size() - (*it) - 2);
		}
		result += string;
		result = result.replace("%%", "%");
		return true;
	}

	bool Dataset::_getCompositeTextKeyFormatIndexes(chstr format, harray<int>& indexes)
	{
		indexes.clear();
		// finding formatting indexes
		hstr string = format;
		int index;
		int currentIndex = 0;
		while (string.size() > 0)
		{
			index = string.find_first_of('%');
			if (index < 0)
			{
				break;
			}
			if (index >= string.size() - 1)
			{
				aprilui::log("WARNING: Last character is '%'");
				return false;
			}
			if (string[index + 1] == '%') // escaped "%", use just one "%".
			{
				string = string(index + 2, string.size() - index - 2);
				currentIndex += index + 2;
				continue;
			}
			if (string[index + 1] != 's')
			{
				aprilui::log(hsprintf("WARNING: Unsupported formatting '%%%c'", string[index + 1]));
				return false;
			}
			indexes += currentIndex + index;
			string = string(index + 2, string.size() - index - 2);
			currentIndex = 0;
		}
		return true;
	}

}
