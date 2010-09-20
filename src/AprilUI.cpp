/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <stdio.h>

#include <april/RenderSystem.h>
#include <atres/Atres.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "AprilUI.h"
#include "AtresRenderInterface.h"
#include "Dataset.h"
#include "Exception.h"

namespace AprilUI
{
	bool register_lock=0;
	hmap<int,April::Texture*> g_font_textures;
	AtresAprilInterface* atres_render_iface=0;
	hmap<hstr,Dataset*> g_datasets;
	float default_scale=1;
	gvec2 cursor_pos;
	void (*g_logFunction)(chstr)=aprilui_writelog;
	
	void logMessage(chstr message, chstr prefix)
	{
		g_logFunction(prefix + message);
	}
	
	void aprilui_writelog(chstr message)
	{
		printf("%s\n", message.c_str());		
	}
	
	void setLogFunction(void (*fnptr)(chstr))
	{
		g_logFunction=fnptr;
	}
	
	void setCursorPos(float x,float y)
	{
		cursor_pos.set(x,y);
	}
	
	gvec2 getCursorPos()
	{
		return cursor_pos;
	}

	void init()
	{
		atres_render_iface=new AtresAprilInterface();
		Atres::setRenderInterface(atres_render_iface);
	}

	Dataset* getDatasetByName(chstr name)
	{
		if (g_datasets.find(name) == g_datasets.end())
			throw GenericException("Dataset '"+name+"' doesn't exist!");
		return g_datasets[name];
	}

	void _registerDataset(chstr name,Dataset* d)
	{
		if (register_lock) return;
		if (g_datasets.find(name) != g_datasets.end())
			throw GenericException("Unable to register dataset '"+name+"', another dataset with the same name exists!");	
		g_datasets[name]=d;
	}
	
	void _unregisterDataset(chstr name,Dataset* d)
	{
		if (register_lock) return;
		g_datasets.erase(name);
	}
	
	void update(float time_increase)
	{
		for (hmap<hstr,Dataset*>::iterator it2=g_datasets.begin();it2!=g_datasets.end();it2++)
			it2->second->update(time_increase);
	}

	void setDefaultScale(float value)
	{
		default_scale=value;
	}
	
	float getDefaultScale()
	{
		return default_scale;
	}

	void destroy()
	{
		register_lock=1;
		if (atres_render_iface) delete atres_render_iface;

		for (hmap<int,April::Texture*>::iterator it=g_font_textures.begin();it!=g_font_textures.end();it++)
			delete it->second;
		g_font_textures.clear();
		
		for (hmap<hstr,Dataset*>::iterator it2=g_datasets.begin();it2!=g_datasets.end();it2++)
			delete it2->second;
		g_datasets.clear();
	}
}
