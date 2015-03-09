/// @file
/// @version 4.0
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
#include <hltypes/harray.h>
#include <hltypes/hexception.h>
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
#include "BaseImage.h"
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
			this->name = hrdir::baseName(hresource::withoutExtension(this->filename));
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
			if (name.contains("/"))
			{
				hstr namePath = hrdir::baseDir(name);
				hstr newFilename = hrdir::baseDir(filename);
				if (newFilename.endsWith(namePath))
				{
					return hrdir::normalize(newFilename.replaced(namePath, ""));
				}
			}
			else
			{
				hstr namePath = name + "." + hresource::extensionOf(filename);
				if (filename.endsWith(namePath))
				{
					return hrdir::normalize(filename.replaced(namePath, ""));
				}
			}
		}
		return hrdir::normalize(hrdir::baseDir(filename));
	}
	
	void Dataset::destroyObjects(chstr rootName)
	{
		this->destroyObjects(this->getObject(rootName));
	}
	
	void Dataset::destroyObjects(BaseObject* root)
	{
		if (!this->objects.hasKey(root->getName()) && !this->animators.hasKey(root->getName()))
		{
			// this object could be from another dataset, so check that first.
			Dataset* dataset = root->getDataset();
			if (dataset != this)
			{
				hlog::writef(logTag, "Dataset '%s' destroying object from another dataset: '%s'", this->getName().cStr(), root->getFullName().cStr());
				dataset->destroyObjects(root);
				return;
			}
			throw ObjectNotExistsException("Object", root->getName(), this->name);
		}
		harray<BaseObject*> children = root->getChildren();
		foreach (BaseObject*, it, children)
		{
			this->destroyObjects(*it);
		}
		if (root->getParent() != NULL)
		{
			root->getParent()->removeChild(root);
		}
		Object* object = dynamic_cast<Object*>(root);
		if (object != NULL)
		{
			if (object->isFocused())
			{
				object->setFocused(false);
			}
			this->objects.removeKey(root->getName());
		}
		else
		{
			Animator* animator = dynamic_cast<Animator*>(root);
			if (animator != NULL)
			{
				this->animators.removeKey(root->getName());
			}
		}
		delete root;
	}
	
	void Dataset::_destroyTexture(Texture* texture)
	{
		hstr filename = texture->getFilename();
		if (!this->textures.hasKey(filename))
		{
			throw ObjectNotExistsException("Texture", filename, this->name);
		}
		this->textures.removeKey(filename);
		delete texture;
	}
	
	void Dataset::_destroyImage(BaseImage* image)
	{
		hstr name = image->getName();
		if (!this->images.hasKey(name))
		{
			throw ObjectNotExistsException("Image", name, this->name);
		}
		this->images.removeKey(name);
		delete image;
	}
	
	void Dataset::_destroyTexture(chstr name)
	{
		if (!this->textures.hasKey(name))
		{
			throw ObjectNotExistsException("Texture", name, this->name);
		}
		delete this->textures[name];
		this->textures.removeKey(name);
	}
	
	void Dataset::_destroyImage(chstr name)
	{
		if (!this->images.hasKey(name))
		{
			throw ObjectNotExistsException("Image", name, this->name);
		}
		delete this->images[name];
		this->images.removeKey(name);
	}

	hstr Dataset::_makeLocalizedTextureName(chstr filename)
	{
		hstr localization = aprilui::getLocalization();
		if (localization != "")
		{
			hstr locpath = hrdir::joinPath(hrdir::joinPath(hrdir::baseDir(filename), localization), hrdir::baseName(filename));
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
		hstr filepath = hrdir::normalize(hrdir::joinPath(this->filePath, filename, false));
		hstr textureName = hrdir::baseName(filename);
		if (this->textures.hasKey(textureName))
		{
			throw ObjectExistsException("Texture", textureName, this->name);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		bool managed = node->pbool("managed", aprilui::isDefaultManagedTextures());
		april::Texture::LoadMode loadMode = aprilui::getDefaultTextureLoadMode();
		// DEPRECATED
		if (node->pexists("dynamic_load"))
		{
			hlog::warn(aprilui::logTag, "'dynamic_load' is deprecated. Use 'managed' and 'load_mode' instead.");
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
			if (mode == "immediate")
			{
				hlog::warn(aprilui::logTag, "'load_mode=\"immediate\"' is deprecated. Defaulting to 'load_mode=\"async\"'."); // DEPRECATED
				loadMode = april::Texture::LOAD_ASYNC;
			}
			else if (mode == "on_demand")		loadMode = april::Texture::LOAD_ON_DEMAND;
			else if (mode == "async")			loadMode = april::Texture::LOAD_ASYNC;
			else if (mode == "async_on_demand")	loadMode = april::Texture::LOAD_ASYNC_ON_DEMAND;
			else throw Exception("load_mode '" + mode + "' not supported");
		}
		hstr locpath = this->_makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->createTextureFromResource(locpath, april::Texture::TYPE_IMMUTABLE, loadMode);
		if (aprilTexture == NULL)
		{
			throw FileCouldNotOpenException(locpath);
		}
		Texture* texture = new Texture(filepath, aprilTexture, managed);
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if		(filter == "linear")	texture->setFilter(april::Texture::FILTER_LINEAR);
			else if	(filter == "nearest")	texture->setFilter(april::Texture::FILTER_NEAREST);
			else throw Exception("texture filter '" + filter + "' not supported");
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
			if (this->images.hasKey(textureName))
			{
				throw ObjectExistsException("Texture", filename, this->name);
			}
			BaseImage* image = new Image(texture, filename, grect(0.0f, 0.0f, (float)texture->getWidth(), (float)texture->getHeight()));
			this->images[textureName] = image;
			image->dataset = this;
		}
		else
		{
			BaseImage* image = NULL;
			hstr name;
			grect rect;
			foreach_xmlnode (child, node)
			{
				if (*child == "Image" && (child->pexists("tile") || child->pexists("tile_w") || child->pexists("tile_h"))) // DEPRECATED (this entire block)
				{
					hlog::warn(aprilui::logTag, "Using 'tile', 'tile_w' and 'tile_h' in an 'Image' is deprecated. Use 'TileImage' instead.");
					name = (prefixImages ? textureName + "/" + child->pstr("name") : child->pstr("name"));
					if (this->images.hasKey(name))
					{
						throw ObjectExistsException("Image", name, this->name);
					}
					aprilui::readRectNode(rect, child);
					gvec2 tile;
					if (child->pexists("tile"))
					{
						tile = april::hstrToGvec2(child->pstr("tile"));
					}
					else
					{
						tile.set(child->pfloat("tile_w", 0.0f), child->pfloat("tile_h", 0.0f));
					}
					TileImage* tileImage = new TileImage(texture, name, rect);
					image = tileImage;
					if (tile.x < 0.0f)
					{
						tileImage->setTileW(-tile.x);
					}
					else if (tile.x > 0.0f)
					{
						tileImage->setTileW(tile.x);
						tileImage->setUseTileCount(true);
					}
					if (tile.y < 0.0f)
					{
						tileImage->setTileH(-tile.y);
					}
					else if (tile.y > 0.0f)
					{
						tileImage->setTileH(tile.y);
						tileImage->setUseTileCount(true);
					}
					if (hsgn(tile.x) != hsgn(tile.y))
					{
						hlog::warn(aprilui::logTag, "'tile_w' and 'tile_h' have to be either both positive or negative!");
					}
					this->images[name] = image;
					image->dataset = this;
					foreach_xmlproperty (prop, child)
					{
						name = prop->name();
						if (name == "name" || name == "rect" || name == "position" || name == "size" || name == "x" || name == "y" || name == "w" || name == "h" || name == "tile" || name == "tile_w" || name == "tile_h")
						{
							continue; // TODO - should be done better, maybe reading parameters from a list, then removing them so they aren't set more than once
						}
						image->setProperty(name, prop->value());
					}
				}
				else
				{
					name = (prefixImages ? textureName + "/" + child->pstr("name") : child->pstr("name"));
					if (this->images.hasKey(name))
					{
						throw ObjectExistsException("Image", name, this->name);
					}
					aprilui::readRectNode(rect, child);
					if (*child == "Image")
					{
						image = new Image(texture, name, rect);
					}
					else if (*child == "TileImage")
					{
						image = new TileImage(texture, name, rect);
					}
					else if (*child == "SkinImage")
					{
						image = new SkinImage(texture, name, rect);
					}
					else
					{
						throw XMLUnknownClassException(child->getValue(), child);
					}
					this->images[name] = image;
					image->dataset = this;
					foreach_xmlproperty (prop, child)
					{
						name = prop->name();
						if (name == "name" || name == "rect" || name == "position" || name == "size" || name == "x" || name == "y" || name == "w" || name == "h")
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
		if (this->images.hasKey(name))
		{
			throw ObjectExistsException("CompositeImage", name, this->name);
		}
		gvec2 size;
		if (node->pexists("size"))
		{
			size = april::hstrToGvec2(node->pstr("size"));
		}
		else
		{
			size.set(node->pfloat("w"), node->pfloat("h"));
		}
		CompositeImage* image = new CompositeImage(name, size);
		grect rect;
		foreach_xmlnode (child, node)
		{
			if (*child == "ImageRef")
			{
				refname = child->pstr("name");
				aprilui::readRectNode(rect, child);
				image->addImageRef(this->getImage(refname), rect);
			}
			else
			{
				hlog::warnf(aprilui::logTag, "Unknown node name '%s' in CompositeImage '%s'.", child->getValue().cStr(), name.cStr());
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
				offset = april::hstrToGvec2(node->pstr("position"));
			}
			else
			{
				offset.set(node->pfloat("x", 0.0f), node->pfloat("y", 0.0f));
			}
			this->parseObjectInclude(hrdir::joinPath(this->filePath, node->pstr("path"), false), parent,
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
				aprilui::readRectNode(rect, node);
				rect += offset;
			}
		}
		else
		{
			return NULL;
		}
		if (this->objects.hasKey(objectName))
		{
			throw ObjectExistsException("Object", objectName, this->name);
		}
		BaseObject* baseObject = NULL;
		Object* object = NULL;
		Animator* animator = NULL;
		if (*node == "Object")
		{
			baseObject = object = aprilui::createObject(className, objectName);
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
			throw XMLUnknownClassException(className, node);
		}
		if (object != NULL)
		{
			object->setRect(rect);
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
			if (name == "name" || name == "rect" || name == "position" || name == "size" || name == "x" || name == "y" || name == "w" || name == "h")
			{
				continue; // TODO - might be done better, maybe reading parameters from a list, then removing them so they aren't set more than once
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
		hstr extension = hrdir::baseName(path).replaced("*", "");
		harray<hstr> contents = hrdir::files(this->filePath, true).sorted();
		foreach (hstr, it, contents)
		{
			if ((*it).endsWith(extension))
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
		hstr baseDir = hrdir::baseDir(path);
		hstr filename = path(baseDir.size() + 1, -1);
		hstr left;
		hstr right;
		filename.split("*", left, right);
		harray<hstr> contents = hrdir::files(baseDir).sorted();
		foreach (hstr, it, contents)
		{
			if ((*it).startsWith(left) && (*it).endsWith(right))
			{
				this->parseObjectIncludeFile(hrdir::joinPath(baseDir, (*it), false), parent, "", "", gvec2());
			}
		}
	}
	
	hlxml::Document* Dataset::_openDocument(chstr filename)
	{
		hlxml::Document* document = this->includeDocuments.tryGet(filename, NULL);
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
			else if	(*node == "Include")		parseGlobalInclude(hrdir::joinPath(hrdir::baseDir(path), node->pstr("path"), false));
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
			catch (hexception& e)
			{
				hlog::error(aprilui::logTag, e.getMessage());
				this->_closeDocuments(); // safe not to throw an exception
				throw e;
			}
		}
		this->loaded = true;
		this->update(0.0f);
	}

	hstr Dataset::_makeTextsPath()
	{
		hstr filepathPrefix = hrdir::joinPath(this->filePath, (this->textsPath != "" ? this->textsPath : aprilui::getDefaultTextsPath()), false);
		hstr filepath = hrdir::normalize(hrdir::joinPath(filepathPrefix, aprilui::getLocalization(), false));
		if (!hrdir::exists(filepath))
		{
			filepath = hrdir::normalize(hrdir::joinPath(filepathPrefix, aprilui::getDefaultLocalization(), false));
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
			lines = f.readLines();
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
				while (i < firstLine.size() && !hbetweenII((int)firstLine[i], 0, 127))
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
						key = (*it2).split("#").first().trimmed(' ');
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
			throw Exception("Unable to unload dataset '" + this->getName() + "', data not loaded!");
		}
		foreach_m (Animator*, it, this->animators)
		{
			if (it->second->getParent() != NULL)
			{
				it->second->getParent()->removeChild(it->second);
			}
			delete it->second;
		}
		this->animators.clear();
		foreach_m (Object*, it, this->objects)
		{
			if (it->second->getParent() != NULL)
			{
				it->second->getParent()->removeChild(it->second);
			}
			it->second->removeChildren(false);
			delete it->second;
		}
		this->objects.clear();
		foreach_m (BaseImage*, it, this->images)
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
			if (this->objects.hasKey(name) || this->animators.hasKey(name))
			{
				throw ObjectExistsException("Object", name, this->name);
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
		bool hasObject = this->objects.hasKey(root->getName());
		bool hasAnimator = this->animators.hasKey(root->getName());
		if (!hasObject && !hasAnimator)
		{
			// this object could be from another dataset, so check that first.
			Dataset* dataset = root->getDataset();
			if (dataset != this)
			{
				hlog::writef(logTag, "Dataset '%s' unregistering object from another dataset: '%s'", this->getName().cStr(), root->getFullName().cStr());
				dataset->unregisterObjects(root);
				return;
			}
			throw ObjectNotExistsException("Object", root->getName(), this->name);
		}
		harray<BaseObject*> children = root->getChildren();
		foreach (BaseObject*, it, children)
		{
			this->unregisterObjects(*it);
		}
		if (hasObject)
		{
			Object* focusedRoot = dynamic_cast<Object*>(root);
			if (focusedRoot != NULL && focusedRoot->isFocused())
			{
				focusedRoot->setFocused(false);
			}
			this->objects.removeKey(root->getName());
		}
		else if (hasAnimator)
		{
			this->animators.removeKey(root->getName());
		}
		root->dataset = NULL;
	}
	
	void Dataset::registerImage(BaseImage* image)
	{
		hstr name = image->getName();
		if (this->images.hasKey(name))
		{
			throw ObjectExistsException("Image", name, this->name);
		}
		this->images[name] = image;
		image->dataset = this;
	}
	
	void Dataset::unregisterImage(BaseImage* image)
	{
		hstr name = image->getName();
		if (!this->images.hasKey(name))
		{
			throw ObjectNotExistsException("Image", name, this->name);
		}
		this->images.removeKey(name);
		image->dataset = NULL;
	}
	
	void Dataset::registerTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (this->textures.hasKey(name))
		{
			throw ObjectExistsException("Texture", name, this->name);
		}
		this->textures[name] = texture;
	}

	void Dataset::unregisterTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (!this->textures.hasKey(name))
		{
			throw ObjectNotExistsException("Texture", name, this->name);
		}
		this->textures.removeKey(name);
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
		int dot = name.indexOf('.');
		if (dot < 0)
		{
			if (!this->objects.hasKey(name))
			{
				throw ObjectNotExistsException("Object", name, this->name);
			}
			return this->objects[name];
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (hexception&)
		{
			throw ObjectNotExistsException("Object", name, this->name);
		}
		return dataset->getObject(name(dot + 1, -1));
	}

	Animator* Dataset::getAnimator(chstr name)
	{
		int dot = name.indexOf('.');
		if (dot < 0)
		{
			if (!this->animators.hasKey(name))
			{
				throw ObjectNotExistsException("Animator", name, this->name);
			}
			return this->animators[name];
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (hexception&)
		{
			throw ObjectNotExistsException("Animator", name, this->name);
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
		return this->images.hasKey(name);
	}
	
	bool Dataset::hasTexture(chstr name)
	{
		return this->textures.hasKey(name);
	}
	
	Object* Dataset::tryGetObject(chstr name)
	{
		int dot = name.indexOf('.');
		if (dot < 0)
		{
			return this->objects.tryGet(name, NULL);
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (hexception&)
		{
			return NULL;
		}
		return dataset->tryGetObject(name(dot + 1, -1));
	}

	Animator* Dataset::tryGetAnimator(chstr name)
	{
		int dot = name.indexOf('.');
		if (dot < 0)
		{
			return this->animators.tryGet(name, NULL);
		}
		Dataset* dataset = NULL;
		try
		{
			dataset = aprilui::getDatasetByName(name(0, dot));
		}
		catch (hexception&)
		{
			return NULL;
		}
		return dataset->tryGetAnimator(name(dot + 1, -1));
	}

	Texture* Dataset::getTexture(chstr name)
	{
		if (!this->textures.hasKey(name))
		{
			throw ObjectNotExistsException("Texture", name, this->name);
		}
		return this->textures[name];
	}
	
	BaseImage* Dataset::getImage(chstr name)
	{
		BaseImage* image = NULL;
		if (name == APRILUI_IMAGE_NAME_NULL)
		{
			return this->nullImage;
		}
		if (this->images.hasKey(name))
		{
			image = this->images[name];
		}
		if (image == NULL)
		{
			int dot = name.indexOf('.');
			if (dot < 0)
			{
				throw ObjectNotExistsException("Image", name, this->name);
			}
			Dataset* dataset;
			try
			{
				dataset = aprilui::getDatasetByName(name(0, dot));
			}
			catch (hexception&)
			{
				throw ObjectNotExistsException("Image", name, this->name);
			}
			image = dataset->getImage(name(dot + 1, -1));
		}
		return image;
	}

	bool Dataset::_findTextEntry(chstr textKey, hstr* text)
	{
		int dot = textKey.indexOf('.');
		if (dot < 0)
		{
			if (!this->texts.hasKey(textKey))
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
		if (this->callbacks.hasKey(name))
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
		ustr uChars = key.uStr();
		if (uChars.size() == 0 || uChars[0] != '{')
		{
			if ((int)uChars.find_first_of('{') >= 0 || (int)uChars.find_first_of('}') >= 0)
			{
				hstr text = hsprintf("Malformed formatted text key '%s'!", key.cStr());
				hlog::error(aprilui::logTag, text);
				return text;
			}
			if (!this->hasTextEntry(key))
			{
				hlog::warnf(aprilui::logTag, "Text key '%s' does not exist!", key.cStr());
			}
			return this->getTextEntry(key);
		}
		int index = (int)uChars.find_first_of('}');
		if (index < 0)
		{
			hlog::errorf(aprilui::logTag, "Could not parse formatted key '%s'.", key.cStr());
			return key;
		}
		harray<ustr> uArgs;
		ustr uFormat = uChars.substr(1, index - 1);
		ustr uArgString = uChars.substr(index + 1, uChars.size() - index - 1);
		// trimming
		if (uArgString.size() > 0)
		{
			const unsigned int* ucstr = uArgString.c_str();
			while (ucstr[0] == ' ')
			{
				++ucstr;
			}
			uArgString = ucstr;
			// r-trimming
			if (uArgString.size() > 0)
			{
				ucstr = uArgString.c_str();
				int i = (int)uArgString.size() - 1;
				while (i >= 0 && ucstr[i] == ' ')
				{
					--i;
				}
				uArgString = uArgString.substr(0, i + 1);
			}
		}
		// trimming finished
		if (!this->_processCompositeTextKeyArgs(uArgString, uArgs))
		{
			hstr format = hstr::fromUnicode(harray<unsigned int>(uFormat.c_str(), (int)uFormat.size()));
			hstr argString = hstr::fromUnicode(harray<unsigned int>(uArgString.c_str(), (int)uArgString.size()));
			hlog::writef(aprilui::logTag, "- while processing args: '%s' with args '%s'.", format.cStr(), argString.cStr());
			return key;
		}
		ustr uPreprocessedFormat;
		harray<ustr> uPreprocessedArgs;
		if (!this->_preprocessCompositeTextKeyFormat(uFormat, uArgs, uPreprocessedFormat, uPreprocessedArgs))
		{
			hstr format = hstr::fromUnicode(harray<unsigned int>(uFormat.c_str(), (int)uFormat.size()));
			hstr argString = hstr::fromUnicode(harray<unsigned int>(uArgString.c_str(), (int)uArgString.size()));
			hlog::writef(aprilui::logTag, "- while preprocessing format: '%s' with args '%s'.", format.cStr(), argString.cStr());
			return key;
		}
		hstr result;
		if (!this->_processCompositeTextKeyFormat(uPreprocessedFormat, uPreprocessedArgs, result))
		{
			hstr format = hstr::fromUnicode(harray<unsigned int>(uFormat.c_str(), (int)uFormat.size()));
			hstr argString = hstr::fromUnicode(harray<unsigned int>(uArgString.c_str(), (int)uArgString.size()));
			hlog::writef(aprilui::logTag, "- while processing format: '%s' with args '%s'.", format.cStr(), argString.cStr());
			return key;
		}
		return result;
	}

	bool Dataset::_processCompositeTextKeyArgs(ustr uArgString, harray<ustr>& uArgs)
	{
		uArgs.clear();
		// splittings args
		int openIndex;
		int closeIndex;
		while (uArgString.size() > 0)
		{
			openIndex = (int)uArgString.find_first_of('{');
			closeIndex = (int)uArgString.find_first_of('}');
			if (openIndex < 0 && closeIndex < 0)
			{
				uArgs += this->_getArgEntries(uArgString);
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
			uArgs += this->_getArgEntries(uArgString.substr(0, openIndex));
			// getting args inside of {}
			uArgs += uArgString.substr(openIndex + 1, closeIndex - openIndex - 1);
			// rest of the args
			uArgString = uArgString.substr(closeIndex + 1, uArgString.size() - closeIndex - 1);
		}
		return true;
	}

	bool Dataset::_preprocessCompositeTextKeyFormat(ustr uFormat, harray<ustr> uArgs, ustr& uPreprocessedFormat, harray<ustr>& uPreprocessedArgs)
	{
		uPreprocessedFormat.clear();
		uPreprocessedArgs.clear();
		// preprocessing of format string and args
		int index;
		ustr uArg;
		harray<int> indexes;
		while (uFormat.size() > 0)
		{
			index = (int)uFormat.find_first_of('%');
			if (index < 0)
			{
				uPreprocessedFormat += uFormat;
				break;
			}
			if (index >= (int)uFormat.size() - 1)
			{
				hlog::error(aprilui::logTag, "Last character is '%'!");
				return false;
			}
			if (uFormat[index + 1] == '%') // escaped "%", continue processing
			{
				uPreprocessedFormat += uFormat.substr(0, index + 2);
				uFormat = uFormat.substr(index + 2, uFormat.size() - index - 2);
				continue;
			}
			if (uFormat[index + 1] == 's') // %s, not processing that now
			{
				if (uArgs.size() == 0)
				{
					hlog::error(aprilui::logTag, "Not enough args!");
					return false;
				}
				uPreprocessedFormat += uFormat.substr(0, index + 2);
				uFormat = uFormat.substr(index + 2, uFormat.size() - index - 2);
				uPreprocessedArgs += uArgs.remove_first();
				continue;
			}
			if (uFormat[index + 1] == 'f')
			{
				if (uArgs.size() == 0)
				{
					hlog::error(aprilui::logTag, "Not enough args!");
					return false;
				}
				uArg = uArgs.remove_first();
				uPreprocessedFormat += uFormat.substr(0, index) + uArg;
				uFormat = uFormat.substr(index + 2, uFormat.size() - index - 2);
				if (!this->_getCompositeTextKeyFormatIndexes(uArg, indexes))
				{
					return false;
				}
				if (indexes.size() > uArgs.size())
				{
					hlog::error(aprilui::logTag, "Not enough args!");
					return false;
				}
				uPreprocessedArgs += uArgs.remove_first(indexes.size());
			}
		}
		uPreprocessedArgs += uArgs; // remaining args
		return true;
	}

	bool Dataset::_processCompositeTextKeyFormat(ustr uFormat, harray<ustr> uArgs, hstr& result)
	{
		result = "";
		ustr uResult;
		// preprocessing of format string and args
		harray<int> indexes;
		if (!this->_getCompositeTextKeyFormatIndexes(uFormat, indexes))
		{
			return false;
		}
		if (uArgs.size() < indexes.size())
		{
			hlog::error(aprilui::logTag, "Not enough args!");
			return false;
		}
		if (indexes.size() > uArgs.size())
		{
			hlog::error(aprilui::logTag, "Too many args!");
			return false;
		}
		foreach (int, it, indexes)
		{
			uResult += uFormat.substr(0, (*it));
			uResult += uArgs.remove_first();
			uFormat = uFormat.substr((*it) + 2, uFormat.size() - (*it) - 2);
		}
		uResult += uFormat;
		int index = (int)uResult.find_first_of('%');
		while (index >= 0 && index < (int)uResult.size() - 1)
		{
			if (uResult[index + 1] == '%')
			{
				uResult.erase(index + 1, 1);
			}
			index = (int)uResult.find_first_of('%', index + 1);
		}
		result = hstr::fromUnicode(uResult.c_str());
		return true;
	}

	bool Dataset::_getCompositeTextKeyFormatIndexes(ustr uFormat, harray<int>& indexes)
	{
		indexes.clear();
		// finding formatting indexes
		int index;
		int currentIndex = 0;
		while (uFormat.size() > 0)
		{
			index = (int)uFormat.find_first_of('%');
			if (index < 0)
			{
				break;
			}
			if (index >= (int)uFormat.size() - 1)
			{
				hlog::error(aprilui::logTag, "Last character is '%'!");
				return false;
			}
			if (uFormat[index + 1] == '%') // escaped "%", use just one "%".
			{
				uFormat = uFormat.substr(index + 2, uFormat.size() - index - 2);
				currentIndex += index + 2;
				continue;
			}
			if (uFormat[index + 1] != 's')
			{
				hlog::errorf(aprilui::logTag, "Unsupported formatting '%%%c'!", uFormat[index + 1]);
				return false;
			}
			indexes += currentIndex + index;
			uFormat = uFormat.substr(index + 2, uFormat.size() - index - 2);
			currentIndex = 0;
		}
		return true;
	}


	harray<Dataset::ustr> Dataset::_getArgEntries(ustr uString)
	{
		harray<hstr> keys;
		int index;
		while (true)
		{
			index = (int)uString.find_first_of(' ');
			if (index < 0)
			{
				break;
			}
			keys += hstr::fromUnicode(uString.substr(0, index).c_str());
			uString = uString.substr(index + 1);
		}
		keys += hstr::fromUnicode(uString.c_str());
		keys.remove_all("");
		harray<ustr> result;
		foreach (hstr, it, keys)
		{
			result += this->getTextEntry(*it).uStr();
		}
		return result;
	}

	void Dataset::_throwInvalidObjectTypeCast(chstr typeName, chstr objName, chstr datasetName)
	{
		throw InvalidObjectTypeCast(typeName, objName, datasetName);
	}
}
