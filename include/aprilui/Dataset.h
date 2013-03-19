/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.55
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
#include <hltypes/hlog.h>
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
		Object* getFocusedObject() { return this->mFocusedObject; }
		Object* getRoot() { return this->mRoot; }
		void setRoot(Object* obj) { this->mRoot = obj; }
		hstr getTextsPath() { return this->mTextsPath; }
		void setTextsPath(chstr value) { this->mTextsPath = value; }
		hstr getFilePath() { return this->mFilePath; }

		int getFocusedObjectIndex();
		bool trySetFocusedObjectByIndex(int value, bool strict = false);
		harray<int> findPossibleFocusIndices(bool strict = false);
		harray<int> findAllFocusIndices();
		
		hmap<hstr, Object*>& getObjects() { return this->mObjects; }
		hmap<hstr, Image*>& getImages() { return this->mImages; }
		hmap<hstr, Texture*>& getTextures() { return this->mTextures; }
		template <class T>
		hmap<hstr, T> getObjectsByType()
		{
			return this->mObjects.dyn_cast_value<hstr, T>();
		}
		
		void updateTextures(float k);
		void unloadUnusedTextures();
		virtual void update(float k);
		void draw();
		
		void _destroyTexture(chstr tex);
		void _destroyImage(chstr img);
		void _destroyTexture(Texture* tex);
		void _destroyImage(Image* img);
		
		void destroyObject(chstr name);
		void destroyObject(Object* object);
		
		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onMouseMove();
		bool onMouseScroll(float x, float y);
		bool onKeyDown(april::Key keyCode);
		bool onKeyUp(april::Key keyCode);
		bool onChar(unsigned int charCode);
		
		virtual Texture* getTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual hstr getTextEntry(chstr textKey);
		virtual bool hasTextEntry(chstr textKey);
		virtual hstr getText(chstr compositeTextKey);
		hmap<hstr, hstr>& getTextEntries() { return this->mTexts; }
		harray<hstr> getTextEntries(harray<hstr> keys);
		
		hstr getName() { return this->mName; }
		
		virtual Object* getObject(chstr name);
		bool hasImage(chstr name);
		bool hasTexture(chstr name);
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
				hlog::warn(aprilui::logTag, "Dynamic cast in getObject<T> failed, object: " + name);
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
		void focus(Object* object);
		void removeFocus();
		
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
		void parseObjectInclude(chstr path, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset);
		void parseObjectIncludeFile(chstr filename, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset);
		virtual void parseExternalXMLNode(hlxml::Node* node) { }
		virtual Object* parseExternalObjectClass(hlxml::Node* node, chstr objName, grect rect) { return 0; }
		
		Object* recursiveObjectParse(hlxml::Node* node, Object* parent);
		Object* recursiveObjectParse(hlxml::Node* node, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset);
		
		void readFile(chstr filename);
		void _loadTexts(chstr path);
		hstr _makeFilePath(chstr filename, chstr name = "", bool useNameBasePath = false);
		hstr _makeLocalizedTextureName(chstr filename);
		hstr _makeTextsPath();
		bool _findTextEntry(chstr textKey, hstr* text);
		
		typedef std::basic_string<unsigned int> ustr;
		
		hstr _parseCompositeTextKey(chstr key);
		bool _processCompositeTextKeyArgs(ustr argString, harray<ustr>& args);
		bool _preprocessCompositeTextKeyFormat(ustr format, harray<ustr> args, ustr& preprocessedFormat, harray<ustr>& preprocessedArgs);
		bool _processCompositeTextKeyFormat(ustr format, harray<ustr> args, hstr& result);
		/// @note The returned indexes count the positions relative to the last format tag (minus the 2 characters of the format tag itself), not from the beginning of the string
		bool _getCompositeTextKeyFormatIndexes(ustr format, harray<int>& indexes);
		harray<ustr> _getArgEntries(ustr string);
		
	};

}
#endif
