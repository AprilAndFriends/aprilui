/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_DATASET_H
#define APRILUI_DATASET_H

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "Exception.h"
#include "TextMap.h"

namespace April
{
	class Texture;
}

struct xml_node;

namespace AprilUI
{
	#define REGISTER_CALLBACK(data_dict,fn) data_dict->registerCallback(#fn,fn)

	class Object;
	class Image;


	class AprilUIExport Dataset
	{
	protected:
		hstr mName;
		hstr mFilename;
		hstr mFilenamePrefix;
		bool mLoaded;
		Object* mFocusedObject;
		hmap<hstr,Object*> mObjects;
		hmap<hstr,April::Texture*> mTextures;
		hmap<hstr,Image*> mImages;
		TextMap mTexts;

		hmap<hstr,void (*)()> mCallbacks;

		April::Texture* parseTexture(xml_node* node);
		void parseRAMTexture(xml_node* node);
		void parseCompositeImage(xml_node* node);
		virtual void parseExternalXMLNode(xml_node* node) {};
		virtual Object* parseExternalObjectClass(xml_node* node,chstr obj_name,float x,float y,float w,float h) { return 0; };
		
		Object* recursiveObjectParse(xml_node* node,Object* parent);
		
		void readFile(chstr filename);
	public:
		Object* parseObject(xml_node* node,Object* parent=0);
		
		Dataset(chstr filename,chstr name_override="");
		virtual ~Dataset();

		void load(chstr path="");
		void unload();
		bool isLoaded() { return mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerManualImage(Image* img);
		void unregisterManualImage(Image* img);

		void registerCallback(chstr name,void (*callback)());
		void triggerCallback(chstr name);

		void setFocusedObject(Object* object) { mFocusedObject=object; }
		Object* getFocusedObject() { return mFocusedObject; }
		
		// use these functions only in debug purposes
		void _setFilename(chstr filename) { mFilename=filename; }
		void _setFilenamePrefix(chstr prefix) { mFilenamePrefix=prefix; }
		hstr _getFilename() { return mFilename; }
		hstr _getFilenamePrefix() { return mFilenamePrefix; }
		hmap<hstr,Object*>& getObjects() { return mObjects; }

		virtual void update(float k);
		
		void _destroyTexture(chstr tex);
		void _destroyImage(chstr img);
		void _destroyTexture(April::Texture* tex);
		void _destroyImage(Image* img);

		virtual Object* getObject(chstr name);
		virtual April::Texture* getTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual const char* getText(chstr name);
		virtual bool textExists(chstr name);
		TextMap& getTextmap() { return mTexts; }

		
		hstr getName();
		
	};

}
#endif
