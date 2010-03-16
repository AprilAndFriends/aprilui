/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#ifndef DATASET_H
#define DATASET_H

#include <map>
#include <string>
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
		std::string mName;
		std::string mFilename;
		std::string mFilenamePrefix;
		bool mLoaded;
		std::map<std::string,Object*> mObjects;
		std::map<std::string,April::Texture*> mTextures;
		std::map<std::string,Image*> mImages;

		std::map<std::string,void (*)()> mCallbacks;

		void parseTexture(_xmlNode* node);
		void parseRAMTexture(_xmlNode* node);
		void parseCompositeImage(_xmlNode* node);
		virtual void parseExternalXMLNode(_xmlNode* node);
		virtual Object* parseExternalObjectClass(_xmlNode* node,std::string obj_name,float x,float y,float w,float h);
		
		Object* recursiveObjectParse(_xmlNode* node,Object* parent);
		
		void readFile(std::string filename);
	public:
		Object* parseObject(_xmlNode* node);

		TextMap texts;
		
		Dataset(std::string filename);
		virtual ~Dataset();

		void load();
		void unload();
		bool isLoaded() { return mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);

		void registerCallback(std::string name,void (*callback)());
		void triggerCallback(std::string name);

		

		virtual void update(float k);
		
		void _destroyTexture(std::string tex);
		void _destroyImage(std::string img);
		void _destroyTexture(April::Texture* tex);
		void _destroyImage(Image* img);

		Object* getObject(std::string name);
		April::Texture* getTexture(std::string name);
		Image* getImage(std::string name);

		
		std::string getName();
	};

}
#endif
