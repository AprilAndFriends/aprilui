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

#include <april/Keys.h>
#include <april/RenderSystem.h>
#include <april/Window.h>
#include <atres/atres.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Animators.h"
#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "Image.h"
#include "Objects.h"

#define REGISTER_ANIMATOR_TYPE(name) aprilui::registerAnimatorFactory(#name, &Animators::name::createInstance)

namespace aprilui
{
	bool registerLock = false;
	hmap<hstr, Dataset*> gDatasets;
	hmap<hstr, Object* (*)(chstr, grect)> gObjectFactories;
	hmap<hstr, Animator* (*)(chstr)> gAnimatorFactories;
	float defaultScale = 1.0f;
	Image* gCursor = NULL;
	bool cursorVisible = true;
    gvec2 cursorPosition;
	bool limitCursorToViewport = false;
	bool limitCursorToScreenViewport = true;
	bool hoverEffectEnabled = true;
	grect viewport;
	grect screenViewport;
	bool debugMode = false;
	hstr defaultTextsPath = "texts";
	hstr localization = "";
	void aprilui_writelog(chstr message)
	{
		printf("%s\n", message.c_str());		
	}
	void (*g_logFunction)(chstr) = aprilui_writelog;
	
	void log(chstr message, chstr prefix)
	{
		g_logFunction(prefix + message);
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
		harray<unsigned char> allowedButtons;
		allowedButtons += april::AK_LBUTTON;
		ButtonBase::setAllowedButtons(allowedButtons);

		APRILUI_REGISTER_OBJECT_TYPE(CallbackObject);
		APRILUI_REGISTER_OBJECT_TYPE(ColoredQuad);
		APRILUI_REGISTER_OBJECT_TYPE(Container);
		APRILUI_REGISTER_OBJECT_TYPE(EditBox);
		APRILUI_REGISTER_OBJECT_TYPE(ImageBox);
		APRILUI_REGISTER_OBJECT_TYPE(ImageButton);
		APRILUI_REGISTER_OBJECT_TYPE(Label);
		APRILUI_REGISTER_OBJECT_TYPE(Slider);
		APRILUI_REGISTER_OBJECT_TYPE(TextButton);
		APRILUI_REGISTER_OBJECT_TYPE(TextImageButton);
		APRILUI_REGISTER_OBJECT_TYPE(ToggleButton);

		REGISTER_ANIMATOR_TYPE(AlphaChanger);
		REGISTER_ANIMATOR_TYPE(BlueChanger);
		REGISTER_ANIMATOR_TYPE(FrameAnimation);
		REGISTER_ANIMATOR_TYPE(GreenChanger);
		REGISTER_ANIMATOR_TYPE(MoverX);
		REGISTER_ANIMATOR_TYPE(MoverY);
		REGISTER_ANIMATOR_TYPE(RedChanger);
		REGISTER_ANIMATOR_TYPE(ResizerX);
		REGISTER_ANIMATOR_TYPE(ResizerY);
		REGISTER_ANIMATOR_TYPE(Rotator);
		REGISTER_ANIMATOR_TYPE(ScalerX);
		REGISTER_ANIMATOR_TYPE(ScalerY);
		REGISTER_ANIMATOR_TYPE(TiledScrollerX);
		REGISTER_ANIMATOR_TYPE(TiledScrollerY);
	}
	
	void destroy()
	{
		registerLock = true;
		foreach_m (Dataset*, it, gDatasets)
		{
			delete it->second;
		}
		gDatasets.clear();
	}
	
	void setDebugMode(bool value)
	{
		debugMode = value;
	}
	
	bool isDebugMode()
	{
		return debugMode;
	}

	void setDefaultScale(float value)
	{
		defaultScale = value;
	}
	
