/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <april/Texture.h>
#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hdir.h>
#include <hltypes/hfile.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hrdir.h>
#include <hltypes/hresource.h>
#include <hlxml/Exception.h>
#include <hlxml/Document.h>
#include <hlxml/Node.h>

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "Images.h"
#include "Objects.h"
#include "Texture.h"
#include "RamTexture.h"

namespace aprilui
{
	void _registerDataset(chstr name, Dataset* dataset);
	void _unregisterDataset(chstr name, Dataset* dataset);
	
	NullImage nullImage;
	
	Dataset::Dataset(chstr filename, chstr name) : EventReceiver()
	{
		mFocusedObject = NULL;
		mFilename = hdir::normalize(filename);
		mFilenamePrefix = _makeFilePath(mFilename, name, false);
		mFilePath = mFilenamePrefix;
		mName = name;
		if (mName == "")
		{
			mName = mFilename.rsplit(".", 1, false).removeFirst().rsplit("/", 1, false).removeLast();
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
			hstr extension = "." + filename.rsplit(".", -1, false).removeLast();
			if (filename.endsWith(name + extension))
			{
				return hdir::normalize(filename.replaced(name + extension, ""));
			}
		}
		return hdir::normalize(hdir::baseDir(filename));
	}
	
	void Dataset::_destroyTexture(Texture* texture)
	{
		if (!mTextures.hasKey(texture->getFilename()))
		{
			throw ApriluiResourceNotExistsException(texture->getFilename(), "Texture", this);
		}
		mTextures.removeKey(texture->getFilename());
		delete texture;
	}
	
	void Dataset::_destroyImage(Image* image)
	{
		if (!mImages.hasKey(image->getName()))
		{
			throw ApriluiResourceNotExistsException(image->getName(), "Image", this);
		}
		mImages.removeKey(image->getName());
		delete image;
	}
	
