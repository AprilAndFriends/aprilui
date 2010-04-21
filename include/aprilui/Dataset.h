/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef DATASET_H
#define DATASET_H

#include <map>
#include "hltypes/hstring.h"
#include "Exception.h"
#include "TextMap.h"
#include "AprilUIExport.h"

namespace April
{
	class Texture;
}

struct _xmlNode;
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
		std::map<hstr,Object*> mObjects;
		std::map<hstr,April::Texture*> mTextures;
		std::map<hstr,Image*> mImages;

		std::map<hstr,void (*)()> mCallbacks;

		April::Texture* parseTexture(_xmlNode* node);
		void parseRAMTexture(_xmlNode* node);
		void parseCompositeImage(_xmlNode* node);
		virtual void parseExternalXMLNode(_xmlNode* node);
		virtual Object* parseExternalObjectClass(_xmlNode* node,chstr obj_name,float x,float y,float w,float h);
		
		Object* recursiveObjectParse(_xmlNode* node,Object* parent);
		
		void readFile(hstr filename);
	public:
		Object* parseObject(_xmlNode* node);

		TextMap texts;
		
		Dataset(hstr filename);
		virtual ~Dataset();

		void load();
		void unload();
		bool isLoaded() { return mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerManualImage(Image* img);
		void unregisterManualImage(Image* img);

		void registerCallback(hstr name,void (*callback)());
		void triggerCallback(hstr name);

		

		virtual void update(float k);
		
		void _destroyTexture(hstr tex);
		void _destroyImage(hstr img);
		void _destroyTexture(April::Texture* tex);
		void _destroyImage(Image* img);

		Object* getObject(hstr name);
		April::Texture* getTexture(hstr name);
		Image* getImage(hstr name);

		
		chstr getName();
	};

}
#endif
