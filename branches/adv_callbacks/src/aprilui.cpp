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
#include <april/Window.h>
#include <atres/atres.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "Image.h"

namespace aprilui
{
	bool registerLock = false;
	hmap<int, april::Texture*> gFontTextures;
	hmap<hstr, Dataset*> gDatasets;
	float defaultScale = 1.0f;
	Image* gCursor = NULL;
	bool cursorVisible = true;
	bool limitCursorToViewport = false;
	bool limitCursorToScreenViewport = true;
	grect viewport;
	grect screenViewport;
#ifdef _DEBUG
	bool debugMode = false;
#endif
	hstr defaultTextsPath = "texts";
	void aprilui_writelog(chstr message)
	{
		printf("%s\n", message.c_str());		
	}
	void (*g_logFunction)(chstr) = aprilui_writelog;
	
	void log(chstr message, chstr prefix)
	{
		g_logFunction(prefix + message);
	}
	
	void logf(chstr message, ...)
	{
		va_list args;
		va_start(args, message);
		aprilui::log(hvsprintf(message.c_str(), args));
		va_end(args);
	}
	
	void setLogFunction(void (*fnptr)(chstr))
	{
		g_logFunction = fnptr;
	}
	
	void init()
	{
		viewport.w = (float)april::rendersys->getWindow()->getWidth();
		viewport.h = (float)april::rendersys->getWindow()->getHeight();
		screenViewport = viewport;
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
	
	grect getViewport()
	{
		return viewport;
	}
	
	void setViewport(grect value)
	{
		viewport = value;
	}
	
	grect getScreenViewport()
	{
		return screenViewport;
	}
	
	void setScreenViewport(grect value)
	{
		screenViewport = value;
	}
	
	gvec2 getCursorPosition()
	{
		april::Window* window = april::rendersys->getWindow();
		gvec2 position = window->getCursorPosition();
		position.x = (float)(int)(position.x * screenViewport.w / window->getWidth()) - viewport.x;
		position.y = (float)(int)(position.y * screenViewport.h / window->getHeight()) - viewport.y;
		if (limitCursorToViewport)
		{
			position.x = hclamp(position.x, 0.0f, viewport.w - 1);
			position.y = hclamp(position.y, 0.0f, viewport.h - 1);
		}
		else if (limitCursorToScreenViewport)
		{
			position.x = hclamp(position.x, -viewport.x, screenViewport.w - viewport.x - 1);
			position.y = hclamp(position.y, -viewport.y, screenViewport.w - viewport.y - 1);
		}
		return position;
	}

	void setCursorImage(Image* image)
	{
		gCursor = image;
	}
	
	void showCursor()
	{
		cursorVisible = true;
	}
	
	void hideCursor()
	{
		cursorVisible = false;
	}
	
	void drawCursor()
	{
		if (gCursor != NULL && cursorVisible)
		{
			gCursor->draw(grect(getCursorPosition(), gCursor->getSrcRect().getSize()));
		}
	}
	
	bool isLimitCursorToViewport()
	{
		return limitCursorToViewport;
	}

	void setLimitCursorToViewport(bool value)
	{
		limitCursorToViewport = value;
	}

	bool isLimitCursorToScreenViewport()
	{
		return limitCursorToScreenViewport;
	}

	void setLimitCursorToScreenViewport(bool value)
	{
		limitCursorToScreenViewport = value;
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
	
}
