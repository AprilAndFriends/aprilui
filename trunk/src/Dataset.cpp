/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/april.h>
#include <april/RenderSystem.h>
#include <april/Texture.h>
#include <april/Window.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hrdir.h>
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
		this->dataset = this;
		this->focusedObject = NULL;
		this->root = NULL;
		this->filename = hrdir::normalize(filename);
		this->filePath = this->_makeFilePath(this->filename, name, useNameBasePath);
		this->name = name;
		this->nullImage = NULL;
		if (this->name == "")
		{
			this->name = hrdir::basename(hresource::no_extension(this->filename));
		}
		this->nullImage = new NullImage();
		this->nullImage->dataset = this;
		this->loaded = false;
		aprilui::_registerDataset(this->name, this);
	}
	
	Dataset::~Dataset()
	{
		delete this->nullImage;
		aprilui::_unregisterDataset(this->name, this);
		if (this->isLoaded())
		{
			if (this->focusedObject != NULL)
			{
				april::window->terminateKeyboardHandling();
			}
			this->unload();
		}
	}

	hmap<hstr, BaseObject*> Dataset::getAllObjects()
	{
		hmap<hstr, BaseObject*> result = this->animators.cast<hstr, BaseObject*>();
		result.inject(this->objects.cast<hstr, BaseObject*>());
		return result;
	}

	int Dataset::getFocusedObjectIndex()
	{
		return (this->focusedObject != NULL && this->focusedObject->isEnabled() &&
			this->focusedObject->isVisible() ? this->focusedObject->getFocusIndex() : -1);
	}

	bool Dataset::trySetFocusedObjectByIndex(int value, bool strict)
	{
		if (value < 0)
		{
			this->removeFocus();
			return true;
		}
		foreach_m (Object*, it, this->objects)
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
		int focusIndex = 0;
		foreach_m (Object*, it, this->objects)
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
		int focusIndex = 0;
		foreach_m (Object*, it, this->objects)
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
			hstr newFilename = name + "." + hresource::extension_of(filename);
			if (filename.ends_with(newFilename))
			{
				return hrdir::normalize(filename.replace(newFilename, ""));
			}
		}
		return hrdir::normalize(hrdir::basedir(filename));
	}
	
	void Dataset::destroyObjects(chstr rootName)
	{
		this->destroyObjects(this->getObject(rootName));
	}
	
	void Dataset::destroyObjects(BaseObject* root)
	{
		if (!this->objects.has_key(root->getName()) && !this->animators.has_key(root->getName()))
		{
			// this object could be from another dataset, so check that first.
			Dataset* dataset = root->getDataset();
			if (dataset != this)
			{
				hlog::writef(logTag, "Dataset '%s' destroying object from another dataset: '%s'", this->getName().c_str(), root->getFullName().c_str());
				dataset->destroyObjects(root);
				return;
			}
			throw ResourceNotExistsException(root->getName(), "Object", this);
		}
		harray<BaseObject*> children = root->getChildren();
		foreach (BaseObject*, it, children)
		{
			this->destroyObjects(*it);
		}
		if (root->getParent() != NULL)
		{
			root->detach();
		}
		Object* object = dynamic_cast<Object*>(root);
		if (object != NULL)
		{
			if (object->isFocused())
			{
				object->setFocused(false);
			}
			this->objects.remove_key(root->getName());
		}
		else
		{
			Animator* animator = dynamic_cast<Animator*>(root);
			if (animator != NULL)
			{
				this->animators.remove_key(root->getName());
			}
		}
		delete root;
	}
	
	void Dataset::_destroyTexture(Texture* texture)
	{
		hstr filename = texture->getFilename();
		if (!this->textures.has_key(filename))
		{
			throw ResourceNotExistsException(filename, "Texture", this);
		}
		this->textures.remove_key(filename);
		delete texture;
	}
	
	void Dataset::_destroyImage(Image* image)
	{
		hstr name = image->getName();
		if (!this->images.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		this->images.remove_key(name);
		delete image;
	}
	
	void Dataset::_destroyTexture(chstr name)
	{
		if (!this->textures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		delete this->textures[name];
		this->textures.remove_key(name);
	}
	
	void Dataset::_destroyImage(chstr name)
	{
		if (!this->images.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		delete this->images[name];
		this->images.remove_key(name);
	}

	hstr Dataset::_makeLocalizedTextureName(chstr filename)
	{
		hstr localization = aprilui::getLocalization();
		if (localization != "")
		{
			hstr locpath = hrdir::join_path(hrdir::join_path(hrdir::basedir(filename), localization), hrdir::basename(filename));
			locpath = april::rendersys->findTextureResource(locpath);
			if (locpath != "")
			{
				return locpath;
			}
		}
		return filename;
	}
	
	void Dataset::parseTexture(hlxml::Node* node)
	{
		hstr filename = hrdir::normalize(node->pstr("filename"));
		hstr filepath = hrdir::normalize(hrdir::join_path(this->filePath, filename, false));
		hstr textureName = hrdir::basename(filename);
		if (this->textures.has_key(textureName))
		{
			throw ObjectExistsException(textureName, filename);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		bool managed = node->pbool("managed", aprilui::isDefaultManagedTextures());
		april::Texture::LoadMode loadMode = aprilui::getDefaultTextureLoadMode();
		// DEPRECATED
		if (node->pexists("dynamic_load"))
		{
			hlog::warn(aprilui::logTag, "\"dynamic_load\" is deprecated. Use \"managed\" and \"load_mode\" instead.");
			bool dynamicLoad = node->pbool("dynamic_load");
			if (dynamicLoad)
			{
				managed = true;
				loadMode = april::Texture::LOAD_ON_DEMAND;
			}
			else
			{
				managed = false;
				loadMode = april::Texture::LOAD_ASYNC;
			}
		}
		if (node->pexists("load_mode"))
		{
			hstr mode = node->pstr("load_mode");
			if (mode == "immediate")			loadMode = april::Texture::LOAD_IMMEDIATE;
			else if (mode == "on_demand")		loadMode = april::Texture::LOAD_ON_DEMAND;
			else if (mode == "async")			loadMode = april::Texture::LOAD_ASYNC;
			else if (mode == "async_on_demand")	loadMode = april::Texture::LOAD_ASYNC_ON_DEMAND;
			else throw hl_exception("load_mode '" + mode + "' not supported");
		}
		hstr locpath = this->_makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->createTextureFromResource(locpath, april::Texture::TYPE_IMMUTABLE, loadMode);
		if (aprilTexture == NULL)
		{
			throw file_could_not_open(locpath);
		}
		Texture* texture = new Texture(filepath, aprilTexture, managed);
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if		(filter == "linear")	texture->setFilter(april::Texture::FILTER_LINEAR);
			else if	(filter == "nearest")	texture->setFilter(april::Texture::FILTER_NEAREST);
			else throw hl_exception("texture filter '" + filter + "' not supported");
		}
		if (node->pbool("wrap", false))
		{
			texture->setAddressMode(april::Texture::ADDRESS_WRAP);
		}
		else
		{
			texture->setAddressMode(april::Texture::ADDRESS_CLAMP);
		}
		this->textures[textureName] = texture;
		// extract image definitions
		if (node->iterChildren() == NULL) // if there are no images defined, create one that fills the whole area
		{
			if (this->images.has_key(textureName))
			{
				throw ResourceExistsException(filename, "Texture", this);
			}
			Image* img = new Image(texture, filename, grect(0, 0, (float)texture->getWidth(), (float)texture->getHeight()));
			this->images[textureName] = img;
			img->dataset = this;
		}
		else
		{
			Image* image = NULL;
			hstr name;
			grect rect;
			gvec2 tile;
			foreach_xmlnode (child, node)
			{
				if (*child == "Image")
				{
					name = (prefixImages ? textureName + "/" + child->pstr("name") : child->pstr("name"));
					if (this->images.has_key(name))
					{
						throw ResourceExistsException(name, "Image", this);
					}
					aprilui::read_rect_node(rect, child);
					tile.set(1.0f, 1.0f);
					if (child->pexists("tile"))
					{
						tile = hstr_to_gvec2(child->pstr("tile"));
					}
					else
					{
						tile.set(child->pfloat("tile_w", 1.0f), child->pfloat("tile_h", 1.0f));
					}
					if (tile.x != 1.0f || tile.y != 1.0f)
					{
						image = new TiledImage(texture, name, rect, tile.x, tile.y);
					}
					else
					{
						image = new Image(texture, name, rect);	
					}
					this->images[name] = image;
					image->dataset = this;
					foreach_xmlproperty (prop, child)
					{
						name = prop->name();
						if (name == "rect" || name == "position" || name == "size" || name == "x" || name == "y" || name == "w" || name == "h")
						{
							continue; // TODO - should be done better, maybe reading parameters from a list, then removing them so they aren't set more than once
						}
						image->setProperty(name, prop->value());
					}
				}
			}
		}
	}
	
	void Dataset::parseCompositeImage(hlxml::Node* node)
	{
		hstr name = node->pstr("name");
		hstr refname;
		if (this->images.has_key(name))
		{
			throw ResourceExistsException(name, "CompositeImage", this);
		}
		gvec2 size;
		if (node->pexists("size"))
		{
			size = hstr_to_gvec2(node->pstr("size"));
		}
		else
		{
			size.set(node->pfloat("w"), node->pfloat("h"));
		}
		CompositeImage* image = new CompositeImage(name, size.x, size.y);
		grect rect;
		foreach_xmlnode (child, node)
		{
			if (*child == "ImageRef")
			{
				refname = child->pstr("name");
				aprilui::read_rect_node(rect, child);
				image->addImageRef(this->getImage(refname), rect);
			}
		}
		this->images[name] = image;
		image->dataset = this;
	}
	
	BaseObject* Dataset::parseObject(hlxml::Node* node, Object* parent)
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
					this->getTexture(*it)->addLink(this->getTexture(*it2));
				}
			}
		}
	}
	
	BaseObject* Dataset::recursiveObjectParse(hlxml::Node* node, Object* parent)
	{
		return this->recursiveObjectParse(node, parent, "", "", gvec2());
	}

	BaseObject* Dataset::recursiveObjectParse(hlxml::Node* node, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		hstr className;
		hstr objectName;
		grect rect(0.0f, 0.0f, 1.0f, 1.0f);
		if (*node == "Include")
		{
			gvec2 offset;
			if (node->pexists("position"))
			{
				offset = hstr_to_gvec2(node->pstr("position"));
			}
			else
			{
				offset.set(node->pfloat("x", 0.0f), node->pfloat("y", 0.0f));
			}
			this->parseObjectInclude(hrdir::join_path(this->filePath, node->pstr("path"), false), parent,
				node->pstr("name_prefix", "") + namePrefix, nameSuffix + node->pstr("name_suffix", ""), offset);
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
				objectName = april::generateName(className);
			}
			if (*node == "Object")
			{
				aprilui::read_rect_node(rect, node, true);
				rect += offset;
			}
		}
		else
		{
			return NULL;
		}
		if (this->objects.has_key(objectName))
		{
			throw ResourceExistsException(objectName, "Object", this);
		}
		BaseObject* baseObject = NULL;
		Object* object = NULL;
		Animator* animator = NULL;
		if (*node == "Object")
		{
			baseObject = object = aprilui::createObject(className, objectName, rect);
		}
		else if (*node == "Animator")
		{
			baseObject = animator = aprilui::createAnimator(className, objectName);
		}
		if (baseObject == NULL)
		{
			baseObject = this->parseExternalObjectClass(node, objectName, rect);
			if (baseObject != NULL)
			{
				object = dynamic_cast<Object*>(baseObject);
				if (object == NULL)
				{
					animator = dynamic_cast<Animator*>(baseObject);
				}
			}
		}
		if (baseObject == NULL)
		{
			throw hlxml::XMLUnknownClassException(className, node);
		}
		baseObject->dataset = this;
		EventArgs args(this);
		baseObject->notifyEvent(Event::RegisteredInDataset, &args);
		if (object != NULL)
		{
			this->objects[objectName] = object;
			if (this->root == NULL)
			{
				this->root = object;
			}
			if (parent != NULL)
			{
				parent->addChild(object);
			}
		}
		else if (animator != NULL)
		{
			this->animators[objectName] = animator;
			if (parent != NULL)
			{
				parent->addChild(animator);
			}
		}
		hstr name;
		foreach_xmlproperty (prop, node)
		{
			name = prop->name();
			if (name == "rect" || name == "position" || name == "size" || name == "x" || name == "y" || name == "w" || name == "h")
			{
				continue; // TODO - should be done better, maybe reading parameters from a list, then removing them so they aren't set more than once
			}
			baseObject->setProperty(name, prop->value());
		}
		if (object != NULL)
		{
			hlxml::Node::Type type;
			foreach_xmlnode (child, node)
			{
				type = child->getType();
				if (type != hlxml::Node::TYPE_TEXT && type != hlxml::Node::TYPE_COMMENT)
				{
					this->recursiveObjectParse(child, object, namePrefix, nameSuffix, gvec2());
				}
			}
		}
		return baseObject;
	}
	
	void Dataset::parseGlobalIncludeFile(chstr filename)
	{
		// parse dataset xml file, error checking first
		hstr originalFilePath = this->filePath;
		this->filePath = this->_makeFilePath(filename);
		this->readFile(filename);
		this->filePath = originalFilePath;
	}
	
	void Dataset::parseGlobalInclude(chstr path)
	{
		hstr originalFilePath = this->filePath;
		this->filePath = this->_makeFilePath(path);
		if (!path.contains("*"))
		{
			this->readFile(path);
			this->filePath = originalFilePath;
			return;
		}
		hstr extension = hrdir::basename(path).replace("*", "");
		harray<hstr> contents = hrdir::files(this->filePath, true).sorted();
		foreach (hstr, it, contents)
		{
			if ((*it).ends_with(extension))
			{
				this->readFile((*it));
			}
		}
		this->filePath = originalFilePath;
	}
	
	void Dataset::parseObjectIncludeFile(chstr filename, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		// parse dataset xml file, error checking first
		hstr path = hrdir::normalize(filename);
		hlog::write(aprilui::logTag, "Parsing object include file: " + path);
		hlxml::Document* doc = this->_openDocument(path);
		hlxml::Node* current = doc->root();
		foreach_xmlnode (node, current)
		{
			if (*node == "Object" || *node == "Animator")
			{
				this->recursiveObjectParse(node, parent, namePrefix, nameSuffix, offset);
			}
		}
	}
	
	void Dataset::parseObjectInclude(chstr path, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		if (!path.contains("*"))
		{
			this->parseObjectIncludeFile(path, parent, namePrefix, nameSuffix, offset);
			return;
		}
		hstr basedir = hrdir::basedir(path);
		hstr filename = path(basedir.size() + 1, -1);
		hstr left;
		hstr right;
		filename.split("*", left, right);
		harray<hstr> contents = hrdir::files(basedir).sorted();
		foreach (hstr, it, contents)
		{
			if ((*it).starts_with(left) && (*it).ends_with(right))
			{
				this->parseObjectIncludeFile(hrdir::join_path(basedir, (*it), false), parent, "", "", gvec2());
			}
		}
	}
	
	hlxml::Document* Dataset::_openDocument(chstr filename)
	{
		hlxml::Document* document = this->includeDocuments.try_get_by_key(filename, NULL);
		if (document == NULL)
		{
			document = new hlxml::Document(filename);
			this->includeDocuments[filename] = document;
		}
		return document;
	}
	
	void Dataset::_closeDocuments()
	{
		foreach_m (hlxml::Document*, it, this->includeDocuments)
		{
			delete it->second;
		}
		this->includeDocuments.clear();
	}
	
	void Dataset::readFile(chstr filename)
	{
		// parse dataset xml file, error checking first
		hstr path = hrdir::normalize(filename);
		hlog::write(aprilui::logTag, "Parsing dataset file: " + path);
		hlxml::Document doc(path);
		hlxml::Node* current = doc.root();
		this->parseExternalXMLNode(current);
		foreach_xmlnode (node, current)
		{
			if		(*node == "Texture")		parseTexture(node);
			else if	(*node == "CompositeImage")	parseCompositeImage(node);
			else if	(*node == "Object")			parseObject(node);
			else if	(*node == "Include")		parseGlobalInclude(hrdir::join_path(hrdir::basedir(path), node->pstr("path"), false));
			else if	(*node == "TextureGroup")	parseTextureGroup(node);
			else
			{
				this->parseExternalXMLNode(node);
			}
		}
	}

	void Dataset::load()
	{
		if (this->filename != "")
		{
			this->_loadTexts(this->_makeTextsPath());
			try
			{
				this->readFile(this->filename);
				this->_closeDocuments(); // safe not to throw an exception
			}
			catch (hltypes::exception& e)
			{
				hlog::error(aprilui::logTag, e.message());
				this->_closeDocuments(); // safe not to throw an exception
				throw e;
			}
		}
		this->loaded = true;
		this->update(0.0f);
	}

	hstr Dataset::_makeTextsPath()
	{
		hstr filepathPrefix = hrdir::join_path(this->filePath, (this->textsPath != "" ? this->textsPath : aprilui::getDefaultTextsPath()), false);
		hstr filepath = hrdir::normalize(hrdir::join_path(filepathPrefix, aprilui::getLocalization(), false));
		if (!hrdir::exists(filepath))
		{
			filepath = hrdir::normalize(hrdir::join_path(filepathPrefix, aprilui::getDefaultLocalization(), false));
		}
		return filepath;
	}
	
	void Dataset::_loadTexts(chstr path)
	{
		hlog::write(aprilui::logTag, "Loading texts: " + path);
		harray<hstr> files = hrdir::files(path, true);
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
					++i;
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
						this->texts[key] = values.join('\n');
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
		if (!this->loaded)
		{
			throw GenericException("Unable to unload dataset '" + this->getName() + "', data not loaded!");
		}
		foreach_m (Animator*, it, this->animators)
		{
			if (it->second->getParent() != NULL)
			{
				it->second->detach();
			}
			delete it->second;
		}
		this->animators.clear();
		foreach_m (Object*, it, this->objects)
		{
			if (it->second->getParent() != NULL)
			{
				it->second->detach();
			}
			it->second->removeChildren(false);
			delete it->second;
		}
		this->objects.clear();
		foreach_m (Image*, it, this->images)
		{
			delete it->second;
		}
		this->images.clear();
		foreach_m (Texture*, it, this->textures)
		{
			delete it->second;
		}
		this->textures.clear();
		this->callbacks.clear();
		this->texts.clear();
		this->root = NULL;
		this->focusedObject = NULL;
		this->loaded = false;
	}
	
	void Dataset::registerObjects(BaseObject* root)
	{
		hstr name;
		harray<BaseObject*> objects;
		objects += root;
		objects += root->getDescendants();
		Object* object = NULL;
		Animator* animator = NULL;
		EventArgs args(this);
		foreach (BaseObject*, it, objects)
		{
			name = (*it)->getName();
			if (this->objects.has_key(name) || this->animators.has_key(name))
			{
				throw ResourceExistsException(name, "Object", this);
			}
			object = dynamic_cast<Object*>(*it);
			if (object != NULL)
			{
				this->objects[name] = object;
			}
			else
			{
				animator = dynamic_cast<Animator*>(*it);
				if (animator != NULL)
				{
					this->animators[name] = animator;
				}
			}
			(*it)->dataset = this;
			(*it)->notifyEvent(Event::RegisteredInDataset, &args);
		}
	}
	
	void Dataset::unregisterObjects(BaseObject* root)
	{
		if (!this->objects.has_key(root->getName()))
		{
			// this object could be from another dataset, so check that first.
			Dataset* dataset = root->getDataset();
			if (dataset != this)
			{
				hlog::writef(logTag, "Dataset '%s' unregistering object from another dataset: '%s'", this->getName().c_str(), root->getFullName().c_str());
				dataset->unregisterObjects(root);
				return;
			}
			throw ResourceNotExistsException(root->getName(), "Object", this);
		}
		harray<BaseObject*> children = root->getChildren();
		foreach (BaseObject*, it, children)
		{
			this->unregisterObjects(*it);
		}
		Object* focusedRoot = dynamic_cast<Object*>(root);
		if (focusedRoot != NULL && focusedRoot->isFocused())
		{
			focusedRoot->setFocused(false);
		}
		this->objects.remove_key(root->getName());
		root->dataset = NULL;
	}
	
	void Dataset::registerImage(Image* image)
	{
		hstr name = image->getName();
		if (this->images.has_key(name))
		{
			throw ResourceExistsException(name, "Image", this);
		}
		this->images[name] = image;
		image->dataset = this;
	}
	
	void Dataset::unregisterImage(Image* image)
	{
		hstr name = image->getName();
		if (!this->images.has_key(name))
		{
			throw ResourceNotExistsException(name, "Image", this);
		}
		this->images.remove_key(name);
		image->dataset = NULL;
	}
	
	void Dataset::registerTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (this->textures.has_key(name))
		{
			throw ResourceExistsException(name, "Texture", this);
		}
		this->textures[name] = texture;
	}

	void Dataset::unregisterTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (!this->textures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		this->textures.remove_key(name);
	}
	
	bool Dataset::isAnimated()
	{
		foreach_m (Object*, it, this->objects)
		{
			if (it->second->isAnimated())
			{
				return true;
			}
		}
		foreach_m (Animator*, it, this->animators)
		{
			if (it->second->isAnimated())
			{
				return true;
			}
		}
		return false;
	}
	
	bool Dataset::isWaitingAnimation()
	{
		foreach_m (Object*, it, this->objects)
		{
			if (it->second->isWaitingAnimation())
			{
				return true;
			}
		}
		foreach_m (Animator*, it, this->animators)
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
		int dot = name.find('.');
		if (dot < 0)
		{
			if (!this->objects.has_key(name))
			{
				throw ResourceNotExistsException(name, "Object", this);
			}
			return this->objects[name];
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (_GenericException&)
		{
			throw ResourceNotExistsException(name, "Object", this);
		}
		return dataset->getObject(name(dot + 1, -1));
	}

	Animator* Dataset::getAnimator(chstr name)
	{
		int dot = name.find('.');
		if (dot < 0)
		{
			if (!this->animators.has_key(name))
			{
				throw ResourceNotExistsException(name, "Animator", this);
			}
			return this->animators[name];
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (_GenericException&)
		{
			throw ResourceNotExistsException(name, "Animator", this);
		}
		return dataset->getAnimator(name(dot + 1, -1));
	}

	bool Dataset::hasObject(chstr name)
	{
		return (this->tryGetObject(name) != NULL);
	}
	
	bool Dataset::hasAnimator(chstr name)
	{
		return (this->tryGetAnimator(name) != NULL);
	}
	
	bool Dataset::hasImage(chstr name)
	{
		return this->images.has_key(name);
	}
	
	bool Dataset::hasTexture(chstr name)
	{
		return this->textures.has_key(name);
	}
	
	Object* Dataset::tryGetObject(chstr name)
	{
		int dot = name.find('.');
		if (dot < 0)
		{
			return this->objects.try_get_by_key(name, NULL);
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (_GenericException&)
		{
			return NULL;
		}
		return dataset->tryGetObject(name(dot + 1, -1));
	}

	Animator* Dataset::tryGetAnimator(chstr name)
	{
		int dot = name.find('.');
		if (dot < 0)
		{
			return this->animators.try_get_by_key(name, NULL);
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (_GenericException&)
		{
			return NULL;
		}
		return dataset->tryGetAnimator(name(dot + 1, -1));
	}

	Texture* Dataset::getTexture(chstr name)
	{
		if (!this->textures.has_key(name))
		{
			throw ResourceNotExistsException(name, "Texture", this);
		}
		return this->textures[name];
	}
	
	Image* Dataset::getImage(chstr name)
	{
		Image* image = NULL;
		if (name == APRILUI_IMAGE_NAME_NULL)
		{
			return this->nullImage;
		}
		if (this->images.has_key(name))
		{
			image = this->images[name];
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
			catch (_GenericException&)
			{
				throw ResourceNotExistsException(name, "Image", this);
			}
			image = dataset->getImage(name(dot + 1, -1));
		}
		return image;
	}

	bool Dataset::_findTextEntry(chstr textKey, hstr* text)
	{
		int dot = textKey.find('.');
		if (dot < 0)
		{
			if (!this->texts.has_key(textKey))
			{
				if (text != NULL)
				{
					*text = "ERROR: Text '" + textKey + "' not found!";
				}
				return false;
			}
			if (text != NULL)
			{
				*text = this->texts[textKey];
			}
			return true;
		}
		Dataset* dataset = aprilui::getDatasetByName(textKey(0, dot));
		return dataset->_findTextEntry(textKey(dot + 1, -1), text);
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
	
	harray<hstr> Dataset::getTexts(harray<hstr> keys)
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
		this->callbacks[name] = callback;
	}
	
	void Dataset::triggerCallback(chstr name)
	{
		if (this->callbacks.has_key(name))
		{
			(*this->callbacks[name])();
		}
	}
	
	void Dataset::draw()
	{
		if (this->root != NULL)
		{
			this->root->draw();
		}
	}
	
	bool Dataset::onMouseDown(april::Key keyCode)
	{
		return (this->root != NULL && this->root->onMouseDown(keyCode));
	}
	
	bool Dataset::onMouseUp(april::Key keyCode)
	{
		return (this->root != NULL && this->root->onMouseUp(keyCode));
	}
	
	bool Dataset::onMouseCancel(april::Key keyCode)
	{
		return (this->root != NULL && this->root->onMouseCancel(keyCode));
	}
	
	bool Dataset::onMouseMove()
	{
		return (this->root != NULL && this->root->onMouseMove());
	}
	
	bool Dataset::onMouseScroll(float x, float y)
	{
		return (this->root != NULL && this->root->onMouseScroll(x, y));
	}
	
	bool Dataset::onKeyDown(april::Key keyCode)
	{
		return (this->root != NULL && this->root->onKeyDown(keyCode));
	}
	
	bool Dataset::onKeyUp(april::Key keyCode)
	{
		return (this->root != NULL && this->root->onKeyUp(keyCode));
	}
	
	bool Dataset::onChar(unsigned int charCode)
	{
		return (this->root != NULL && this->root->onChar(charCode));
	}
	
	bool Dataset::onTouch(const harray<gvec2>& touches)
	{
		return (this->root != NULL && this->root->onTouch(touches));
	}
	
	bool Dataset::onButtonDown(april::Button buttonCode)
	{
		return (this->root != NULL && this->root->onButtonDown(buttonCode));
	}
	
	bool Dataset::onButtonUp(april::Button buttonCode)
	{
		return (this->root != NULL && this->root->onButtonUp(buttonCode));
	}
	
	void Dataset::updateTextures(float timeDelta)
	{
		foreach_m (Texture*, it, this->textures)
		{
			it->second->update(timeDelta);
		}
	}
	
	void Dataset::processEvents()
	{
		QueuedCallback callback;
		while (this->callbackQueue.size() > 0)
		{
			callback = this->callbackQueue.remove_first();
			callback.event->execute(callback.args);
			delete callback.args; // deleting only args because event is a pointer to object's events which get deleted by the owning object, while args are allocated by the callback queue.
		}
	}
	
	void Dataset::queueCallback(Event* event, EventArgs* args)
	{
		QueuedCallback callback;
		callback.event = event;
		callback.args = args;
		this->callbackQueue += callback;
	}
	
	void Dataset::removeCallbackFromQueue(Event* event)
	{
		if (event == NULL || this->callbackQueue.size() == 0)
		{
			return; // optimizations, callback queue is often empty.
		}
		// remove all instances of the given event
		harray<int> removeList;
		for_iter (i, 0, this->callbackQueue.size())
		{
			if (this->callbackQueue[i].event == event)
			{
				removeList += i;
			}
		}
		removeList.sort();
		removeList.reverse();
		foreach (int, it, removeList)
		{
			delete this->callbackQueue.remove_at(*it).args;
		}
	}
	
	void Dataset::update(float timeDelta)
	{
		this->updateTextures(timeDelta);
		if (this->root != NULL && this->root->getParent() == NULL)
		{
			this->root->update(timeDelta);
		}
		this->clearChildUnderCursor();
	}

	void Dataset::clearChildUnderCursor()
	{
		foreach_m (Object*, it, this->objects)
		{
			it->second->clearChildUnderCursor();
		}
	}

	void Dataset::notifyEvent(chstr type, EventArgs* args)
	{
		foreach_m (Object*, it, this->objects)
		{
			it->second->notifyEvent(type, args);
		}
		foreach_m (Animator*, it, this->animators)
		{
			it->second->notifyEvent(type, args);
		}
	}

	void Dataset::unloadUnusedTextures()
	{
		foreach_m (Texture*, it, this->textures)
		{
			if (it->second->isManaged() && it->second->getUnusedTime() > 1.0f)
			{
				it->second->unload();
			}
		}
	}

	void Dataset::reloadTexts()
	{
		this->texts.clear();
		this->_loadTexts(this->_makeTextsPath());
	}
	
	void Dataset::reloadTextures()
	{
		foreach_m (aprilui::Texture*, it, this->textures)
		{
			it->second->reload(this->_makeLocalizedTextureName(it->second->getOriginalFilename()));
		}
	}

	void Dataset::focus(Object* object)
	{
		this->removeFocus();
		this->focusedObject = object;
		this->focusedObject->notifyEvent(Event::FocusGained, NULL);
		this->focusedObject->triggerEvent(Event::FocusGained);
	}

	void Dataset::removeFocus()
	{
		if (this->focusedObject != NULL)
		{
			april::window->terminateKeyboardHandling();
			// in case of a recursive call, the focused object has to be removed first.
			aprilui::Object* object = this->focusedObject;
			this->focusedObject = NULL;
			object->triggerEvent(Event::FocusLost);
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
				++cstr;
			}
			argString = cstr;
			// r-trimming
			if (argString.size() > 0)
			{
				cstr = argString.c_str();
				int i = argString.size() - 1;
				while (i >= 0 && cstr[i] == ' ')
				{
					--i;
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
