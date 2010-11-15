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
	bool registerLock = false;
	hmap<int, April::Texture*> gFontTextures;
	hmap<hstr, Dataset*> gDatasets;
	float defaultScale = 1.0f;
	gvec2 cursorPosition;
#ifdef _DEBUG
	bool debugMode = false;
#endif
	hstr defaultTextsPath = "texts";
	void (*g_logFunction)(chstr) = aprilui_writelog;
	
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
		g_logFunction = fnptr;
	}
	
	void setCursorPos(float x, float y)
	{
		cursorPosition.set(x, y);
	}
	
	gvec2 getCursorPos()
	{
		return cursorPosition;
	}

	void init()
	{
	}

	Dataset* getDatasetByName(chstr name)
	{
		if (!gDatasets.has_key(name))
		{
			throw GenericException("Dataset '" + name + "' doesn't exist!");
		}
		return gDatasets[name];
	}

	void _registerDataset(chstr name, Dataset* dataset)
	{
		if (!registerLock)
		{
			if (gDatasets.has_key(name))
			{
				throw GenericException("Unable to register dataset '" + name + "', another dataset with the same name exists!");
			}
			gDatasets[name] = dataset;
		}
	}
	
	void _unregisterDataset(chstr name, Dataset* dataset)
	{
		if (!registerLock)
		{
			gDatasets.remove_key(name);
		}
	}
	
	void updateTextures(float time_increase)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->updateTextures(time_increase);
		}
	}
	
	void update(float time_increase)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->update(time_increase);
		}
	}

#ifdef _DEBUG
	void setDebugMode(bool value)
	{
		debugMode = value;
	}
	
	bool isDebugMode()
	{
		return debugMode;
	}
#endif

	hstr getDefaultTextsPath()
	{
		return defaultTextsPath;
	}
	
	void setDefaultTextsPath(chstr path)
	{
		defaultTextsPath = path;
	}
	
	void setDefaultScale(float value)
	{
		defaultScale = value;
	}
	
	float getDefaultScale()
	{
		return defaultScale;
	}

	void destroy()
	{
		registerLock = true;
		for (hmap<int, April::Texture*>::iterator it = gFontTextures.begin(); it != gFontTextures.end(); it++)
		{
			delete it->second;
		}
		gFontTextures.clear();
		foreach_m (Dataset*, it, gDatasets)
		{
			delete it->second;
		}
		gDatasets.clear();
	}
}
