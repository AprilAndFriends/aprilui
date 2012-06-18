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
#include <atres/atres.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"

namespace aprilui
{
	bool registerLock = false;
	hmap<int, april::Texture*> gFontTextures;
	hmap<hstr, Dataset*> gDatasets;
	float defaultScale = 1.0f;
	gvec2 cursorPosition;
	hstr (*g_macroCallback)(chstr) = 0;
#ifdef _DEBUG
	bool debugMode = false;
#endif
	hstr defaultTextsPath = "texts";
	void (*g_logFunction)(chstr) = aprilui_writelog;
	hstr g_locale;
	float textureIdleUnloadTime = 0.0f;
	// TODO - hack, has to be removed
	bool forcedDynamicLoading = false;
	
	void log(chstr message, chstr prefix)
	{
		g_logFunction(prefix + message);
	}
	void logMessage(chstr message, chstr prefix)
	{
		log(message, prefix);
	}
	
	void aprilui_writelog(chstr message)
	{
		printf("%s\n", message.c_str());		
	}
	
	void setLogFunction(void (*fnptr)(chstr))
	{
		g_logFunction = fnptr;
	}
	
	void init()
	{
	}

	void destroy()
	{
		registerLock = true;
		for (hmap<int, april::Texture*>::iterator it = gFontTextures.begin(); it != gFontTextures.end(); it++)
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
	
	// deprecated
	gvec2 getCursorPos()
	{
		return cursorPosition;
	}

	// deprecated
	void setCursorPos(float x, float y)
	{
		cursorPosition.set(x, y);
	}
	
	gvec2 getCursorPosition()
	{
		return cursorPosition;
	}

	void setCursorPosition(gvec2 position)
	{
		cursorPosition = position;
	}
	
	void setCursorPosition(float x, float y)
	{
		cursorPosition = gvec2(x, y);
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
	
	hstr getLocalization()
	{
		return g_locale;
	}
	
	void setLocalization(chstr localization)
	{
		g_locale = localization;
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->notifyEvent("onLocalizationChanged", NULL);
		}
	}
	
	float getTextureIdleUnloadTime()
	{
		return textureIdleUnloadTime;
	}

	void setTextureIdleUnloadTime(float value)
	{
		textureIdleUnloadTime = value;
	}

	void setDefaultScale(float value)
	{
		defaultScale = value;
	}
	
	float getDefaultScale()
	{
		return defaultScale;
	}
	
	void OnMouseDown(float x, float y, int button)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnMouseDown(x, y, button);
		}
	}
	
	void OnMouseUp(float x, float y, int button)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnMouseUp(x, y, button);
		}
	}
	
	void OnMouseMove(float x, float y)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnMouseMove(x, y);
		}
	}
	
	void OnKeyDown(unsigned int keycode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnKeyDown(keycode);
		}
	}
	
	void OnKeyUp(unsigned int keycode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnKeyUp(keycode);
		}
	}
	
	void OnChar(unsigned int charcode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnChar(charcode);
		}
	}
	
	apriluiFnExport void setMacroCallback(hstr (*callback)(chstr))
	{
		g_macroCallback = callback;
	}
	
	apriluiFnExport hstr expandMacro(chstr macro)
	{
		if (g_macroCallback == NULL) throw hl_exception("aprilui macro callback not set, trying to expand macro '" + macro + "'");
		return g_macroCallback(macro);
	}

	// TODO - hack, has to be removed
	bool getForcedDynamicLoading() { return forcedDynamicLoading; }
	void setForcedDynamicLoading(bool value) { forcedDynamicLoading = value; }

}
