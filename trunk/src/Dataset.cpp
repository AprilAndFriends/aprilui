/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <xmlHelper.h>
#include "AprilUI.h"
#include "Dataset.h"
#include "Exception.h"
#include "Objects.h"
#include "Animators.h"
#include "april/RenderSystem.h"
#include "Image.h"
#include "Util.h"
#include "hltypes/harray.h"

namespace AprilUI
{
	void _registerDataset(chstr name,Dataset* d);
	void _unregisterDataset(chstr name,Dataset* d);
	
	NullImage g_null_img;

	Dataset::Dataset(chstr filename)
	{
		
		int slash=filename.rfind("/");
		int dot=filename.rfind(".");
		mFilenamePrefix=filename(0,slash);
		mFilename=filename;
		mName=filename(slash+1,dot-slash-1);
		mLoaded=0;
		mObjectIterator=mObjects.end();
		mTextureIterator=mTextures.end();
		mImageIterator=mImages.end();

		_registerDataset(mName,this);
	}
	
	Dataset::~Dataset()
	{
		if (isLoaded()) unload();
		_unregisterDataset(mName,this);
	}

	hstr Dataset::getName()
	{
		return mName;
	}

	void Dataset::_destroyTexture(April::Texture* tex)
	{
		if (mTextures.find(tex->getFilename()) == mTextures.end())
			throw ResourceNotExistsException(tex->getFilename(),"Texture",this);
		mTextures.erase(tex->getFilename());
		delete tex;
	}

	void Dataset::_destroyImage(Image* img)
	{
		if (mImages.find(img->getName()) == mImages.end())
			throw ResourceNotExistsException(img->getName(),"Image",this);
		mImages.erase(img->getName());
		delete img;
	}

	void Dataset::_destroyTexture(chstr tex)
	{
		if (mTextures.find(tex) == mTextures.end())
			throw ResourceNotExistsException(tex,"April::Texture",this);
		April::Texture* i=mTextures[tex];
		mTextures.erase(tex);
		delete i;
	}

	void Dataset::_destroyImage(chstr img)
	{
		if (mImages.find(img) == mImages.end())
			throw ResourceNotExistsException(img,"Image",this);
		Image* i=mImages[img];
		mImages.erase(img);
		delete i;
	}

	April::Texture* Dataset::parseTexture(xml_node* node)
	{
		hstr filename=node->pstr("filename");
		int slash=filename.find("/")+1;
		hstr tex_name=filename(slash,filename.rfind(".")-slash);
		if (mTextures.find(tex_name) != mTextures.end()) throw ObjectExistsException(filename);

		bool prefix_images=true,dynamic_load=0;
		try { prefix_images=node->pint("prefix_images")!=0; } catch (_XMLException) { }
		try { dynamic_load=node->pint("dynamic_load")!=0; }
		catch (_XMLException) { }
		
		April::Texture* t=rendersys->loadTexture(mFilenamePrefix+"/"+filename,dynamic_load);
		if (!t) throw FileNotFoundException(mFilenamePrefix+"/"+filename);
		mTextures[tex_name]=t;
		// extract image definitions
		if (node->iter_children() == 0) // if there are no images defined, create one that fills the whole area
		{
			if (mImages.find(tex_name) != mImages.end()) throw ResourceExistsException(filename,"April::Texture",this);	
			mImages[tex_name]=new Image(t,filename,0,0,(float)t->getWidth(),(float)t->getHeight());
		}
		else
		{
			Image* i;
			for (node = node->iter_children(); node != 0; node=node->next())
			{
				if (*node == "Image")
				{
					hstr name;
					if (prefix_images) name=tex_name+"/"+node->pstr("name");
					else               name=node->pstr("name");
					if (mImages.find(name) != mImages.end()) throw ResourceExistsException(name,"Image",this);
					float x=node->pfloat("x"), y=node->pfloat("y"),
						  w=node->pfloat("w"), h=node->pfloat("h");
					
					bool vertical=false,invertx=false,inverty=false;
					float tile_w=1,tile_h=1;
					try { vertical=node->pint("vertical")!=0; } catch (_XMLException) { }
					try { invertx=node->pint("invertx")!=0; } catch (_XMLException) { }
					try { inverty=node->pint("inverty")!=0; } catch (_XMLException) { }
					
					try   { tile_w=node->pfloat("tile_w"); }
					catch (_XMLException) { }
					try   { tile_h=node->pfloat("tile_h"); }
					catch (_XMLException) { }
					
					if (tile_w != 1 || tile_h != 1) i=new TiledImage(t,name,x,y,w,h,vertical,tile_w,tile_h);
					else
					{
						try
						{
							unsigned int color=node->phex("color");
							i=new ColoredImage(t,name,x,y,w,h,vertical,color);
						}
						catch (_XMLException)
						{
							i=new Image(t,name,x,y,w,h,vertical,invertx,inverty);    
						}
					}
					try
					{
						hstr mode=node->pstr("blend_mode");
						if (mode == "add") i->setBlendMode(April::ADD);
					}
					catch (_XMLException) { }
					

					
					mImages[name]=i;
				}
		    }
		}
		return t;
		
	}

