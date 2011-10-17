/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hdir.h>
#include <hltypes/hfile.h>
#include <hltypes/hmap.h>
#include <hltypes/util.h>
#include <hlxml/Document.h>
#include <hlxml/Exception.h>
#include <hlxml/Node.h>
#include <hlxml/Property.h>

#include "Animators.h"
#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "Images.h"
#include "Objects.h"
#include "Util.h"

namespace aprilui
{
	void _registerDataset(chstr name, Dataset* dataset);
	void _unregisterDataset(chstr name, Dataset* dataset);
	
	NullImage nullImage;
	
	Dataset::Dataset(chstr filename, chstr name, bool useNameBasePath)
	{
		mFocusedObject = NULL;
		mRoot = NULL;
		mFilename = normalize_path(filename);
		int slash = mFilename.rfind('/');
		int dot = mFilename.rfind('.');
		if (name == "")
		{
			mFilePath = normalize_path(mFilename(0, slash));
			mName = (name == "" ? mFilename(slash + 1, dot - slash - 1) : name);
		}
		else
		{
			hstr extension = mFilename(dot, mFilename.size() - dot);
			if (mFilename.ends_with(name + extension) && useNameBasePath)
			{
				mFilePath = normalize_path(mFilename.replace(name + extension, ""));
			}
			else
			{
				mFilePath = normalize_path(mFilename(0, slash));
			}
			mName = name;
		}
		mLoaded = false;
		_registerDataset(mName, this);
	}
	
	Dataset::~Dataset()
	{
		if (isLoaded())
		{
			unload();
		}
		_unregisterDataset(mName, this);
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
		mObjects.remove_key(object->getName());
		delete object;
	}
	
	void Dataset::destroyAndDetachObject(chstr name, bool recursive)
	{
		destroyAndDetachObject(getObject(name), recursive);
	}
	