	float getDefaultScale()
	{
		return defaultScale;
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

	bool isHoverEffectEnabled()
	{
		return hoverEffectEnabled;
	}

	void setHoverEffectEnabled(bool value)
	{
		hoverEffectEnabled = value;
	}

	hstr getDefaultTextsPath()
	{
		return defaultTextsPath;
	}
	
	void setDefaultTextsPath(chstr value)
	{
		defaultTextsPath = value;
	}
	
	hstr getLocalization()
	{
		return localization;
	}
	
	void setLocalization(chstr value)
	{
		localization = value;
	}

	hmap<hstr, Dataset*> getDatasets()
	{
		return gDatasets;
	}

	void registerObjectFactory(chstr typeName, Object* (*factory)(chstr, grect))
	{
		if (gObjectFactories.has_key(typeName))
		{
			throw ObjectFactoryExistsException(typeName);
		}
		gObjectFactories[typeName] = factory;
	}

	void registerAnimatorFactory(chstr typeName, Animator* (*factory)(chstr))
	{
		if (gAnimatorFactories.has_key(typeName))
		{
			throw AnimatorFactoryExistsException(typeName);
		}
		gAnimatorFactories[typeName] = factory;
	}

	Object* _createObject(chstr typeName, chstr name, grect rect)
	{
		if (gObjectFactories.has_key(typeName))
		{
			return (*gObjectFactories[typeName])(name, rect);
		}
		return NULL;
	}

	Animator* _createAnimator(chstr typeName, chstr name)
	{
		if (gAnimatorFactories.has_key(typeName))
		{
			return (*gAnimatorFactories[typeName])(name);
		}
		return NULL;
	}

    void updateCursorPosition()
    {
		april::Window* window = april::rendersys->getWindow();
		cursorPosition = window->getCursorPosition();
		cursorPosition.x = (float)(int)(cursorPosition.x * screenViewport.w / window->getWidth()) - viewport.x;
		cursorPosition.y = (float)(int)(cursorPosition.y * screenViewport.h / window->getHeight()) - viewport.y;
		if (limitCursorToViewport)
		{
			cursorPosition.x = hclamp(cursorPosition.x, 0.0f, viewport.w - 1);
			cursorPosition.y = hclamp(cursorPosition.y, 0.0f, viewport.h - 1);
		}
		else if (limitCursorToScreenViewport)
		{
			cursorPosition.x = hclamp(cursorPosition.x, -viewport.x, screenViewport.w - viewport.x - 1);
			cursorPosition.y = hclamp(cursorPosition.y, -viewport.y, screenViewport.w - viewport.y - 1);
		}
    }
    
    void setCursorPosition(gvec2 position)
    {
        cursorPosition = position;
    }
    
	gvec2 getCursorPosition()
	{
        return cursorPosition;
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
	
	void unloadUnusedTextures()
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->unloadUnusedTextures();
		}
	}
	
	void update(float time_increase)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->update(time_increase);
		}
	}

	void onMouseDown(float x, float y, int button)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseDown(x, y, button);
		}
	}
	
	void onMouseUp(float x, float y, int button)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseUp(x, y, button);
		}
	}
	
	void onMouseMove(float x, float y)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseMove(x, y);
		}
	}
	
	void onKeyDown(unsigned int keycode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onKeyDown(keycode);
		}
	}
	
	void onKeyUp(unsigned int keycode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onKeyUp(keycode);
		}
	}
	
	void onChar(unsigned int charcode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onChar(charcode);
		}
	}
	
	void OnMouseDown(float x, float y, int button) { onMouseDown(x, y, button); } // DEPRECATED
	void OnMouseUp(float x, float y, int button) { onMouseUp(x, y, button); } // DEPRECATED
	void OnMouseMove(float x, float y) { onMouseMove(x, y); } // DEPRECATED
	void OnKeyDown(unsigned int keycode) { onKeyDown(keycode); } // DEPRECATED
	void OnKeyUp(unsigned int keycode) { onKeyUp(keycode); } // DEPRECATED
	void OnChar(unsigned int charcode) { onChar(charcode); } // DEPRECATED

}