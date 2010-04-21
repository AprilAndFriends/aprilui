/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "AprilUI.h"
#include "atres/Atres.h"
#include "AtresRenderInterface.h"
#include "april/RenderSystem.h"
#include "Dataset.h"
#include "Exception.h"
#include <map>
#include "hltypes/hstring.h"

namespace AprilUI
{
	bool register_lock=0;
	std::map<int,April::Texture*> g_font_textures;
	AtresAprilInterface* atres_render_iface=0;
	std::map<hstr,Dataset*> g_datasets;

	void init()
	{
		atres_render_iface=new AtresAprilInterface();
		Atres::setRenderInterface(atres_render_iface);
	}

	Dataset* getDatasetByName(hstr name)
	{
		if (g_datasets.find(name) == g_datasets.end())
			throw GenericException("Dataset '"+name+"' doesn't exist!");
		return g_datasets[name];
	}

	void _registerDataset(hstr name,Dataset* d)
	{
		if (register_lock) return;
		if (g_datasets.find(name) != g_datasets.end())
			throw GenericException("Unable to register dataset '"+name+"', another dataset with the same name exists!");	
		g_datasets[name]=d;
	}
	
	void _unregisterDataset(hstr name,Dataset* d)
	{
		if (register_lock) return;
		g_datasets.erase(name);
	}
	
	void update(float time_increase)
	{
		for (std::map<hstr,Dataset*>::iterator it2=g_datasets.begin();it2!=g_datasets.end();it2++)
			it2->second->update(time_increase);
	}

	void destroy()
	{
		register_lock=1;
		if (atres_render_iface) delete atres_render_iface;

		for (std::map<int,April::Texture*>::iterator it=g_font_textures.begin();it!=g_font_textures.end();it++)
			delete it->second;
		
		for (std::map<hstr,Dataset*>::iterator it2=g_datasets.begin();it2!=g_datasets.end();it2++)
			delete it2->second;
	}
}
