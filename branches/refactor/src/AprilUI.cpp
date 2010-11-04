/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
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
#include "Dataset.h"
#include "Exception.h"

namespace AprilUI
{
	bool register_lock=0;
	hmap<int,April::Texture*> g_font_textures;
	hmap<hstr,Dataset*> g_datasets;
	float default_scale=1;
	gvec2 cursor_pos;
#ifdef _DEBUG
	bool g_debug_mode = false;
#endif
	hstr defaultTextsPath="texts";
	
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
	}

	Dataset* getDatasetByName(chstr name)
	{
		if (!g_datasets.has_key(name))
			throw GenericException("Dataset '"+name+"' doesn't exist!");
		return g_datasets[name];
	}

	void _registerDataset(chstr name,Dataset* d)
	{
		if (register_lock) return;
		if (g_datasets.has_key(name))
			throw GenericException("Unable to register dataset '"+name+"', another dataset with the same name exists!");	
		g_datasets[name]=d;
	}
	
	void _unregisterDataset(chstr name,Dataset* d)
	{
		if (register_lock) return;
		g_datasets.remove_key(name);
	}
	
	void updateTextures(float time_increase)
	{
		foreach_m (Dataset*, it, g_datasets)
			it->second->updateTextures(time_increase);
	}
	
	void update(float time_increase)
	{
		foreach_m (Dataset*, it, g_datasets)
			it->second->update(time_increase);
	}

#ifdef _DEBUG
	void setDebugMode(bool value)
	{
		g_debug_mode = value;
	}
	
	bool isDebugMode()
	{
		return g_debug_mode;
	}
#endif

	hstr getDefaultTextsPath()
	{
		return defaultTextsPath;
	}
	
	void setDefaultTextsPath(chstr path)
	{
		defaultTextsPath=path;
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

		for (hmap<int,April::Texture*>::iterator it=g_font_textures.begin();it!=g_font_textures.end();it++)
			delete it->second;
		g_font_textures.clear();
		
		foreach_m (Dataset*, it, g_datasets)
			delete it->second;
		g_datasets.clear();
	}
}