	void Dataset::destroyAndDetachObject(Object* object, bool recursive)
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
				object->removeChild(object->getChildren()[0]);
			}
		}
		if (object->getParent() != NULL)
		{
			object->detach();
		}
		mObjects.remove_key(object->getName());
		delete object;
	}
	
	void Dataset::_destroyTexture(april::Texture* texture)
	{
		if (!mTextures.has_key(texture->getFilename()))
		{
			throw ResourceNotExistsException(texture->getFilename(), "april::Texture", this);
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
			throw ResourceNotExistsException(name, "april::Texture", this);
		}
		april::Texture* texture = mTextures[name];
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
	
	april::Texture* Dataset::parseTexture(hlxml::Node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		hstr filepath = normalize_path(mFilePath + "/" + filename);
		int slash = filename.rfind('/') + 1;
		hstr textureName = filename(slash, filename.rfind('.') - slash);
		if (mTextures.has_key(textureName))
		{
			throw ObjectExistsException(textureName, filename);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		bool dynamicLoad = node->pbool("dynamic_load", false);
		
		april::Texture* texture = april::rendersys->loadTexture(filepath, dynamicLoad);
		if (texture == NULL)
		{
			throw file_not_found(filepath);
		}
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if      (filter == "linear")  texture->setTextureFilter(april::Linear);
			else if (filter == "nearest") texture->setTextureFilter(april::Nearest);
			else throw hl_exception("texture filter '" + filter + "' not supported");
		}
		texture->setTextureWrapping(node->pbool("wrap", true));
		mTextures[textureName] = texture;
		// extract image definitions
		if (node->iterChildren() == NULL) // if there are no images defined, create one that fills the whole area
		{
			if (mImages.has_key(textureName))
			{
				throw ResourceExistsException(filename, "april::Texture", this);
			}
			mImages[textureName] = new Image(texture, filename, grect(0, 0, (float)texture->getWidth(), (float)texture->getHeight()));
		}
		else
		{
			Image* image;
			for (node = node->iterChildren(); node != NULL; node = node->next())
			{
				if (*node == "Image")
				{
					hstr name = (prefixImages ? textureName + "/" + node->pstr("name") : node->pstr("name"));
					if (mImages.has_key(name))
					{
						throw ResourceExistsException(name, "Image", this);
					}
					grect rect(node->pfloat("x"), node->pfloat("y"), node->pfloat("w"), node->pfloat("h"));
					
					bool vertical = node->pbool("vertical", false);
					float tile_w = node->pfloat("tile_w", 1.0f);
					float tile_h = node->pfloat("tile_h", 1.0f);
					
					if (tile_w != 1.0f || tile_h != 1.0f)
					{
						image = new TiledImage(texture, name, rect, vertical, tile_w, tile_h);
					}
					else if (node->pexists("color"))
					{
						april::Color color(node->pstr("color"));
						image = new ColoredImage(texture, name, rect, vertical, color);
					}
					else
					{
						bool invertX = node->pbool("invertx", false);
						bool invertY = node->pbool("inverty", false);
						image = new Image(texture, name, rect, vertical, invertX, invertY);    
					}
					hstr mode = node->pstr("blend_mode", "default");
					if (mode == "add")
					{
						image->setBlendMode(april::ADD);
					}
					mImages[name] = image;
				}
		    }
		}
		return texture;
	}
	
	void Dataset::parseRAMTexture(hlxml::Node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		hstr filepath = normalize_path(mFilePath + "/" + filename);
		int slash = filename.find('/') + 1;
		hstr textureName = filename(slash, filename.rfind('.') - slash);
		if (mTextures.has_key(textureName))
		{
			throw ResourceExistsException(filename, "RAMTexture", this);
		}
		bool dynamicLoad = node->pbool("dynamic_load", false);
		april::Texture* texture = april::rendersys->loadRAMTexture(filepath, dynamicLoad);
		if (!texture)
		{
			throw file_not_found(filepath);
		}
		mTextures[textureName] = texture;
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
		for (node = node->iterChildren(); node != NULL; node = node->next())
		{
			if (*node == "ImageRef")
			{
				refname = node->pstr("name");
				image->addImageRef(getImage(refname),
					grect(node->pfloat("x"), node->pfloat("y"), node->pfloat("w"), node->pfloat("h")));
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
		hstr className = node->pstr("type");
		
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
		Object* object;
		
	#define parse(cls) if (className == #cls) object = new cls(objectName, rect)
	#define parse_animator(cls) if (className == #cls) object = new Animators::cls(objectName)
		
		/*if*/parse(CallbackObject);
		else  parse(ColoredQuad);
		else  parse(Container);
		else  parse(ImageBox);
		else  parse(ImageButton);
		else  parse(TextImageButton);
		else  parse(Slider);
		else  parse(ToggleButton);
		else  parse(Label);
		else  parse(TextButton);
		else  parse(EditBox);
#ifdef _HAVE_PARTICLE
		else  parse(Particle);
#endif
		else if (*node == "Animator")
		{
			/*if*/parse_animator(AlphaChanger);
			else  parse_animator(BlueChanger);
			else  parse_animator(FrameAnimation);
			else  parse_animator(GreenChanger);
			else  parse_animator(MoverX);
			else  parse_animator(MoverY);
			else  parse_animator(RedChanger);
			else  parse_animator(Rotator);
			else  parse_animator(ResizerX);
			else  parse_animator(ResizerY);
			else  parse_animator(ScalerX);
			else  parse_animator(ScalerY);
			else object = parseExternalObjectClass(node, objectName, rect);
		}
		else
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

		for (hlxml::Property* prop = node->iterProperties(); prop != NULL; prop = prop->next())
		{
            name = prop->name();
            if (name == "x" || name == "y" || name == "w" || name == "h")
			{
				continue; // TODO - should be done better, maybe reading parameters from a list, then removing them so they aren't set more than once
			}
			object->setProperty(name, prop->value());
		}
		
		for (node = node->iterChildren(); node != NULL; node = node->next())
		{
				if (*node == "Property")
				{
					object->setProperty(node->pstr("name"), node->pstr("value"));
				}
				else
				{
					recursiveObjectParse(node, object);
				}
		}
		return object;
	}
	
	void Dataset::readFile(chstr filename)
	{
		// parse datadef xml file, error checking first
		hstr path(getPWD() + "/" + normalize_path(filename));
#ifdef NO_FS_TREE
		path = path.ltrim('.');
		path = path.ltrim('/');
		path = path.replace("/","___");
		
#endif
		hlxml::Document doc(path);
		hlxml::Node* current = doc.root("DataDefinition");

		parseExternalXMLNode(current);
		
		hmap<april::Texture*, hstr> dynamicLinks;
		hstr links;

		for (hlxml::Node* p = current->iterChildren(); p != NULL; p = p->next())
		{
			if      (*p == "Texture")
			{
				april::Texture* texture = parseTexture(p);
				// TODO - needs to be removed, kill it with fire
				/// {
				if (p->pexists("dynamic_link"))
				{
					links = p->pstr("dynamic_link");
					dynamicLinks[texture] = links;
				}
				/// }
			}
			else if (*p == "RAMTexture") parseRAMTexture(p);
			else if (*p == "CompositeImage") parseCompositeImage(p);
			else if (*p == "Object") parseObject(p);
			else if (*p == "TextureGroup") parseTextureGroup(p);
			else
			{
				parseExternalXMLNode(p);
			}
		}
		
		// TODO - needs to be removed, kill it with fire
		/// {
		// adjust dynamic texture links
		harray<hstr> dlst;
		for (hmap<april::Texture*, hstr>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
		{
			dlst = it->second.split(',', -1, true);
			foreach (hstr, it2, dlst)
			{
				it->first->addDynamicLink(getTexture(*it2));
			}
		}
		/// }
	}
	
	void Dataset::load(chstr path)
	{
		hstr textsPath = (path != "" ? path : getDefaultTextsPath()) + "/" + getLocalization();
		hstr filepath = normalize_path(mFilePath + "/" + textsPath);
#ifdef NO_FS_TREE
		textsPath = textsPath.ltrim('.').ltrim('.').ltrim('/');
		textsPath = textsPath.replace("/", "___");
		filepath = filepath.replace("/", "___");
#endif
		// texts
		_loadTexts(filepath);
		aprilui::log("loading datadef: " + mFilename);
		readFile(mFilename);
		mLoaded = true;
		this->update(0);
	}
	
	void Dataset::_loadTexts(chstr path)
	{
		aprilui::log("loading texts from '" + path + "'");
		harray<hstr> files = hdir::files(path, true);
		harray<hstr> lines;
		harray<hstr> values;
		bool keyMode = true;
		hstr key;
		hfile f;
		foreach (hstr, it, files)
		{
			f.open(*it);
			if (!f.is_open())
			{
				throw hl_exception("Failed to load file " + (*it));
			}
			lines = f.read_lines();
			f.close();

			// ignore file header, silly utf-8 encoded text files have 2-3 char markers
			while (lines[0].size() > 0 && lines[0][0] < 0) lines[0] = lines[0](1, lines[0].size() - 1);

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
						key = (*it2);
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
		foreach_m (april::Texture*, it, mTextures)
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
	
	void Dataset::registerManualTexture(april::Texture* tex)
	{
		hstr name = tex->getFilename();

		if (mTextures.has_key(name))
		{
			throw ResourceExistsException(name, "Texture", this);
		}
		mTextures[name] = tex;
	}

	void Dataset::unregisterManualTexture(april::Texture* tex)
	{
		hstr name = tex->getFilename();
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
		aprilui::Animator* object;
		foreach_m (Object*, it, mObjects)
		{
			object = dynamic_cast<aprilui::Animator*>(it->second);
			if (object != NULL && object->isWaitingAnimation())
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
	
	april::Texture* Dataset::getTexture(chstr name)
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
	
	hstr Dataset::getText(chstr name)
	{
		return mTexts[name];
	}
	
	bool Dataset::textExists(chstr name)
	{
		return mTexts.has_key(name);
	}
	
	void Dataset::registerCallback(chstr name, void (*callback)())
	{
		mCallbacks[name] = callback;
	}
	
	void Dataset::triggerCallback(chstr name)
	{
		if (mCallbacks.has_key(name))
		{
			mCallbacks[name]();
		}
	}
	
	void Dataset::draw()
	{
		if (mRoot != NULL)
		{
			mRoot->draw();
		}
	}
	
	bool Dataset::onMouseDown(float x, float y, int button)
	{
		return (mRoot != NULL && mRoot->onMouseDown(x, y, button));
	}
	
	bool Dataset::onMouseUp(float x, float y, int button)
	{
		return (mRoot != NULL && mRoot->onMouseUp(x, y, button));
	}
	
	void Dataset::onMouseMove(float x, float y)
	{
		if (mRoot != NULL)
		{
			mRoot->onMouseMove(x, y);
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
		foreach_m (april::Texture*, it, mTextures)
		{
			it->second->update(k);
		}
	}
	
	void Dataset::unloadUnusedTextures()
	{
		foreach_m(april::Texture*, it, mTextures)
		{
			if (it->second->isDynamic() && it->second->getUnusedTime() > 1) it->second->unload();
		}
	}
	
	void Dataset::update(float k)
	{
		updateTextures(k);
		if (mRoot != NULL)
		{
			mRoot->update(k);
		}
	}
	
}
