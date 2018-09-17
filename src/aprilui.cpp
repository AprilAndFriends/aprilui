/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>

#include <april/RenderSystem.h>
#include <atres/atres.h>
#include <hltypes/hlog.h>
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
	gvec2f cursorPosition;
	hstr (*g_macroCallback)(chstr) = 0;
#ifdef _DEBUG
	bool debugMode = false;
#endif
	hstr defaultTextsPath = "texts";
	hstr g_locale;
	float textureIdleUnloadTime = 0.0f;
	grectf viewport(0.0f, 0.0f, 1024.0f, 768.0f);
	// TODO - hack, has to be removed
	bool forcedDynamicLoading = false;
	
	void log(chstr message, chstr prefix)
	{
		hlog::write(prefix, message);
	}
	void logMessage(chstr message, chstr prefix)
	{
		log(message, prefix);
	}
	
	void aprilui_writelog(chstr message)
	{
		log("aprilui", message);
	}
	
	void init()
	{
	}

	void destroy()
	{
		registerLock = true;
		for (hmap<int, april::Texture*>::iterator it = gFontTextures.begin(); it != gFontTextures.end(); it++)
		{
			april::rendersys->destroyTexture(it->second);
		}
		gFontTextures.clear();
		foreach_m (Dataset*, it, gDatasets)
		{
			delete it->second;
		}
		gDatasets.clear();
	}
	
	gvec2f getCursorPosition()
	{
		return cursorPosition;
	}

	void setCursorPosition(gvec2f position)
	{
		cursorPosition = position;
	}
	
	void setCursorPosition(float x, float y)
	{
		cursorPosition = gvec2f(x, y);
	}
	
	grectf getViewport()
	{
		return viewport;
	}

	void setViewport(grectf value)
	{
		viewport = value;
	}

	hmap<hstr, Dataset*> getDatasets()
	{
		return gDatasets;
	}

	bool hasDataset(chstr name)
	{
		return gDatasets.hasKey(name);
	}

	Dataset* getDatasetByName(chstr name)
	{
		if (!gDatasets.hasKey(name))
		{
			throw GenericException("Dataset '" + name + "' doesn't exist!");
		}
		return gDatasets[name];
	}

	void _registerDataset(chstr name, Dataset* dataset)
	{
		if (!registerLock)
		{
			if (gDatasets.hasKey(name))
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
			gDatasets.removeKey(name);
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
			it->second->reloadTexts();
			it->second->reloadTextures();
		}
		// finished localization change, now call the appropriate event
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
	
	void OnKeyDown(unsigned int keyCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnKeyDown(keyCode);
		}
	}
	
	void OnKeyUp(unsigned int keyCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnKeyUp(keyCode);
		}
	}
	
	void OnChar(unsigned int charCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->OnChar(charCode);
		}
	}
	
	void setMacroCallback(hstr (*callback)(chstr))
	{
		g_macroCallback = callback;
	}
	
	hstr expandMacro(chstr macro)
	{
		if (g_macroCallback == NULL)
		{
			throw Exception("aprilui macro callback not set, trying to expand macro '" + macro + "'");
		}
		return g_macroCallback(macro);
	}

	void unloadUnusedTextures()
	{
		foreach_m( Dataset*, it, gDatasets)
		{
			it->second->unloadUnusedTextures();
		}
	}

	// TODO - hack, has to be removed
	bool getForcedDynamicLoading() { return forcedDynamicLoading; }
	void setForcedDynamicLoading(bool value) { forcedDynamicLoading = value; }

}
