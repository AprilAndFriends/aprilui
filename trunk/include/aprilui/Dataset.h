/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a dataset.

#ifndef APRILUI_DATASET_H
#define APRILUI_DATASET_H

#include <gtypes/Rectangle.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiExport.h"
#include "EventReceiver.h"
#include "Exception.h"

namespace hlxml
{
	class Node;
}

namespace aprilui
{
	#define REGISTER_CALLBACK(dataDict, fn) dataDict->registerCallback(#fn, fn)

	class Object;
	class Image;
	class Texture;
	class NullImage;

	class apriluiExport Dataset : public EventReceiver
	{
	public:
		Object* parseObject(hlxml::Node* node, Object* parent = NULL);
		
		Dataset(chstr filename, chstr name = "", bool useNameBasePath = false);
		~Dataset();

		void load();
		void unload();
		bool isLoaded() { return this->mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerManualImage(Image* img);
		void unregisterManualImage(Image* img);
		void registerManualTexture(Texture* tex);
		void unregisterManualTexture(Texture* tex);

		void registerCallback(chstr name, void (*callback)());
		void triggerCallback(chstr name);
		
		bool isAnimated();
		bool isWaitingAnimation();
		void setFocusedObject(Object* object) { this->mFocusedObject = object; }
		Object* getFocusedObject() { return this->mFocusedObject; }
		Object* getRoot() { return this->mRoot; }
		void setRoot(Object* obj) { this->mRoot = obj; }
		hstr getTextsPath() { return this->mTextsPath; }
		void setTextsPath(chstr value) { this->mTextsPath = value; }
		
		// use these functions only in debug purposes
		hstr _getFilename() { return this->mFilename; }
		void _setFilename(chstr filename) { this->mFilename = filename; }
		hstr _getFilePath() { return this->mFilePath; }
		void _setFilePath(chstr prefix) { this->mFilePath = prefix; }
		hmap<hstr, Object*>& getObjects() { return this->mObjects; }
		hmap<hstr, Image*>& getImages() { return this->mImages; }
		template <class T>
		hmap<hstr, T> getObjectsByType()
		{
			return mObjects.dyn_cast_value<hstr, T>();
		}

		void updateTextures(float k);
		void unloadUnusedTextures();
		virtual void update(float k);
		void draw();
		
		void _destroyTexture(chstr tex);
		void _destroyImage(chstr img);
		void _destroyTexture(Texture* tex);
		void _destroyImage(Image* img);
		
		void destroyObject(chstr name, bool recursive = false);
		void destroyObject(Object* object, bool recursive = false);

		bool onMouseDown(int button);
		bool onMouseUp(int button);
		void onMouseMove();
		void onMouseScroll(float x, float y);
		void onKeyDown(unsigned int keycode);
		void onKeyUp(unsigned int keycode);
		void onChar(unsigned int charcode);

		virtual Texture* getTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual hstr getTextEntry(chstr textKey);
		virtual bool hasTextEntry(chstr textKey);
		virtual hstr getText(chstr compositeTextKey);
		hmap<hstr, hstr>& getTextEntries() { return mTexts; }
		harray<hstr> getTextEntries(harray<hstr> keys);

		hstr getName() { return this->mName; }

		virtual Object* getObject(chstr name);
		bool hasImage(chstr name);
		bool hasObject(chstr name);
		Object* tryGetObject(chstr name);

		template <class T> T getObject(chstr name)
		{
			T object = dynamic_cast<T>(this->getObject(name));
			if (object == NULL)
			{
				throw _InvalidObjectTypeCast(hsprintf("Object '%s' found in dataset '%s' but dynamic cast failed.", name.c_str(), this->getName().c_str()));
			}
			return object;
		}

		template <class T> void getObject(chstr name, T& out)
		{
			out = this->getObject<T>(name);
		}
		
		template <class T> T tryGetObject(chstr name)
		{
			T object = dynamic_cast<T>(this->tryGetObject(name));
			if (object == NULL)
			{
				aprilui::log("WARNING: Dynamic cast in getObject<T> failed, object: " + name);
			}
			return object;
		}
		
		template <class T> void tryGetObject(chstr name, T& out)
		{
			out = this->tryGetObject<T>(name);
		}

		void notifyEvent(chstr name, void* params);
		void reloadTexts();
		void reloadTextures();
		void removeFocus();

		DEPRECATED_ATTRIBUTE void destroyAndDetachObject(chstr name, bool recursive = false) { this->destroyObject(name, recursive); }
		DEPRECATED_ATTRIBUTE void destroyAndDetachObject(Object* object, bool recursive = false) { this->destroyObject(object, recursive); }
		DEPRECATED_ATTRIBUTE bool OnMouseDown(float x, float y, int button) { return this->onMouseDown(button); }
		DEPRECATED_ATTRIBUTE bool OnMouseUp(float x, float y, int button) { return this->onMouseUp(button); }
		DEPRECATED_ATTRIBUTE void OnMouseMove(float x, float y) { this->onMouseMove(); }
		DEPRECATED_ATTRIBUTE void OnKeyDown(unsigned int keycode) { this->onKeyDown(keycode); }
		DEPRECATED_ATTRIBUTE void OnKeyUp(unsigned int keycode) { this->onKeyUp(keycode); }
		DEPRECATED_ATTRIBUTE void OnChar(unsigned int charcode) { this->onChar(charcode); }
		DEPRECATED_ATTRIBUTE bool onMouseDown(float x, float y, int button) { return this->onMouseDown(button); }
		DEPRECATED_ATTRIBUTE bool onMouseUp(float x, float y, int button) { return this->onMouseUp(button); }
		DEPRECATED_ATTRIBUTE void onMouseMove(float x, float y) { this->onMouseMove(); }
		DEPRECATED_ATTRIBUTE bool textExists(chstr key) { return this->hasTextEntry(key); }
		DEPRECATED_ATTRIBUTE bool hasTextKey(chstr key) { return this->hasTextEntry(key); }
		DEPRECATED_ATTRIBUTE hmap<hstr, hstr>& getTexts() { return this->getTextEntries(); }

	protected:
		hstr mName;
		hstr mFilename;
		hstr mFilePath;
		hstr mTextsPath;
		bool mLoaded;
		Object* mFocusedObject;
		Object* mRoot;
		hmap<hstr, Object*> mObjects;
		hmap<hstr, Texture*> mTextures;
		hmap<hstr, Image*> mImages;
		hmap<hstr, hstr> mTexts;
		NullImage* mNullImage;

		hmap<hstr, void (*)()> mCallbacks;

		void parseTexture(hlxml::Node* node);
		void parseRamTexture(hlxml::Node* node);
		void parseTextureGroup(hlxml::Node* node);
		void parseCompositeImage(hlxml::Node* node);
		void parseGlobalInclude(chstr path);
		void parseObjectInclude(chstr path, Object* parent);
		void parseObjectIncludeFile(chstr filename, Object* parent);
		virtual void parseExternalXMLNode(hlxml::Node* node) { }
		virtual Object* parseExternalObjectClass(hlxml::Node* node, chstr objName, grect rect) { return 0; }
		
		Object* recursiveObjectParse(hlxml::Node* node, Object* parent);
		
		void readFile(chstr filename);
		void _loadTexts(chstr path);
		hstr _makeFilePath(chstr filename, chstr name = "", bool useNameBasePath = false);
		hstr _makeLocalizedTextureName(chstr filename);
		hstr _makeTextsPath();

		hstr _parseCompositeTextKey(chstr key);
		bool _processCompositeTextKeyArgs(chstr argString, harray<hstr>& args);
		bool _preprocessCompositeTextKeyFormat(chstr format, harray<hstr> args, hstr& preprocessedFormat, harray<hstr>& preprocessedArgs);
		bool _processCompositeTextKeyFormat(chstr format, harray<hstr> args, hstr& result);
		/// @note The returned indexes count the positions relative to the last format tag (minus the 2 characters of the format tag itself), not from the beginning of the string
		bool _getCompositeTextKeyFormatIndexes(chstr format, harray<int>& indexes);

		
	};

}
#endif
