/// @file
/// @version 4.1
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

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "BaseImage.h"
#include "Dataset.h"
#include "Exception.h"
#include "Images.h"
#include "Objects.h"
#include "Style.h"
#include "Texture.h"

namespace aprilui
{
	static harray<hstr> _ignoredStandardProperties = hstr("name,rect,position,size,x,y,w,h").split(',');
	static harray<hstr> _ignoredStyleProperties = _ignoredStandardProperties + "style";

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
		if (this->name == "")
		{
			this->name = hrdir::baseName(hresource::withoutExtension(this->filename));
		}
		this->loaded = false;
		aprilui::_registerDataset(this->name, this);
	}
	
	Dataset::~Dataset()
	{
		aprilui::_unregisterDataset(this->name, this);
		if (this->isLoaded())
		{
			if (this->focusedObject != NULL && dynamic_cast<EditBox*>(this->focusedObject) != NULL)
			{
				april::window->hideVirtualKeyboard();
			}
			this->unload();
		}
	}

	bool Dataset::isLoaded() const
	{
		return (this->loaded || this->objects.size() > 0 || this->animators.size() > 0 || this->textures.size() > 0 ||
			this->images.size() > 0 || this->styles.size() > 0 || this->texts.size() > 0);
	}

	hmap<hstr, BaseObject*> Dataset::getAllObjects() const
	{
		hmap<hstr, BaseObject*> result = this->animators.cast<hstr, BaseObject*>();
		result.inject(this->objects.cast<hstr, BaseObject*>());
		return result;
	}

	int Dataset::getFocusedObjectIndex() const
	{
		return (this->focusedObject != NULL && this->focusedObject->isEnabled() &&
			this->focusedObject->isVisible() ? this->focusedObject->getFocusIndex() : -1);
	}

	bool Dataset::isAnimated() const
	{
		HL_LAMBDA_CLASS(_isAnimatedObject, bool, ((hstr const& name, Object* const& object) { return object->isAnimated(); }));
		HL_LAMBDA_CLASS(_isAnimatedAnimator, bool, ((hstr const& name, Animator* const& animator) { return animator->isAnimated(); }));
		return (this->objects.matchesAny(&_isAnimatedObject::lambda) || this->animators.matchesAny(&_isAnimatedAnimator::lambda));
	}
	
	bool Dataset::isWaitingAnimation() const
	{
		HL_LAMBDA_CLASS(_isWaitingAnimationObject, bool, ((hstr const& name, Object* const& object) { return object->isWaitingAnimation(); }));
		HL_LAMBDA_CLASS(_isWaitingAnimationAnimator, bool, ((hstr const& name, Animator* const& animator) { return animator->isWaitingAnimation(); }));
		return (this->objects.matchesAny(&_isWaitingAnimationObject::lambda) || this->animators.matchesAny(&_isWaitingAnimationAnimator::lambda));
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
		hlog::warnf(logTag, "Object with Focus Index '%d' in '%s' does not exist, is not visible or is not enabled!", value, this->name.cStr());
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
			__THROW_EXCEPTION(ObjectNotExistsException("Object", root->getName(), this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		harray<BaseObject*> children = root->getChildren();
		foreach (BaseObject*, it, children)
		{
			this->destroyObjects(*it);
		}
		aprilui::Object* rootParent = root->getParent();
		if (rootParent != NULL)
		{
			rootParent->removeChild(root);
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
		if (this->root == root)
		{
			this->root = NULL;
		}
		delete root;
	}
	
	void Dataset::_destroyTexture(chstr name)
	{
		if (!this->textures.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Texture", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		delete this->textures[name];
		this->textures.removeKey(name);
	}

	void Dataset::_destroyImage(chstr name)
	{
		if (!this->images.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		delete this->images[name];
		this->images.removeKey(name);
	}

	void Dataset::_destroyStyle(chstr name)
	{
		if (!this->styles.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Style", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		delete this->styles[name];
		this->styles.removeKey(name);
	}

	void Dataset::_destroyTexture(Texture* texture)
	{
		hstr filename = texture->getFilename();
		if (!this->textures.hasKey(filename))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Texture", filename, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->textures.removeKey(filename);
		delete texture;
	}
	
	void Dataset::_destroyImage(BaseImage* image)
	{
		hstr name = image->getName();
		if (!this->images.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->images.removeKey(name);
		delete image;
	}

	void Dataset::_destroyStyle(Style* style)
	{
		hstr name = style->getName();
		if (!this->styles.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Style", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->styles.removeKey(name);
		delete style;
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
			__THROW_EXCEPTION(ObjectExistsException("Texture", textureName, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		bool managed = node->pbool("managed", aprilui::isDefaultManagedTextures());
		april::Texture::LoadMode loadMode = aprilui::getDefaultTextureLoadMode();
		// DEPRECATED
		if (node->pexists("dynamic_load"))
		{
			hlog::warn(logTag, "'dynamic_load' is deprecated. Use 'managed' and 'load_mode' instead.");
			bool dynamicLoad = node->pbool("dynamic_load");
			if (dynamicLoad)
			{
				managed = true;
				loadMode = april::Texture::LoadMode::OnDemand;
			}
			else
			{
				managed = false;
				loadMode = april::Texture::LoadMode::Async;
			}
		}
		if (node->pexists("load_mode"))
		{
			hstr mode = node->pstr("load_mode");
			if (mode == "immediate")
			{
				hlog::warn(logTag, "'load_mode=\"immediate\"' is deprecated. Defaulting to 'load_mode=\"async\"'."); // DEPRECATED
				loadMode = april::Texture::LoadMode::Async;
			}
			else if (mode == "on_demand")				loadMode = april::Texture::LoadMode::OnDemand;
			else if (mode == "async")					loadMode = april::Texture::LoadMode::Async;
			else if (mode == "async_deferred_upload")	loadMode = april::Texture::LoadMode::AsyncDeferredUpload;
			else if (mode == "async_on_demand")
			{
				hlog::warn(logTag, "'load_mode=\"async_on_demand\"' is deprecated. Use 'load_mode=\"async_deferred_upload\"' instead."); // DEPRECATED
				loadMode = april::Texture::LoadMode::AsyncDeferredUpload;
			}
			else
			{
				__THROW_EXCEPTION(Exception("Load Mode '" + mode + "' is not supported!"), aprilui::systemConsistencyDebugExceptionsEnabled, return);
			}
		}
		hstr locpath = this->_makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->createTextureFromResource(locpath, april::Texture::Type::Immutable, loadMode);
		if (aprilTexture == NULL)
		{
			__THROW_EXCEPTION(FileCouldNotOpenException(locpath), aprilui::textureFilesDebugExceptionsEnabled, return);
		}
		Texture* texture = new Texture(filename, filepath, aprilTexture, managed);
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if		(filter == "linear")	texture->setFilter(april::Texture::Filter::Linear);
			else if	(filter == "nearest")	texture->setFilter(april::Texture::Filter::Nearest);
			else
			{
				__THROW_EXCEPTION(Exception("Texture Filter '" + filter + "' is not supported!"), aprilui::systemConsistencyDebugExceptionsEnabled, return);
			}
		}
		if (node->pbool("wrap", false))
		{
			texture->setAddressMode(april::Texture::AddressMode::Wrap);
		}
		else
		{
			texture->setAddressMode(april::Texture::AddressMode::Clamp);
		}
		this->textures[textureName] = texture;
		texture->dataset = this;
		// extract image definitions
		if (node->children.size() == 0) // if there are no images defined, create one that fills the whole area
		{
			if (this->images.hasKey(textureName))
			{
				__THROW_EXCEPTION(ObjectExistsException("Image", textureName, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
			}
			aprilTexture->loadMetaData();
			BaseImage* image = new Image(texture, filename, grect(0.0f, 0.0f, (float)texture->getWidth(), (float)texture->getHeight()));
			this->images[textureName] = image;
			image->dataset = this;
		}
		else
		{
			BaseImage* image = NULL;
			hstr name;
			grect rect;
			hmap<hstr, hstr> childProperties;
			foreach_xmlnode (child, node)
			{
				childProperties = (*child)->properties;
				if ((*child)->name == "Image" && (childProperties.hasKey("tile") || childProperties.hasKey("tile_w") || childProperties.hasKey("tile_h"))) // DEPRECATED (this entire block)
				{
					hlog::warn(logTag, "Using 'tile', 'tile_w' and 'tile_h' in an 'Image' is deprecated. Use 'TileImage' instead.");
					name = childProperties["name"];
					if (prefixImages)
					{
						name = textureName + "/" + name;
					}
					if (this->images.hasKey(name))
					{
						__THROW_EXCEPTION(ObjectExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, continue);
					}
					aprilui::_readRectNode(rect, (*child));
					gvec2 tile;
					if (childProperties.hasKey("tile"))
					{
						tile = april::hstrToGvec2(childProperties["tile"]);
					}
					else
					{
						tile.set((*child)->pfloat("tile_w", 0.0f), (*child)->pfloat("tile_h", 0.0f));
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
						hlog::warn(logTag, "'tile_w' and 'tile_h' have to be either both positive or negative!");
					}
					this->images[name] = image;
					image->dataset = this;
					childProperties.removeKeys(((_ignoredStandardProperties + "tile") + "tile_w") + "tile_h");
					foreach_m (hstr, it, childProperties)
					{
						image->setProperty(it->first, it->second);
					}
				}
				else
				{
					name = childProperties["name"];
					if (prefixImages)
					{
						name = textureName + "/" + name;
					}
					if (this->images.hasKey(name))
					{
						__THROW_EXCEPTION(ObjectExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, continue);
					}
					aprilui::_readRectNode(rect, (*child));
					if ((*child)->name == "Image")
					{
						image = new Image(texture, name, rect);
					}
					else if ((*child)->name == "TileImage")
					{
						image = new TileImage(texture, name, rect);
					}
					else if ((*child)->name == "SkinImage")
					{
						image = new SkinImage(texture, name, rect);
					}
					else
					{
						__THROW_EXCEPTION(XMLUnknownClassException((*child)->name, (*child)), aprilui::systemConsistencyDebugExceptionsEnabled, continue);
					}
					this->images[name] = image;
					image->dataset = this;
					childProperties.removeKeys(_ignoredStandardProperties);
					foreach_m (hstr, it, childProperties)
					{
						image->setProperty(it->first, it->second);
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
			__THROW_EXCEPTION(ObjectExistsException("CompositeImage", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
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
			if ((*child)->name == "ImageRef")
			{
				refname = (*child)->pstr("name");
				aprilui::_readRectNode(rect, (*child));
				image->addImageRef(this->getImage(refname), rect);
			}
			else
			{
				hlog::warnf(logTag, "Unknown node name '%s' in CompositeImage '%s'.", (*child)->name.cStr(), name.cStr());
			}
		}
		this->images[name] = image;
		image->dataset = this;
	}
	
	void Dataset::parseStyle(hlxml::Node* node)
	{
		hstr styleName = node->pstr("name");
		if (this->styles.hasKey(styleName))
		{
			__THROW_EXCEPTION(ObjectExistsException("Style", styleName, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		Style* style = NULL;
		if (node->pexists("base"))
		{
			hstr baseStyleName = node->pstr("base");
			try
			{
				style = this->getStyle(baseStyleName);
				if (style != NULL)
				{
					style = style->clone();
					style->name = styleName;
				}
				else
				{
					hlog::error(logTag, "Cannot find Base-Style '" + baseStyleName + "'!");
				}
			}
			catch (hexception& e)
			{
				hlog::error(logTag, "Cannot find Base-Style '" + baseStyleName + "':" + e.getMessage());
			}
		}
		if (style == NULL)
		{
			style = new Style(styleName);
		}
		this->styles[styleName] = style;
		style->dataset = this;
		const hmap<hstr, Object* (*)(chstr)>& objectFactories = aprilui::getObjectFactories();
		const hmap<hstr, Animator* (*)(chstr)>& animatorFactories = aprilui::getAnimatorFactories();
		hstr className;
		hstr name;
		hmap<hstr, hstr> properties;
		bool isObject = false;
		bool isAnimator = false;
		foreach_xmlnode (child, node)
		{
			isObject = false;
			isAnimator = false;
			className = (*child)->name;
			if (className == "Object")
			{
				isObject = true;
				className = (*child)->pstr("type", "");
			}
			else if (className == "Animator")
			{
				isAnimator = true;
				className = (*child)->pstr("type", "");
			}
			else
			{
				isObject = objectFactories.hasKey(className);
				if (!isObject)
				{
					isAnimator = animatorFactories.hasKey(className);
				}
			}
			if (isObject || isAnimator)
			{
				properties = (*child)->properties;
				if (properties.removeKeys(_ignoredStandardProperties) > 0)
				{
					hlog::error(logTag, "Using properties '" + _ignoredStandardProperties.joined(',') + "' in Style is not allowed!");
				}
				properties.removeKey("type");
				if (className == "")
				{
					if (isObject)
					{
						style->objectDefaults.properties.inject(properties);
					}
					else if (isAnimator)
					{
						style->animatorDefaults.properties.inject(properties);
					}
				}
				else
				{
					if (isObject)
					{
						if (!style->objects.hasKey(className))
						{
							style->objects[className] = Style::Group(properties);
						}
						else
						{
							style->objects[className].properties.inject(properties);
						}
					}
					else if (isAnimator)
					{
						if (!style->animators.hasKey(className))
						{
							style->animators[className] = Style::Group(properties);
						}
						else
						{
							style->animators[className].properties.inject(properties);
						}
					}
				}
			}
			else
			{
				hlog::warn(logTag, "Object/Animator type '" + className + "' not found! It might not be registered in aprilui.");
			}
		}
	}

	BaseObject* Dataset::parseObject(hlxml::Node* node, Object* parent)
	{
		Style style;
		return this->recursiveObjectParse(node, parent, &style, "", "", gvec2());
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
		Style style;
		return this->recursiveObjectParse(node, parent, &style, "", "", gvec2());
	}

	BaseObject* Dataset::recursiveObjectParse(hlxml::Node* node, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		hstr objectName;
		hstr className = node->name;
		if (className == "Include")
		{
			return this->recursiveObjectIncludeParse(node, parent, style, namePrefix, nameSuffix, offset);
		}
		const hmap<hstr, Object* (*)(chstr)>& objectFactories = aprilui::getObjectFactories();
		const hmap<hstr, Animator* (*)(chstr)>& animatorFactories = aprilui::getAnimatorFactories();
		bool isObject = false;
		bool isAnimator = false;
		if (className == "Object")
		{
			isObject = true;
			className = node->pstr("type", "");
		}
		else if (className == "Animator")
		{
			isAnimator = true;
			className = node->pstr("type", "");
		}
		else
		{
			isObject = objectFactories.hasKey(className);
			if (!isObject)
			{
				isAnimator = animatorFactories.hasKey(className);
			}
		}
		grect rect(0.0f, 0.0f, 1.0f, 1.0f);
		if (!isObject && !isAnimator)
		{
			return NULL;
		}
		if (node->pexists("name"))
		{
			objectName = namePrefix + node->pstr("name") + nameSuffix;
		}
		else
		{
			objectName = april::generateName(className);
		}
		if (isObject)
		{
			aprilui::_readRectNode(rect, node);
			rect += offset;
		}
		if (isObject && this->objects.hasKey(objectName))
		{
			__THROW_EXCEPTION(ObjectExistsException("Object", objectName, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
		}
		if (isAnimator && this->animators.hasKey(objectName))
		{
			__THROW_EXCEPTION(ObjectExistsException("Animator", objectName, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
		}
		BaseObject* baseObject = NULL;
		Object* object = NULL;
		Animator* animator = NULL;
		if (isObject)
		{
			baseObject = object = aprilui::createObject(className, objectName);
		}
		else if (isAnimator)
		{
			baseObject = animator = aprilui::createAnimator(className, objectName);
		}
		if (baseObject == NULL)
		{
			isObject = false;
			isAnimator = false;
			baseObject = this->parseExternalObjectClass(node, objectName, rect);
			if (baseObject != NULL)
			{
				object = dynamic_cast<Object*>(baseObject);
				if (object != NULL)
				{
					isObject = true;
				}
				else
				{
					animator = dynamic_cast<Animator*>(baseObject);
					if (animator != NULL)
					{
						isAnimator = true;
					}
				}
			}
		}
		if (baseObject == NULL)
		{
			__THROW_EXCEPTION(XMLUnknownClassException(className, node), aprilui::systemConsistencyDebugExceptionsEnabled, return NULL);
		}
		if (isObject)
		{
			object->setRect(rect);
		}
		baseObject->dataset = this;
		EventArgs args(Event::RegisteredInDataset, this);
		baseObject->notifyEvent(Event::RegisteredInDataset, &args);
		bool isCustomStyle = node->pexists("style");
		if (isCustomStyle)
		{
			hstr styleName = node->pstr("style");
			if (styleName != "")
			{
				Style* newStyle = NULL;
				try
				{
					newStyle = this->getStyle(styleName);
					if (newStyle != NULL)
					{
						style = style->_injected(newStyle);
					}
					else
					{
						hlog::error(logTag, "Cannot find Style '" + styleName + "'!");
						isCustomStyle = false;
					}
				}
				catch (hexception& e)
				{
					hlog::error(logTag, "Cannot find Style '" + styleName + "':" + e.getMessage());
					isCustomStyle = false;
				}
			}
			else
			{
				style = new Style();
			}
		}
		baseObject->applyStyle(style);
		if (isObject)
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
		else if (isAnimator)
		{
			this->animators[objectName] = animator;
			if (parent != NULL)
			{
				parent->addChild(animator);
			}
		}
		hstr name;
		hmap<hstr, hstr> properties = node->properties;
		properties.removeKeys(_ignoredStyleProperties);
		foreach_m (hstr, it, properties)
		{
			baseObject->setProperty(it->first, it->second);
		}
		if (isObject)
		{
			foreach_xmlnode (child, node)
			{
				if ((*child)->type != hlxml::Node::Type::Text && (*child)->type != hlxml::Node::Type::Comment)
				{
					this->recursiveObjectParse((*child), object, style, namePrefix, nameSuffix, gvec2());
				}
			}
		}
		if (isCustomStyle)
		{
			delete style;
		}
		return baseObject;
	}

	BaseObject* Dataset::recursiveObjectIncludeParse(hlxml::Node* node, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		if (node->pexists("position"))
		{
			offset += april::hstrToGvec2(node->pstr("position"));
		}
		else
		{
			offset += gvec2(node->pfloat("x", 0.0f), node->pfloat("y", 0.0f));
		}
		hstr path = hrdir::joinPath(this->filePath, node->pstr("path"), false);
		hstr newNamePrefix = node->pstr("name_prefix", "") + namePrefix;
		hstr newNameSuffix = nameSuffix + node->pstr("name_suffix", "");
		BaseObject* includeRoot = this->parseObjectInclude(path, parent, style, newNamePrefix, newNameSuffix, offset);
		BaseObject* descendant = NULL;
		hstr typeName;
		hstr objectName;
		hstr newName;
		hmap<hstr, hstr> childProperties;
		foreach_xmlnode (child, node)
		{
			if ((*child)->name == "Property")
			{
				if ((*child)->type != hlxml::Node::Type::Text && (*child)->type != hlxml::Node::Type::Comment)
				{
					childProperties = (*child)->properties;
					if (childProperties.hasKey("object"))
					{
						objectName = newNamePrefix + childProperties["object"] + newNameSuffix;
						descendant = (includeRoot->getName() == objectName ? includeRoot : includeRoot->findDescendantByName(objectName));
						if (descendant != NULL)
						{
							typeName = "";
							if (childProperties.hasKey("type"))
							{
								typeName = childProperties["type"];
								childProperties.removeKey("type");
							}
							if (typeName == "" || descendant->getClassName() == typeName)
							{
								childProperties.removeKey("object");
								if (childProperties.hasKey("name"))
								{
									newName = newNamePrefix + childProperties["name"] + newNameSuffix;
									if (!this->hasObject(newName))
									{
										this->unregisterObjects(descendant);
										descendant->setName(newName);
										this->registerObjects(descendant, false);
									}
									else
									{
										hlog::errorf(logTag, "Cannot set name '%s' for object '%s' in '%s', object already exists in '%s'!",
											childProperties["name"].cStr(), objectName.cStr(), path.cStr(), this->name.cStr());
									}
									childProperties.removeKey("name");
								}
								foreach_m (hstr, it, childProperties)
								{
									descendant->setProperty(it->first, it->second);
								}
							}
							else if (typeName != "")
							{
								hlog::errorf(logTag, "Found object '%s' in '%s', but found type '%s' instead of expected type '%s'!",
									objectName.cStr(), path.cStr(), descendant->getClassName().cStr(), typeName.cStr());
							}
						}
						else
						{
							hlog::errorf(logTag, "Could not find object '%s' in '%s'!", objectName.cStr(), path.cStr());
						}
					}
					else
					{
						hlog::errorf(logTag, "No object specified for property in '%s'!", path.cStr());
					}
				}
			}
		}
		return includeRoot;
	}
	
	void Dataset::parseGlobalIncludeFile(chstr filename)
	{
		// parse dataset xml file, error checking first
		hstr originalFilePath = this->filePath;
		this->filePath = this->_makeFilePath(filename);
		this->readFile(filename);
		this->filePath = originalFilePath;
	}
	
	void Dataset::parseGlobalInclude(chstr path, bool optional)
	{
		int nParsed = 0;
		hstr originalFilePath = this->filePath;
		this->filePath = this->_makeFilePath(path);
		if (!path.contains("*"))
		{
			nParsed = 1;
			this->readFile(path);
			this->filePath = originalFilePath;
		}
		else
		{
			if (!optional && !hrdir::exists(this->filePath))
			{
				throw Exception(hsprintf("Failed parsing dataset include dir '%s' (included from '%s'), dir not found.", this->filePath.cStr(), originalFilePath.cStr()));
			}
			hstr extension = hrdir::baseName(path).replaced("*", "");
			harray<hstr> contents = hrdir::files(this->filePath, true).sorted();
			foreach (hstr, it, contents)
			{
				if ((*it).endsWith(extension))
				{
					this->readFile((*it));
					nParsed++;
				}
			}
		}
		this->filePath = originalFilePath;
		hlog::writef(logTag, "Parsed dataset include command: '%s', %d files parsed", path.cStr(), nParsed);
	}
	
	BaseObject* Dataset::parseObjectIncludeFile(chstr filename, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		// parse dataset xml file, error checking first
		hstr path = hrdir::normalize(filename);
		hlog::write(logTag, "Parsing object include file: " + path);
		hlxml::Document* doc = this->_openDocument(path);
		hlxml::Node* current = doc->root();
		BaseObject* root = NULL;

		const hmap<hstr, Object* (*)(chstr)>& objectFactories = aprilui::getObjectFactories();
		const hmap<hstr, Animator* (*)(chstr)>& animatorFactories = aprilui::getAnimatorFactories();
		hstr className;
		foreach_xmlnode (node, current)
		{
			className = (*node)->name;
			if (className == "Object" || className == "Animator" || objectFactories.hasKey(className) || animatorFactories.hasKey(className))
			{
				if (root == NULL)
				{
					root = this->recursiveObjectParse((*node), parent, style, namePrefix, nameSuffix, offset);
				}
				else
				{
					hlog::errorf(logTag, "Detected multiple roots in '%s'. Ignoring other root objects.", path.cStr());
					break;
				}
			}
		}
		return root;
	}
	
	BaseObject* Dataset::parseObjectInclude(chstr path, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset)
	{
		if (!path.contains("*"))
		{
			return this->parseObjectIncludeFile(path, parent, style, namePrefix, nameSuffix, offset);
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
				this->parseObjectIncludeFile(hrdir::joinPath(baseDir, (*it), false), parent, style, "", "", gvec2());
			}
		}
		return NULL; // since multiple files are loaded, no object is returned
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
		hlog::write(logTag, "Parsing dataset file: " + path);
		hlxml::Document doc(path);
		hlxml::Node* current = doc.root();
		if (current == NULL)
		{
			__THROW_EXCEPTION(Exception("Unable to parse Xml file '" + filename + "', no root node found!"), aprilui::systemConsistencyDebugExceptionsEnabled, return);
		}
		this->parseExternalXMLNode(current);
		const hmap<hstr, Object* (*)(chstr)>& objectFactories = aprilui::getObjectFactories();
		foreach_xmlnode (node, current)
		{
			if ((*node)->type != hlxml::Node::Type::Comment)
			{
				if		((*node)->name == "Texture")		this->parseTexture(*node);
				else if ((*node)->name == "CompositeImage")	this->parseCompositeImage(*node);
				else if ((*node)->name == "Style")			this->parseStyle(*node);
				else if ((*node)->name == "Include")		this->parseGlobalInclude(hrdir::joinPath(hrdir::baseDir(path), (*node)->pstr("path"), false), (*node)->pbool("optional", false));
				else if	((*node)->name == "TextureGroup")	this->parseTextureGroup(*node);
				else if ((*node)->name == "Object" || objectFactories.hasKey((*node)->name))
				{
					this->parseObject(*node);
				}
				else
				{
					this->parseExternalXMLNode(*node);
				}
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
				hlog::error(logTag, e.getFullMessage());
				this->_closeDocuments(); // safe not to throw an exception
				throw e;
			}
		}
		this->loaded = true;
		this->update(0.0f);
		this->triggerEvent(aprilui::Event::DatasetLoaded);
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
		hlog::write(logTag, "Loading texts: " + path);
		harray<hstr> files = hrdir::files(path, true);
		hresource file;
		hstream data;
		foreach (hstr, it, files)
		{
			file.open(*it);
			data.writeRaw(file);
			data.rewind();
			file.close();
			if (data.size() > 0)
			{
				this->_loadTextResource(data, this->texts);
				data.clear();
			}
		}
	}
	
	void Dataset::_loadTextResource(hstream& data, hmap<hstr, hstr>& textsMap)
	{
		harray<hstr> values;
		bool keyMode = true;
		hstr key;
		harray<hstr> lines = data.readLines();
		// UTF-8 might have a Byte Order Marker
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
		keyMode = true;
		values.clear();
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
					textsMap[key] = values.joined('\n');
				}
			}
			else
			{
				values += (*it2);
			}
		}
	}

	void Dataset::unload()
	{
		if (!this->isLoaded())
		{
			return;
		}
		this->_closeDocuments();
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
		foreach_m (Style*, it, this->styles)
		{
			delete it->second;
		}
		this->styles.clear();
		this->callbacks.clear();
		this->texts.clear();
		this->root = NULL;
		this->focusedObject = NULL;
		this->loaded = false;
		this->triggerEvent(aprilui::Event::DatasetUnloaded);
	}
	
	void Dataset::registerObjects(BaseObject* root, bool setRootIfNull)
	{
		hstr name;
		harray<BaseObject*> objects;
		objects += root;
		objects += root->getDescendants();
		Object* object = NULL;
		Animator* animator = NULL;
		EventArgs args(Event::RegisteredInDataset, this);
		foreach (BaseObject*, it, objects)
		{
			name = (*it)->getName();
			if (this->objects.hasKey(name) || this->animators.hasKey(name))
			{
				// this exception cannot be disabled on purpose
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
		// if no root objects exists, this root becomes the new root object
		if (setRootIfNull && this->root == NULL)
		{
			Object* rootObject = dynamic_cast<Object*>(root);
			if (rootObject != NULL)
			{
				this->root = rootObject;
			}
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
			// this exception cannot be disabled on purpose
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
	
	void Dataset::registerTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (this->textures.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectExistsException("Texture", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->textures[name] = texture;
		texture->dataset = this;
	}

	void Dataset::unregisterTexture(Texture* texture)
	{
		hstr name = texture->getFilename();
		if (!this->textures.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Texture", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->textures.removeKey(name);
		texture->dataset = NULL;
	}
	
	void Dataset::registerImage(BaseImage* image)
	{
		hstr name = image->getName();
		if (this->images.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->images[name] = image;
		image->dataset = this;
	}

	void Dataset::unregisterImage(BaseImage* image)
	{
		hstr name = image->getName();
		if (!this->images.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->images.removeKey(name);
		image->dataset = NULL;
	}

	void Dataset::registerStyle(Style* style)
	{
		hstr name = style->getName();
		if (this->styles.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectExistsException("Style", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->styles[name] = style;
		style->dataset = this;
	}

	void Dataset::unregisterStyle(Style* style)
	{
		hstr name = style->getName();
		if (!this->styles.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Style", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return);
		}
		this->styles.removeKey(name);
		style->dataset = NULL;
	}

	Object* Dataset::getObject(chstr name)
	{
		int dot = name.indexOf('.');
		if (dot < 0)
		{
			if (!this->objects.hasKey(name))
			{
				__THROW_EXCEPTION(ObjectNotExistsException("Object", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
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
		}
		if (dataset == NULL)
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Object", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
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
				__THROW_EXCEPTION(ObjectNotExistsException("Animator", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
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
		}
		if (dataset == NULL)
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Animator", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
		}
		return dataset->getAnimator(name(dot + 1, -1));
	}

	Object* Dataset::tryGetObject(chstr name) const
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
		}
		return (dataset != NULL ? dataset->tryGetObject(name(dot + 1, -1)) : NULL);
	}

	Animator* Dataset::tryGetAnimator(chstr name) const
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
		}
		return (dataset != NULL ? dataset->tryGetAnimator(name(dot + 1, -1)) : NULL);
	}

	Texture* Dataset::getTexture(chstr name)
	{
		if (!this->textures.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Texture", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
		}
		return this->textures[name];
	}
	
	BaseImage* Dataset::getImage(chstr name)
	{
		if (name == "null") // DEPRECATED
		{
			hlog::warn(logTag, "The 'null' image name has been deprecated. Use an empty string instead to define 'no image'.");
			return NULL;
		}
		BaseImage* image = this->images.tryGet(name, NULL);
		if (image == NULL)
		{
			int dot = name.indexOf('.');
			if (dot < 0)
			{
				__THROW_EXCEPTION(ObjectNotExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
			}
			Dataset* dataset = NULL;
			try
			{
				dataset = aprilui::getDatasetByName(name(0, dot));
			}
			catch (hexception&)
			{
			}
			if (dataset == NULL)
			{
				__THROW_EXCEPTION(ObjectNotExistsException("Image", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
			}
			image = dataset->getImage(name(dot + 1, -1));
		}
		return image;
	}

	Style* Dataset::getStyle(chstr name)
	{
		Style* style = this->styles.tryGet(name, NULL);
		if (style == NULL)
		{
			int dot = name.indexOf('.');
			if (dot < 0)
			{
				__THROW_EXCEPTION(ObjectNotExistsException("Style", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
			}
			Dataset* dataset = NULL;
			try
			{
				dataset = aprilui::getDatasetByName(name(0, dot));
			}
			catch (hexception&)
			{
			}
			if (dataset == NULL)
			{
				__THROW_EXCEPTION(ObjectNotExistsException("Style", name, this->name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
			}
			style = dataset->getStyle(name(dot + 1, -1));
		}
		return style;
	}

	bool Dataset::hasObject(chstr name) const
	{
		return (this->tryGetObject(name) != NULL);
	}

	bool Dataset::hasAnimator(chstr name) const
	{
		return (this->tryGetAnimator(name) != NULL);
	}

	bool Dataset::hasTexture(chstr name) const
	{
		return this->textures.hasKey(name);
	}

	bool Dataset::hasImage(chstr name) const
	{
		return this->images.hasKey(name);
	}

	bool Dataset::hasStyle(chstr name) const
	{
		return this->styles.hasKey(name);
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
			callback = this->callbackQueue.removeFirst();
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
		// don't change this, other implementations would require QueuedCallback::operator==
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
			delete this->callbackQueue.removeAt(*it).args;
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

	void Dataset::unloadUnusedResources()
	{
		this->_closeDocuments();
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
			april::window->hideVirtualKeyboard();
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
				hlog::error(logTag, text);
				return text;
			}
			if (!this->hasTextEntry(key))
			{
				hlog::warnf(logTag, "Text key '%s' does not exist!", key.cStr());
			}
			return this->getTextEntry(key);
		}
		int index = (int)uChars.find_first_of('}');
		if (index < 0)
		{
			hlog::errorf(logTag, "Could not parse formatted key '%s'.", key.cStr());
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
			hlog::writef(logTag, "- while processing args: '%s' with args '%s'.", format.cStr(), argString.cStr());
			return key;
		}
		ustr uPreprocessedFormat;
		harray<ustr> uPreprocessedArgs;
		if (!this->_preprocessCompositeTextKeyFormat(uFormat, uArgs, uPreprocessedFormat, uPreprocessedArgs))
		{
			hstr format = hstr::fromUnicode(harray<unsigned int>(uFormat.c_str(), (int)uFormat.size()));
			hstr argString = hstr::fromUnicode(harray<unsigned int>(uArgString.c_str(), (int)uArgString.size()));
			hlog::writef(logTag, "- while preprocessing format: '%s' with args '%s'.", format.cStr(), argString.cStr());
			return key;
		}
		hstr result;
		if (!this->_processCompositeTextKeyFormat(uPreprocessedFormat, uPreprocessedArgs, result))
		{
			hstr format = hstr::fromUnicode(harray<unsigned int>(uFormat.c_str(), (int)uFormat.size()));
			hstr argString = hstr::fromUnicode(harray<unsigned int>(uArgString.c_str(), (int)uArgString.size()));
			hlog::writef(logTag, "- while processing format: '%s' with args '%s'.", format.cStr(), argString.cStr());
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
				hlog::error(logTag, "'{' without '}' or '}' without '{'.");
				return false;
			}
			if (closeIndex < openIndex)
			{
				hlog::error(logTag, "'}' before '{'.");
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
		harray<int> indices;
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
				hlog::error(logTag, "Last character is '%'!");
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
					hlog::error(logTag, "Not enough args!");
					return false;
				}
				uPreprocessedFormat += uFormat.substr(0, index + 2);
				uFormat = uFormat.substr(index + 2, uFormat.size() - index - 2);
				uPreprocessedArgs += uArgs.removeFirst();
				continue;
			}
			if (uFormat[index + 1] == 'f')
			{
				if (uArgs.size() == 0)
				{
					hlog::error(logTag, "Not enough args!");
					return false;
				}
				uArg = uArgs.removeFirst();
				uPreprocessedFormat += uFormat.substr(0, index) + uArg;
				uFormat = uFormat.substr(index + 2, uFormat.size() - index - 2);
				if (!this->_getCompositeTextKeyFormatIndices(uArg, indices))
				{
					return false;
				}
				if (indices.size() > uArgs.size())
				{
					hlog::error(logTag, "Not enough args!");
					return false;
				}
				uPreprocessedArgs += uArgs.removeFirst(indices.size());
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
		harray<int> indices;
		if (!this->_getCompositeTextKeyFormatIndices(uFormat, indices))
		{
			return false;
		}
		if (uArgs.size() < indices.size())
		{
			hlog::error(logTag, "Not enough args!");
			return false;
		}
		if (indices.size() > uArgs.size())
		{
			hlog::error(logTag, "Too many args!");
			return false;
		}
		foreach (int, it, indices)
		{
			uResult += uFormat.substr(0, (*it));
			uResult += uArgs.removeFirst();
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

	bool Dataset::_getCompositeTextKeyFormatIndices(ustr uFormat, harray<int>& indices)
	{
		indices.clear();
		// finding formatting indices
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
				hlog::error(logTag, "Last character is '%'!");
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
				hlog::errorf(logTag, "Unsupported formatting '%%%c'!", uFormat[index + 1]);
				return false;
			}
			indices += currentIndex + index;
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
		keys.removeAll("");
		harray<ustr> result;
		foreach (hstr, it, keys)
		{
			result += this->getTextEntry(*it).uStr();
		}
		return result;
	}

	void Dataset::_throwInvalidObjectTypeCast(chstr typeName, chstr objName, chstr datasetName)
	{
		// this exception cannot be disabled on purpose
		throw InvalidObjectTypeCast(typeName, objName, datasetName);
	}

}
