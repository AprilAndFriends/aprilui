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

#include "Exception.h"

#include "apriluiExport.h"

namespace april
{
	class Texture;
}

struct xml_node;

namespace aprilui
{
	#define REGISTER_CALLBACK(data_dict, fn) data_dict->registerCallback(#fn, fn)

	class Object;
	class Image;

	class apriluiExport Dataset
	{
	public:
		Object* parseObject(xml_node* node, Object* parent = NULL);
		
		Dataset(chstr filename, chstr name = "");
		virtual ~Dataset();

		void load(chstr path = "");
		void unload();
		bool isLoaded() { return mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerManualImage(Image* img);
		void unregisterManualImage(Image* img);

		void registerCallback(chstr name, void (*callback)());
		void triggerCallback(chstr name);
		
		bool isAnimated();
		void setFocusedObject(Object* object) { mFocusedObject = object; }
		Object* getFocusedObject() { return mFocusedObject; }
		
		// use these functions only in debug purposes
		void _setFilename(chstr filename) { mFilename = filename; }
		void _setFilenamePrefix(chstr prefix) { mFilenamePrefix = prefix; }
		hstr _getFilename() { return mFilename; }
		hstr _getFilenamePrefix() { return mFilenamePrefix; }
		hmap<hstr, Object*>& getObjects() { return mObjects; }

		void updateTextures(float k);
		virtual void update(float k);
		void draw();
		
		void _destroyTexture(chstr tex);
		void _destroyImage(chstr img);
		void _destroyTexture(april::Texture* tex);
		void _destroyImage(Image* img);

		void OnMouseDown(float x, float y, int button);
		void OnMouseUp(float x, float y, int button);
		void OnMouseMove(float x, float y);
		void OnKeyDown(unsigned int keycode);
		void OnKeyUp(unsigned int keycode);
		void OnChar(unsigned int charcode);
		
		virtual Object* getObject(chstr name);
		virtual april::Texture* getTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual hstr getText(chstr name);
		virtual bool textExists(chstr name);
		hmap<hstr, hstr>& getTexts() { return mTexts; }

		hstr getName() { return mName; }
		
		template <class T> T getObject(chstr name)
		{
			return dynamic_cast<T>(getObject(name));
		}
		
	protected:
		hstr mName;
		hstr mFilename;
		hstr mFilenamePrefix;
		bool mLoaded;
		Object* mFocusedObject;
		hmap<hstr, Object*> mObjects;
		hmap<hstr, april::Texture*> mTextures;
		hmap<hstr, Image*> mImages;
		hmap<hstr, hstr> mTexts;

		hmap<hstr, void (*)()> mCallbacks;

		april::Texture* parseTexture(xml_node* node);
		void parseRAMTexture(xml_node* node);
		void parseCompositeImage(xml_node* node);
		virtual void parseExternalXMLNode(xml_node* node) { }
		virtual Object* parseExternalObjectClass(xml_node* node, chstr obj_name, grect rect) { return 0; }
		
		Object* recursiveObjectParse(xml_node* node, Object* parent);
		
		void readFile(chstr filename);
		void _loadTexts(chstr path);
		
	};

}
#endif