	void Dataset::parseRAMTexture(xml_node* node)
	{
		hstr filename=node->pstr("filename");
		int slash=filename.find("/")+1;
		hstr tex_name=filename(slash,filename.rfind(".")-slash);
		if (mTextures.find(tex_name) != mTextures.end()) throw ResourceExistsException(filename,"RAMTexture",this);

		bool dynamic_load=false;
		try   { dynamic_load=node->pint("dynamic_load")!=0; }
		catch (_XMLException) { }
		
		April::Texture* t=rendersys->loadRAMTexture(mFilenamePrefix+"/"+filename,dynamic_load);
		if (!t) throw FileNotFoundException(mFilenamePrefix+"/"+filename);
		mTextures[tex_name]=t;
	
	}
	
	void Dataset::parseCompositeImage(xml_node* node)
	{
		hstr name=node->pstr("name"),refname;
		if (mImages.find(name) != mImages.end()) throw ResourceExistsException(name,"CompositeImage",this);

		CompositeImage* img=new CompositeImage(name,node->pfloat("w"),node->pfloat("h"));
		
		for (node = node->iter_children(); node != 0; node=node->next())
		{
			if (*node == "ImageRef")
			{
				refname=node->pstr("name");
				img->addImageRef(getImage(refname),node->pfloat("x"),node->pfloat("y"),
				                                   node->pfloat("w"),node->pfloat("h"));
			}
		}
		
		mImages[name]=img;
	}


	Object* Dataset::parseObject(xml_node* node)
	{
		return recursiveObjectParse(node,0);
	}

	Object* Dataset::recursiveObjectParse(xml_node* node,Object* parent)
	{
		hstr obj_name;
		float x=0,y=0,w=1,h=1;

		hstr class_name=node->pstr("type");

		if (*node == "Object")
		{
			try { obj_name=node->pstr("name"); }
			catch (_XMLException)
			{
				obj_name=generateName(class_name);
				xmlSetProp(node,(xmlChar*) "name",(xmlChar*) obj_name.c_str());
			}
			x=node->pfloat("x");
			y=node->pfloat("y");

			try { w=node->pfloat("w"); }
			catch (_XMLException) { w=-1; }
			try { h=node->pfloat("h"); }
			catch (_XMLException) { h=-1; }
		}
		else if (*node == "Animator")
		{
			try { obj_name=node->pstr("name"); }
			catch (_XMLException)
			{
				obj_name=generateName("Animator");
			}
		}
		else return 0;

		
		if (mObjects.find(obj_name) != mObjects.end())
			throw ResourceExistsException(obj_name,"Object",this);

		Object* o;
		


	#define parse(cls) if (class_name ==  #cls) o=new cls(obj_name,x,y,w,h)
	#define parse_animator(cls) if (class_name ==  #cls) o=new Animators::cls(obj_name)
		
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
		else  parse(RotationImageBox);
		else  parse(RotatableImageBox);
		else if (*node == "Animator")
		{
			/*if*/parse_animator(Mover);
			else  parse_animator(Scaler);
			else  parse_animator(Rotator);
			else  parse_animator(ColorAlternator);
			else  parse_animator(AlphaFader);
			else  parse_animator(AlphaOscillator);
			else  parse_animator(AlphaHover);
			else  parse_animator(Blinker);
			else  parse_animator(FrameAnimation);
			else  parse_animator(Earthquake);
			else o=parseExternalObjectClass(node,obj_name,x,y,w,h);
		}
		else o=parseExternalObjectClass(node,obj_name,x,y,w,h);
		
		if (!o) throw XMLUnknownClassException(class_name,node);
		o->_setDataset(this);
		
		
		for (xml_prop* prop=node->iter_properties();prop != 0; prop=prop->next())
			o->setProperty(prop->name(),prop->value());
		
		mObjects[obj_name]=o;
		if (parent) parent->addChild(o);
		
		for (node = node->iter_children(); node != 0; node=node->next())
			if (node->type !=  XML_TEXT_NODE && node->type != XML_COMMENT_NODE)
			{
				if (*node == "Property")
					o->setProperty(node->pstr("name"),node->pstr("value"));
				else 
					recursiveObjectParse(node,o);
			}
		return o;
	}

	void Dataset::readFile(chstr filename)
	{
		// parse datadef xml file, error checking first
		xml_doc doc(getPWD()+"/"+filename);
		xml_node* cur=doc.root("DataDefinition");

		parseExternalXMLNode(cur);
		
		std::map<April::Texture*,hstr> dynamic_links;
		hstr links;
		for (xml_node* p = cur->iter_children(); p != 0; p=p->next())
		{
			if      (*p == "Texture")
			{
				April::Texture* t=parseTexture(p);
				try
				{
					links=p->pstr("dynamic_link");
					dynamic_links[t]=links;
				}
				catch (_XMLException) { }
				
			}
			else if (*p == "RAMTexture") parseRAMTexture(p);
			else if (*p == "CompositeImage") parseCompositeImage(p);
			else if (*p == "Object") parseObject(p);
			else if (p->type !=  XML_TEXT_NODE && p->type != XML_COMMENT_NODE)
				     parseExternalXMLNode(p);
		}
		
	// adjust dynamic texture links
		std::map<April::Texture*,hstr>::iterator map_it;
		harray<hstr> dlst;
		for (map_it = dynamic_links.begin();map_it != dynamic_links.end();map_it++)
		{
			dlst=map_it->second.split(',');
			foreach(hstr,dlst)
				map_it->first->addDynamicLink(getTexture(*it));
		}
	}

