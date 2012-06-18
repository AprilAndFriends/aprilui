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
#include <april/Texture.h>
#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hdir.h>
#include <hltypes/hfile.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>

#include "Animators.h"
#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "Images.h"
#include "Objects.h"
#include "Texture.h"
#include "Util.h"
#include "xmlHelper.h"

namespace aprilui
{
	void _registerDataset(chstr name, Dataset* dataset);
	void _unregisterDataset(chstr name, Dataset* dataset);
	
	NullImage nullImage;
	
	Dataset::Dataset(chstr filename, chstr name) : EventReceiver()
	{
		mFocusedObject = NULL;
		mFilename = normalize_path(filename);
		mFilenamePrefix = _makeFilePath(mFilename, name, false);
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

	Texture* Dataset::parseTexture(xml_node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		if (filename.starts_with("$")) filename = expandMacro(filename);
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

		filepath = _makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->loadTexture(filepath, aprilui::getForcedDynamicLoading() || dynamicLoad);
		if (aprilTexture == NULL)
		{
			throw FileNotFoundException(filepath);
		}
		Texture* texture = new Texture(filepath, aprilTexture);
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if      (filter == "linear")  texture->setFilter(april::Texture::FILTER_LINEAR);
			else if (filter == "nearest") texture->setFilter(april::Texture::FILTER_NEAREST);
			else throw hl_exception("texture filter '" + filter + "' not supported");
		}
		texture->setAddressMode(node->pbool("wrap", true) ? april::Texture::ADDRESS_WRAP : april::Texture::ADDRESS_CLAMP);
		mTextures[textureName] = texture;
		// extract image definitions
		if (node->iter_children() == NULL) // if there are no images defined, create one that fills the whole area
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
	
	void Dataset::parseRamTexture(xml_node* node)
	{
		hstr filename = normalize_path(node->pstr("filename"));
		hstr filepath = normalize_path(mFilenamePrefix + "/" + filename);
		int slash = filename.find('/') + 1;
		hstr textureName = filename(slash, filename.rfind('.') - slash);
		if (mTextures.has_key(textureName))
		{
			throw ResourceExistsException(filename, "RamTexture", this);
		}
		bool dynamicLoad = node->pbool("dynamic_load", false);
		april::Texture* aprilTexture = april::rendersys->loadRamTexture(filepath, aprilui::getForcedDynamicLoading() || dynamicLoad);
		if (!aprilTexture)
		{
			throw file_not_found(filepath);
		}
		mTextures[textureName] = new Texture(filepath, aprilTexture);
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
		
		hmap<Texture*, hstr> dynamicLinks;
		hstr links;
		for (xml_node* p = current->iter_children(); p != NULL; p = p->next())
		{
			if      (*p == "Texture")
			{
				Texture* texture = parseTexture(p);
				if (p->pexists("dynamic_link"))
				{
					links = p->pstr("dynamic_link");
					dynamicLinks[texture] = links;
				}
			}
			else if (*p == "RamTexture") parseRamTexture(p);
			else if (*p == "CompositeImage") parseCompositeImage(p);
			else if (*p == "Object") parseObject(p);
			else if (p->type != XML_TEXT_NODE && p->type != XML_COMMENT_NODE)
			{
				parseExternalXMLNode(p);
			}
		}
		
		// adjust dynamic texture links
		harray<hstr> dlst;
		for (hmap<Texture*, hstr>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
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
		foreach_m (Texture*, it, mTextures)
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
	
	void Dataset::registerManualTexture(Texture* tex)
	{
		hstr filename = tex->getFilename();
		int slash = filename.rfind('/') + 1;
		hstr name = filename(slash, filename.rfind('.') - slash);
		if (mTextures.has_key(name))
		{
			throw ResourceExistsException(name, "Texture", this);
		}
		mTextures[name] = tex;
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
	
	hstr Dataset::getTextEntry(chstr textKey)
	{
		return mTexts[textKey];
	}
	
	hstr Dataset::getText(chstr compositeTextKey)
	{
		return _parseCompositeTextKey(compositeTextKey);
	}
	
	bool Dataset::textExists(chstr textKey)
	{
		return mTexts.has_key(textKey);
	}
	
	harray<hstr> Dataset::getTextEntries(harray<hstr> keys)
	{
		return mTexts.values(keys & mTexts.keys());
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
		foreach_m (Texture*, it, mTextures)
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
	
	void Dataset::notifyEvent(chstr name, void* params)
	{
		if (name == "onLocalizationChanged")
		{
			mTexts.clear();
			hstr filepath = normalize_path(mFilenamePrefix + "/" + getDefaultTextsPath() + "/" + getLocalization());
			_loadTexts(filepath);
			foreach_m (aprilui::Texture*, it, mTextures)
			{
				it->second->reload(_makeLocalizedTextureName(it->second->getOriginalFilename()));
			}
		}
		foreach_m (aprilui::Object*, it, mObjects)
		{
			it->second->notifyEvent(name, params);
		}
	}
	
	hstr Dataset::_parseCompositeTextKey(chstr key)
	{
		if (!key.starts_with("{"))
		{
			if (!mTexts.has_key(key))
			{
				aprilui::log(hsprintf("WARNING! Text key '%s' does not exist", key.c_str()));
			}
			return mTexts[key];
		}
		int index = key.find_first_of('}');
		if (index < 0)
		{
			aprilui::log(hsprintf("WARNING! Error while trying to parse formatted key '%s'.", key.c_str()));
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
				aprilui::log("WARNING! '{' without '}' or '}' without '{'");
				return false;
			}
			if (closeIndex < openIndex)
			{
				aprilui::log("WARNING! '}' before '{'");
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
				aprilui::log("WARNING! Last character is '%'");
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
					aprilui::log("WARNING! Not enough args");
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
					aprilui::log("WARNING! Not enough args");
					return false;
				}
				preprocessedFormat += string(0, index) + args.pop_first();
				string = string(index + 2, string.size() - index - 2);
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
			aprilui::log("WARNING! Not enough args");
			return false;
		}
		if (indexes.size() > args.size())
		{
			aprilui::log("WARNING! Too many args");
			return false;
		}
		foreach (int, it, indexes)
		{
			result += string(0, (*it));
			result += args.pop_first();
			string = string((*it) + 2, string.size() - (*it) - 2);
		}
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
				aprilui::log("WARNING! Last character is '%'");
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
				aprilui::log(hsprintf("WARNING! Unsupported formatting '%%%c'", string[index + 1]));
				return false;
			}
			indexes += currentIndex + index;
			string = string(index + 2, string.size() - index - 2);
			currentIndex = 0;
		}
		return true;
	}

}
