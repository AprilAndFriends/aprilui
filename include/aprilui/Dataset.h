/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_DATASET_H
#define APRILUI_DATASET_H

#include <gtypes/Rectangle.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Exception.h"

#include "apriluiExport.h"

namespace april
{
	class Texture;
}

namespace hlxml
{
	struct Node;
}

namespace aprilui
{
	#define REGISTER_CALLBACK(data_dict, fn) data_dict->registerCallback(#fn, fn)

	class Object;
	class Image;

	class apriluiExport Dataset
	{
	public:
		Object* parseObject(hlxml::Node* node, Object* parent = NULL);
		
		Dataset(chstr filename, chstr name = "", bool useNameBasePath = false);
		virtual ~Dataset();

		void load(chstr path = "");
		void unload();
		bool isLoaded() { return mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerManualImage(Image* img);
		void unregisterManualImage(Image* img);
		void registerManualTexture(april::Texture* tex);
		void unregisterManualTexture(april::Texture* tex);

		void registerCallback(chstr name, void (*callback)());
		void triggerCallback(chstr name);
		
		bool isAnimated();
		bool isWaitingAnimation();
		void setFocusedObject(Object* object) { mFocusedObject = object; }
		Object* getFocusedObject() { return mFocusedObject; }
		Object* getRoot() { return mRoot; }
		
		// use these functions only in debug purposes
		hstr _getFilename() { return mFilename; }
		void _setFilename(chstr filename) { mFilename = filename; }
		hstr _getFilePath() { return mFilePath; }
		void _setFilePath(chstr prefix) { mFilePath = prefix; }
		hmap<hstr, Object*>& getObjects() { return mObjects; }
		hmap<hstr, Image*>& getImages() { return mImages; }
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
		void _destroyTexture(april::Texture* tex);
		void _destroyImage(Image* img);
		
		void destroyObject(chstr name, bool recursive = false);
		void destroyObject(Object* object, bool recursive = false);
		void destroyAndDetachObject(chstr name, bool recursive = false);
		void destroyAndDetachObject(Object* object, bool recursive = false);

		bool onMouseDown(float x, float y, int button);
		bool onMouseUp(float x, float y, int button);
		void onMouseMove(float x, float y);
		void onKeyDown(unsigned int keycode);
		void onKeyUp(unsigned int keycode);
		void onChar(unsigned int charcode);
		DEPRECATED_ATTRIBUTE bool OnMouseDown(float x, float y, int button) { return onMouseDown(x, y, button); }
		DEPRECATED_ATTRIBUTE bool OnMouseUp(float x, float y, int button) { return onMouseUp(x, y, button); }
		DEPRECATED_ATTRIBUTE void OnMouseMove(float x, float y) { onMouseMove(x, y); }
		DEPRECATED_ATTRIBUTE void OnKeyDown(unsigned int keycode) { onKeyDown(keycode); }
		DEPRECATED_ATTRIBUTE void OnKeyUp(unsigned int keycode) { onKeyUp(keycode); }
		DEPRECATED_ATTRIBUTE void OnChar(unsigned int charcode) { onChar(charcode); }

		virtual Object* getObject(chstr name);
		virtual april::Texture* getTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual hstr getText(chstr name);
		virtual bool textExists(chstr name);
		hmap<hstr, hstr>& getTexts() { return mTexts; }

		hstr getName() { return mName; }
		
		template <class T> T getObject(chstr name)
		{
			T object = dynamic_cast<T>(getObject(name));
			if (object == NULL)
			{
				aprilui::log("WARNING: Dynamic cast in getObject<T> failed, object: " + name);
			}
			return object;
		}

		template <class T> void getObject(chstr name, T& out)
		{
			out = getObject<T>(name);
		}

	protected:
		hstr mName;
		hstr mFilename;
		hstr mFilePath;
		bool mLoaded;
		Object* mFocusedObject;
		Object* mRoot;
		hmap<hstr, Object*> mObjects;
		hmap<hstr, april::Texture*> mTextures;
		hmap<hstr, Image*> mImages;
		hmap<hstr, hstr> mTexts;

		hmap<hstr, void (*)()> mCallbacks;

		void parseTexture(hlxml::Node* node);
		void parseRAMTexture(hlxml::Node* node);
		void parseTextureGroup(hlxml::Node* node);
		void parseCompositeImage(hlxml::Node* node);
		void parseGlobalInclude(chstr path);
		void parseObjectInclude(chstr path, Object* parent);
		void parseObjectIncludeFile(chstr filename, Object* parent);
		virtual void parseExternalXMLNode(hlxml::Node* node) { }
		virtual Object* parseExternalObjectClass(hlxml::Node* node, chstr obj_name, grect rect) { return 0; }
		
		Object* recursiveObjectParse(hlxml::Node* node, Object* parent);
		
		void readFile(chstr filename);
		void _loadTexts(chstr path);
		hstr _makeFilePath(chstr filename, chstr name = "", bool useNameBasePath = false);
		
	};

}
#endif