	void Dataset::load()
	{
		hstr base_dir=pathGetBaseDir(mFilename);
		harray<hstr> scenes;

		// texts
		writelog("loading texts");
		texts.load(mFilenamePrefix+"/texts");
		
		// audio
		mLoaded=1;

		writelog("loading datadef: "+mFilename);
		readFile(mFilename);
	}

	void Dataset::unload()
	{
		if (!mLoaded) throw GenericException("Unable to unload dataset '"+getName()+"', data not loaded!");

		foreach_in_map(Object*,mObjects)          delete it->second; mObjects.clear();
		foreach_in_map(Image*,mImages)            delete it->second; mImages.clear();
		foreach_in_map(April::Texture*,mTextures) delete it->second; mTextures.clear();
		mCallbacks.clear();
		
		mLoaded=0;
	}

	void Dataset::registerManualObject(Object* o)
	{
		if (mObjects.find(o->getName()) != mObjects.end()) throw ResourceExistsException(o->getName(),"Object",this);
		mObjects[o->getName()]=o;
		o->_setDataset(this);
	}

	void Dataset::unregisterManualObject(Object* o)
	{
		if (mObjects.find(o->getName()) == mObjects.end()) throw ResourceNotExistsException(o->getName(),"Object",this);
		mObjects.erase(o->getName());
		o->_setDataset(NULL);
	}
	
	void Dataset::registerManualImage(Image* img)
	{
		if (mImages.find(img->getName()) != mImages.end()) throw ResourceExistsException(img->getName(),"Image",this);
		mImages[img->getName()]=img;
	}
	
	void Dataset::unregisterManualImage(Image* img)
	{
		if (mImages.find(img->getName()) == mImages.end()) throw ResourceNotExistsException(img->getName(),"Image",this);
		mImages.erase(img->getName());
	}

	Object* Dataset::getObject(chstr name)
	{
		Object* o=mObjects[name];
		if (!o)
			throw ResourceNotExistsException(name,"Object",this);
		return o;
	}
	
	April::Texture* Dataset::getTexture(chstr name)
	{
		if (mTextures.find(name) == mTextures.end()) throw ResourceNotExistsException(name,"Texture",this);
		return mTextures[name];
	}

	Image* Dataset::getImage(chstr name)
	{
		Image* i;
		if (name == "null") return &g_null_img;

		
		if (mImages.find(name) == mImages.end() && name.starts_with("0x")) // create new image with a color. don't overuse this,it's meant to be handy when needed only ;)
			i=mImages[name]=new ColorImage(name);
		else
			i=mImages[name];

		if (!i)
		{
			int dot=name.find(".");
			if (dot > -1)
			{
				
				Dataset* d;
				try { d=getDatasetByName(name(0,dot)); }
				catch (_GenericException) { throw ResourceNotExistsException(name,"Image",this); }
				return d->getImage(name(dot+1,100));
			}
			else throw ResourceNotExistsException(name,"Image",this);
		}
		
			
		return i;
	}

	void Dataset::registerCallback(chstr name,void (*callback)())
	{
		mCallbacks[name]=callback;
	}

	void Dataset::triggerCallback(chstr name)
	{
		void (*callback)()=mCallbacks[name];
		if (callback) callback();
	}

	void Dataset::update(float k)
	{
		foreach_in_map(April::Texture*,mTextures)
			it->second->update(k);
	}
	
	Object** Dataset::iterateObjects()
	{
		mObjectIterator=mObjects.begin();
		if (mObjectIterator==mObjects.end())
			return NULL;
		return &mObjectIterator->second;
	}
	
	Object** Dataset::nextObject()
	{
		mObjectIterator++;
		if (mObjectIterator==mObjects.end())
			return NULL;
		return &mObjectIterator->second;
	}
	
	April::Texture** Dataset::iterateTextures()
	{
		mTextureIterator=mTextures.begin();
		if (mTextureIterator==mTextures.end())
			return NULL;
		return &mTextureIterator->second;
	}
	
	April::Texture** Dataset::nextTexture()
	{
		mTextureIterator++;
		if (mTextureIterator==mTextures.end())
			return NULL;
		return &mTextureIterator->second;
	}
	
	Image** Dataset::iterateImages()
	{
		mImageIterator=mImages.begin();
		if (mImageIterator==mImages.end())
			return NULL;
		return &mImageIterator->second;
	}
	
	Image** Dataset::nextImage()
	{
		mImageIterator++;
		if (mImageIterator==mImages.end())
			return NULL;
		return &mImageIterator->second;
	}
}
