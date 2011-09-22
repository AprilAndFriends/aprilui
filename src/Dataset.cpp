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
#include <hltypes/harray.h>
#include <hltypes/hdir.h>
#include <hltypes/hfile.h>
#include <hltypes/hmap.h>
#include <hltypes/util.h>

#include "Animators.h"
#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "Images.h"
#include "Objects.h"
#include "Util.h"
#include "xmlHelper.h"

namespace aprilui
{
	void _registerDataset(chstr name, Dataset* dataset);
	void _unregisterDataset(chstr name, Dataset* dataset);
	
	NullImage nullImage;
	
	Dataset::Dataset(chstr filename, chstr name)
	{
		mFocusedObject = NULL;
		mFilename = normalize_path(filename);
		int slash = mFilename.rfind('/');
		int dot = mFilename.rfind('.');
		mFilenamePrefix = mFilename(0, slash);
		mName = (name == "" ? mFilename(slash + 1, dot - slash - 1) : name);
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
	
	april::Texture* Dataset::parseTexture(xml_node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		hstr filepath = normalize_path(mFilenamePrefix + "/" + filename);

		int slash = filename.rfind('/') + 1;
		hstr textureName = filename(slash, filename.rfind('.') - slash);
		if (mTextures.has_key(textureName))
		{
			throw ObjectExistsException(filename);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		bool dynamicLoad = node->pbool("dynamic_load", false);
		
		if (mTexExtOverride && filepath.contains("."))
		{
			hstr left,right;
			filepath.rsplit(".", left, right);
			filepath = left + "." + mTexExtOverride;
		}
		hstr locale = getLocalization();
		if (locale)
		{
			harray<hstr> e = filepath.rsplit("/",1);
			hstr locpath = e[0] + "/" + locale + "/" + e[1];
			if (hfile::exists(locpath)) filepath = locpath;
		}

		april::Texture* texture = april::rendersys->loadTexture(filepath, dynamicLoad);
		if (texture == NULL)
		{
			throw FileNotFoundException(filepath);
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
		if (node->iter_children() == NULL) // if there are no images defined, create one that fills the whole area
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
			for (node = node->iter_children(); node != NULL; node = node->next())
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
	
	void Dataset::parseRAMTexture(xml_node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		hstr filepath = normalize_path(mFilenamePrefix + "/" + filename);
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
			throw FileNotFoundException(filepath);
		}
		mTextures[textureName] = texture;
	}
	
	void Dataset::parseCompositeImage(xml_node* node)
	{
		hstr name = node->pstr("name");
		hstr refname;
		if (mImages.has_key(name))
		{
			throw ResourceExistsException(name, "CompositeImage", this);
		}
		CompositeImage* image = new CompositeImage(name, node->pfloat("w"), node->pfloat("h"));
		for (node = node->iter_children(); node != NULL; node=node->next())
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
	
	Object* Dataset::parseObject(xml_node* node, Object* parent)
	{
		return recursiveObjectParse(node, parent);
	}
	
	Object* Dataset::recursiveObjectParse(xml_node* node, Object* parent)
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
				xmlSetProp(node, (xmlChar*)"name", (xmlChar*)objectName.c_str());
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
		
		/*if*/parse(DummyObject);
		else  parse(CallbackObject);
		else  parse(ColoredQuad);
		else  parse(ImageBox);
		else  parse(ColoredImageBox);
		else  parse(ImageButton);
		else  parse(TextImageButton);
		else  parse(Slider);
		else  parse(ToggleButton);
		else  parse(Label);
		else  parse(TextButton);
		else  parse(EditBox);
		else  parse(RotationImageBox);
		else  parse(RotatableImageBox);
		else if (*node == "Animator")
		{
			/*if*/parse_animator(Mover);
			else  parse_animator(MoveOscillator);
			else  parse_animator(Scaler);
			else  parse_animator(ScaleOscillator);
			else  parse_animator(Rotator);
			else  parse_animator(RotationOscillator);
			else  parse_animator(ColorAlternator);
			else  parse_animator(AlphaFader);
			else  parse_animator(AlphaOscillator);
			else  parse_animator(AlphaHover);
			else  parse_animator(Blinker);
			else  parse_animator(FrameAnimation);
			else  parse_animator(Earthquake);
			else object = parseExternalObjectClass(node, objectName, rect);
		}
		else
		{
			object = parseExternalObjectClass(node, objectName, rect);
		}
		
		if (object == NULL)
		{
			throw XMLUnknownClassException(className, node);
		}
		object->_setDataset(this);
		for (xml_prop* prop = node->iter_properties(); prop != NULL; prop = prop->next())
		{
			object->setProperty(prop->name(), prop->value());
		}
		mObjects[objectName] = object;
		if (parent != NULL)
		{
			parent->addChild(object);
		}
		
		for (node = node->iter_children(); node != NULL; node = node->next())
		{
			if (node->type != XML_TEXT_NODE && node->type != XML_COMMENT_NODE)
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
		}
		return object;
	}
	
	void Dataset::readFile(chstr filename)
	{
		// parse datadef xml file, error checking first
		xml_doc doc(getPWD() + "/" + normalize_path(filename));
		xml_node* current = doc.root("DataDefinition");
		
		parseExternalXMLNode(current);
		
		hmap<april::Texture*, hstr> dynamicLinks;
		hstr links;
		for (xml_node* p = current->iter_children(); p != NULL; p = p->next())
		{
			if      (*p == "Texture")
			{
				april::Texture* texture = parseTexture(p);
				if (p->pexists("dynamic_link"))
				{
					links = p->pstr("dynamic_link");
					dynamicLinks[texture] = links;
				}
			}
			else if (*p == "RAMTexture") parseRAMTexture(p);
			else if (*p == "CompositeImage") parseCompositeImage(p);
			else if (*p == "Object") parseObject(p);
			else if (p->type != XML_TEXT_NODE && p->type != XML_COMMENT_NODE)
			{
				parseExternalXMLNode(p);
			}
		}
		
		// adjust dynamic texture links
		harray<hstr> dlst;
		for (hmap<april::Texture*, hstr>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
		{
			dlst = it->second.split(',');
			foreach (hstr, it2, dlst)
			{
				it->first->addDynamicLink(getTexture(*it2));
			}
		}
	}
	
	void Dataset::load(chstr path)
	{
		hstr textsPath = (path != "" ? path : getDefaultTextsPath());
		hstr base_dir = pathGetBaseDir(mFilename);
		// texts
		logMessage("loading texts");
		hstr filepath = normalize_path(mFilenamePrefix + "/" + textsPath);
		_loadTexts(filepath);
		// audio
		mLoaded = true;
		logMessage("loading datadef: " + mFilename);
		readFile(mFilename);
		this->update(0);
	}
	
	void Dataset::_loadTexts(hstr path)
	{
		hstr localization = getLocalization();
		if (localization) path += "/" + localization;
		logMessage("loading texts from '" + path + "'");
		harray<hstr> files = hdir::files(path, true);
		harray<hstr> lines;
		harray<hstr> values;
		bool keyMode = true;
		hstr key;
		hfile f;
		foreach (hstr, it, files)
		{
			if (!it->ends_with(".loc")) continue;
			f.open(*it);
			if (!f.is_open())
			{
				throw hl_exception("Failed to load file " + (*it));
			}
			lines = f.read_lines();
			f.close();
			
			if (lines.size() == 0) continue; // empty file

			while (lines[0].size() > 0 && lines[0][0] < 0) lines[0]=lines[0](1,lines[0].size()-1);

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
		foreach_m(Object*, it, mObjects)
		{
			delete it->second;
		}
		mObjects.clear();
		foreach_m(Image*, it, mImages)
		{
			delete it->second;
		}
		mImages.clear();
		foreach_m(april::Texture*, it, mTextures)
		{
			delete it->second;
		}
		mTextures.clear();
		mCallbacks.clear();
		mTexts.clear();
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
		
		if (!mImages.has_key(name) && name.starts_with("0x")) // create new image with a color. don't overuse this,it's meant to be handy when needed only ;)
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
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->draw();
			}
		}
	}
	
	void Dataset::OnMouseDown(float x, float y, int button)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnMouseDown(x, y, button);
			}
		}
	}
	
	void Dataset::OnMouseUp(float x, float y, int button)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnMouseUp(x, y, button);
			}
		}
	}
	
	void Dataset::OnMouseMove(float x, float y)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnMouseMove(x, y);
			}
		}
	}
	
	void Dataset::OnKeyDown(unsigned int keycode)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnKeyDown(keycode);
			}
		}
	}
	
	void Dataset::OnKeyUp(unsigned int keycode)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnKeyUp(keycode);
			}
		}
	}
	
	void Dataset::OnChar(unsigned int charcode)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnChar(charcode);
			}
		}
	}
	
	void Dataset::updateTextures(float k)
	{
		foreach_m (april::Texture*, it, mTextures)
		{
			it->second->update(k);
		}
	}
	
	void Dataset::update(float k)
	{
		updateTextures(k);
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->update(k);
			}
		}
	}
	
}