	void Dataset::_destroyTexture(chstr name)
	{
		if (!mTextures.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "Texture", this);
		}
		Texture* texture = mTextures[name];
		mTextures.removeKey(name);
		delete texture;
	}
	
	void Dataset::_destroyImage(chstr name)
	{
		if (!mImages.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "Image", this);
		}
		Image* image = mImages[name];
		mImages.removeKey(name);
		delete image;
	}
	
	hstr Dataset::_makeLocalizedTextureName(chstr filename)
	{
		hstr localization = getLocalization();
		if (localization != "")
		{
			hstr locpath = hdir::baseDir(filename) + "/" + localization + "/" + hdir::baseName(filename);
			locpath = april::rendersys->findTextureResource(locpath);
			if (locpath != "")
			{
				return locpath;
			}
		}
		return filename;
	}

	Texture* Dataset::parseTexture(hlxml::Node* node)
	{
		hstr filename = hdir::normalize(node->pstr("filename"));
		if (filename.startsWith("$")) filename = expandMacro(filename);
		hstr filepath = hdir::normalize(mFilenamePrefix + "/" + filename);

		int slash = filename.rindexOf('/') + 1;
		hstr textureName = filename(slash, filename.rindexOf('.') - slash);
		if (mTextures.hasKey(textureName))
		{
			throw ObjectExistsException(filename);
		}
		bool prefixImages = node->pbool("prefix_images", true);
		april::Texture::LoadMode loadMode = april::Texture::LoadMode::Async;
		if (aprilui::getForcedDynamicLoading() || node->pbool("dynamic_load", false))
		{
			loadMode = april::Texture::LoadMode::OnDemand;
		}
		if (mTexExtOverride && filepath.contains("."))
		{
			hstr left,right;
			filepath.rsplit(".", left, right);
			filepath = left + "." + mTexExtOverride;
		}

		hstr locpath = _makeLocalizedTextureName(filepath);
		april::Texture* aprilTexture = april::rendersys->createTextureFromResource(locpath, april::Texture::Type::Immutable, loadMode);
		if (aprilTexture == NULL)
		{
			throw FileNotFoundException(locpath);
		}
		Texture* texture = new Texture(filepath, aprilTexture);
		if (node->pexists("filter"))
		{
			hstr filter = node->pstr("filter");
			if      (filter == "linear")  texture->setFilter(april::Texture::Filter::Linear);
			else if (filter == "nearest") texture->setFilter(april::Texture::Filter::Nearest);
			else throw Exception("texture filter '" + filter + "' not supported");
		}
		texture->setAddressMode(node->pbool("wrap", true) ? april::Texture::AddressMode::Wrap : april::Texture::AddressMode::Clamp);
		mTextures[textureName] = texture;
		// extract image definitions
		if (node->children.size() == 0) // if there are no images defined, create one that fills the whole area
		{
			if (mImages.hasKey(textureName))
			{
				throw ApriluiResourceExistsException(filename, "Texture", this);
			}
			mImages[textureName] = new Image(texture, filename, grectf(0, 0, (float)texture->getWidth(), (float)texture->getHeight()));
		}
		else
		{
			Image* image = NULL;
			foreach_xmlnode (child, node)
			{
				if ((*child)->name == "Image")
				{
					hstr name = (prefixImages ? textureName + "/" + (*child)->pstr("name") : (*child)->pstr("name"));
					if (mImages.hasKey(name))
					{
						throw ApriluiResourceExistsException(name, "Image", this);
					}
					grectf rect;
					if ((*child)->pexists("rect")) // aprilui trunk compatibility
					{
						rect = april::hstrToGrectf((*child)->pstr("rect"));
					}
					else
					{
						rect.set((*child)->pfloat("x"), (*child)->pfloat("y"), (*child)->pfloat("w"), (*child)->pfloat("h"));
					}
					
					bool vertical = (*child)->pbool("vertical", false);
					float tile_w = (*child)->pfloat("tile_w", 1.0f);
					float tile_h = (*child)->pfloat("tile_h", 1.0f);
					
					if (tile_w != 1.0f || tile_h != 1.0f)
					{
						image = new TiledImage(texture, name, rect, vertical, tile_w, tile_h);
					}
					else if ((*child)->pexists("color"))
					{
						april::Color color((*child)->pstr("color"));
						image = new ColoredImage(texture, name, rect, vertical, color);
					}
					else
					{
						bool invertX = (*child)->pbool("invertx", false);
						bool invertY = (*child)->pbool("inverty", false);
						image = new Image(texture, name, rect, vertical, invertX, invertY);    
					}
					hstr mode = (*child)->pstr("blend_mode", "default");
					if (mode == "add")
					{
						image->setBlendMode(april::BlendMode::Add);
					}
					mImages[name] = image;
				}
		    }
		}
		return texture;
	}
	
	void Dataset::parseRamTexture(hlxml::Node* node)
	{
		hstr filename = hdir::normalize(node->pstr("filename"));
		hstr filepath = hdir::normalize(mFilenamePrefix + "/" + filename);
		int slash = filename.indexOf('/') + 1;
		hstr textureName = filename(slash, filename.rindexOf('.') - slash);
		if (mTextures.hasKey(textureName))
		{
			throw ApriluiResourceExistsException(filename, "RamTexture", this);
		}
//		bool dynamicLoad = node->pbool("dynamic_load", false);
		hstr locpath = april::rendersys->findTextureResource(_makeLocalizedTextureName(filepath));
		if (!hresource::exists(locpath))
		{
			throw FileNotFoundException(locpath);
		}
		RamTexture* tex = new RamTexture(locpath);
		mRamTextures[textureName] = tex;
	}
	
	void Dataset::parseCompositeImage(hlxml::Node* node)
	{
		hstr name = node->pstr("name");
		hstr refname;
		if (mImages.hasKey(name))
		{
			throw ApriluiResourceExistsException(name, "CompositeImage", this);
		}
		CompositeImage* image = new CompositeImage(name, node->pfloat("w"), node->pfloat("h"));
		foreach_xmlnode (child, node)
		{
			if ((*child)->name == "ImageRef")
			{
				refname = (*child)->pstr("name");
				image->addImageRef(getImage(refname), grectf((*child)->pfloat("x"), (*child)->pfloat("y"), (*child)->pfloat("w"), (*child)->pfloat("h")));
			}
		}
		mImages[name] = image;
	}
	
	Object* Dataset::parseObject(hlxml::Node* node, Object* parent)
	{
		return recursiveObjectParse(node, parent);
	}
	
	Object* Dataset::recursiveObjectParse(hlxml::Node* node, Object* parent)
	{
		hstr objectName;
		grectf rect(0, 0, 1, 1);
		hstr className = node->pstr("type");
		
		if (node->name == "Object")
		{
			if (node->pexists("name"))
			{
				objectName = node->pstr("name");
			}
			else
			{
				objectName = generateName(className);
			}
			rect.x = node->pfloat("x");
			rect.y = node->pfloat("y");
			rect.w = node->pfloat("w", -1.0f);
			rect.h = node->pfloat("h", -1.0f);
		}
		else if (node->name == "Animator")
		{
			objectName = node->pstr("name", generateName("Animator"));
		}
		else
		{
			return NULL;
		}
		if (mObjects.hasKey(objectName))
		{
			throw ApriluiResourceExistsException(objectName, "Object", this);
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
		else if (node->name == "Animator")
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
		hstr name;
		foreach_m (hstr, it, node->properties)
		{
			if (it->first == "x" || it->first == "y" || it->first == "w" || it->first == "h")
			{
				continue; // TODO - should be done better, maybe reading parameters from a list, then removing them so they aren't set more than once
			}
			object->setProperty(it->first, it->second);
		}
		mObjects[objectName] = object;
		if (parent != NULL)
		{
			parent->addChild(object);
		}
		
		foreach_xmlnode (child, node)
		{
			if ((*child)->type != hlxml::Node::Type::Text && (*child)->type != hlxml::Node::Type::Comment)
			{
				if ((*child)->name == "Property")
				{
					object->setProperty((*child)->pstr("name"), (*child)->pstr("value"));
				}
				else
				{
					recursiveObjectParse((*child), object);
				}
			}
		}
		return object;
	}
	
	void Dataset::readFile(chstr filename)
	{
		hstr path = hdir::normalize(filename);
		aprilui::log("parsing dataset file '" + path + "'");
		hlxml::Document* doc = new hlxml::Document(path);
		hlxml::Node* current = doc->root();

		parseExternalXMLNode(current);




		hmap<Texture*, hstr> dynamicLinks;
		hstr links;
		foreach_xmlnode (node, current)
		{
			if      ((*node)->name == "Texture")
			{
				Texture* texture = parseTexture(*node);
				if ((*node)->pexists("dynamic_link"))
				{
					links = (*node)->pstr("dynamic_link");
					dynamicLinks[texture] = links;
				}
			}
			else if ((*node)->name == "RamTexture")     parseRamTexture(*node);
			else if ((*node)->name == "CompositeImage") parseCompositeImage(*node);
			else if ((*node)->name == "Object")         parseObject(*node);
			else if ((*node)->type != hlxml::Node::Type::Text && (*node)->type != hlxml::Node::Type::Comment)
			{
				parseExternalXMLNode(*node);
			}
		}
		delete  doc;

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
	
	void Dataset::destroyObject(Object* object)
	{
		if (!this->mObjects.hasKey(object->getName()))
		{
			throw ApriluiResourceNotExistsException(object->getName(), "Object", this);
		}
		harray<Object*> children = object->getChildren();
		foreach (Object*, it, children)
		{
			this->destroyObject(*it);
		}
		if (object->getParent() != NULL)
		{
			object->getParent()->removeChild(object);
		}
		this->mObjects.removeKey(object->getName());
		delete object;
	}
	
	void Dataset::load(chstr path)
	{
		// texts
		logMessage("loading texts");
		_loadTexts(_makeTextsPath());
		// audio
		mLoaded = true;
		logMessage("loading datadef: " + mFilename);
		readFile(mFilename);
		this->update(0);
	}
	
	void Dataset::_loadTexts(hstr path)
	{
		logMessage("Loading texts: " + path);
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
				while (i < firstLine.size() && !hbetweenIE((int)firstLine[i], 0, 127))
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
						key = (*it2).split("#").first().trimmed(' ');
					}
				}
				else if ((*it2) == "}")
				{
					keyMode = true;
					if (key != "")
					{
						mTexts[key] = values.joined('\n');
					}
				}
				else
				{
					values += (*it2);
				}
			}
		}
	}
	
	hstr Dataset::_makeTextsPath()
	{
		hstr filepathPrefix = mFilenamePrefix + "/" + aprilui::getDefaultTextsPath();
		hstr filepath = hdir::normalize(filepathPrefix + "/" + aprilui::getLocalization());
		if (!hrdir::exists(filepath))
		{
			filepath = hdir::normalize(filepathPrefix);
		}
		return filepath;
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
		
		foreach_m (aprilui::RamTexture*, it, mRamTextures)
		{
			delete it->second;
		}
		mTextures.clear();
		mRamTextures.clear();
		mCallbacks.clear();
		mTexts.clear();
		mLoaded = false;
	}
	
	void Dataset::registerManualObject(Object* object)
	{
		hstr name = object->getName();
		if (mObjects.hasKey(name))
		{
			throw ApriluiResourceExistsException(name, "Object", this);
		}
		mObjects[name] = object;
		object->_setDataset(this);
	}
	
	void Dataset::unregisterManualObject(Object* object)
	{
		hstr name = object->getName();
		if (!mObjects.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "Object", this);
		}
		mObjects.removeKey(name);
		object->_setDataset(NULL);
	}
	
	void Dataset::registerObjects(Object* root, bool unused) // aprilui trunk compatibility
	{
		registerManualObject(root);
	}

	void Dataset::unregisterObjects(Object* root) // aprilui trunk compatibility
	{
		unregisterManualObject(root);
	}

	void Dataset::registerManualImage(Image* image)
	{
		hstr name = image->getName();
		if (mImages.hasKey(name))
		{
			throw ApriluiResourceExistsException(name, "Image", this);
		}
		mImages[name] = image;
	}
	
	void Dataset::unregisterManualImage(Image* image)
	{
		hstr name = image->getName();
		if (!mImages.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "Image", this);
		}
		mImages.removeKey(name);
	}
	
	void Dataset::registerImage(Image* img) // aprilui trunk compatibility
	{
		registerManualImage(img);
	}

	void Dataset::unregisterImage(Image* img) // aprilui trunk compatibility
	{
		unregisterManualImage(img);
	}

	void Dataset::registerManualTexture(Texture* tex)
	{
		hstr filename = tex->getFilename();
		int slash = filename.rindexOf('/') + 1;
		hstr name = filename(slash, filename.rindexOf('.') - slash);
		if (mTextures.hasKey(name))
		{
			throw ApriluiResourceExistsException(name, "Texture", this);
		}
		mTextures[name] = tex;
	}
	
	void Dataset::registerTexture(Texture* texture) // aprilui trunk compatibility
	{
		registerManualTexture(texture);
	}
	
	void Dataset::unregisterTexture(Texture* texture) // aprilui trunk compatibility
	{
		hstr filename = texture->getFilename();
		int slash = filename.rindexOf('/') + 1;
		hstr name = filename(slash, filename.rindexOf('.') - slash);
		if (!mTextures.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "Texture", this);
		}
		mTextures.removeKey(name);
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
		if (!mObjects.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "Object", this);
		}
		return mObjects[name];
	}
	
	Texture* Dataset::getTexture(chstr name)
	{
		if (!mTextures.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "Texture", this);
		}
		return mTextures[name];
	}
	
	RamTexture* Dataset::getRamTexture(chstr name)
	{
		if (!mRamTextures.hasKey(name))
		{
			throw ApriluiResourceNotExistsException(name, "RamTexture", this);
		}
		return mRamTextures[name];
	}
	
	Image* Dataset::getImage(chstr name)
	{
		Image* image;
		if (name == "null")
		{
			return &nullImage;
		}
		
		if (!mImages.hasKey(name) && name.startsWith("0x")) // create new image with a color. don't overuse this,it's meant to be handy when needed only ;)
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
			int dot = name.indexOf('.');
			if (dot < 0)
			{
				throw ApriluiResourceNotExistsException(name, "Image", this);
			}
			Dataset* dataset;
			try
			{
				dataset = getDatasetByName(name(0, dot));
			}
			catch (_GenericException)
			{
				throw ApriluiResourceNotExistsException(name, "Image", this);
			}
			image = dataset->getImage(name(dot + 1, 100));
		}
		return image;
	}
	
	hstr Dataset::getTextEntry(chstr textKey)
	{
		return mTexts.tryGet(textKey, "");
	}
	
	hstr Dataset::getText(chstr compositeTextKey)
	{
		return _parseCompositeTextKey(compositeTextKey);
	}
	
	bool Dataset::textExists(chstr textKey)
	{
		return mTexts.hasKey(textKey);
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
		if (mCallbacks.hasKey(name))
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
	
	void Dataset::OnKeyDown(unsigned int keyCode)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnKeyDown(keyCode);
			}
		}
	}
	
	void Dataset::OnKeyUp(unsigned int keyCode)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnKeyUp(keyCode);
			}
		}
	}
	
	void Dataset::OnChar(unsigned int charCode)
	{
		foreach_m (aprilui::Object*, it, mObjects)
		{
			if (it->second->getParent() == NULL)
			{
				it->second->OnChar(charCode);
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
	
	hstr Dataset::_parseCompositeTextKey(chstr key)
	{
		if (!key.startsWith("{"))
		{
			if (!textExists(key))
			{
				aprilui::log(hsprintf("WARNING! Text key '%s' does not exist", key.cStr()));
			}
			return getTextEntry(key);
		}
		int index = key.indexOfAny('}');
		if (index < 0)
		{
			aprilui::log(hsprintf("WARNING! Error while trying to parse formatted key '%s'.", key.cStr()));
			return key;
		}
		harray<hstr> args;
		hstr format = key(1, index - 1);
		hstr argString = key(index + 1, key.size() - index - 1).trimmed(' ');
		if (!_processCompositeTextKeyArgs(argString, args))
		{
			aprilui::log(hsprintf("- while processing args: '%s' with args '%s'.", format.cStr(), argString.cStr()));
			return key;
		}
		hstr preprocessedFormat;
		harray<hstr> preprocessedArgs;
		if (!_preprocessCompositeTextKeyFormat(format, args, preprocessedFormat, preprocessedArgs))
		{
			aprilui::log(hsprintf("- while preprocessing format: '%s' with args '%s'.", format.cStr(), argString.cStr()));
			return key;
		}
		hstr result;
		if (!_processCompositeTextKeyFormat(preprocessedFormat, preprocessedArgs, result))
		{
			aprilui::log(hsprintf("- while processing format: '%s' with args '%s'.", format.cStr(), argString.cStr()));
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
			openIndex = string.indexOfAny('{');
			closeIndex = string.indexOfAny('}');
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
			index = string.indexOfAny('%');
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
				preprocessedArgs += args.removeFirst();
				continue;
			}
			if (string[index + 1] == 'f')
			{
				if (args.size() == 0)
				{
					aprilui::log("WARNING! Not enough args");
					return false;
				}
				hstr arg = args.removeFirst();
				preprocessedFormat += string(0, index) + arg;
				string = string(index + 2, string.size() - index - 2);
				if (!_getCompositeTextKeyFormatIndexes(arg, indexes))
				{
					return false;
				}
				if (indexes.size() > args.size())
				{
					aprilui::log("WARNING! Not enough args");
					return false;
				}
				preprocessedArgs += args.removeFirst(indexes.size());
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
			result += args.removeFirst();
			string = string((*it) + 2, string.size() - (*it) - 2);
		}
		result += string;
		result = result.replaced("%%", "%");
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
			index = string.indexOfAny('%');
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
