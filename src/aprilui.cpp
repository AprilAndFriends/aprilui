/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>

#include <april/april.h>
#include <april/Keys.h>
#include <april/RenderSystem.h>
#include <april/Window.h>
#include <atres/atres.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Animators.h"
#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "Image.h"
#include "Objects.h"

#define REGISTER_ANIMATOR_TYPE(name) aprilui::registerAnimatorFactory(#name, &Animators::name::createInstance)

namespace aprilui
{
	hstr logTag = "aprilui";

	bool registerLock = false;
	hmap<hstr, Dataset*> gDatasets;
	hmap<hstr, Object* (*)(chstr, grect)> gObjectFactories;
	hmap<hstr, Animator* (*)(chstr)> gAnimatorFactories;
	float defaultScale = 1.0f;
	Image* gCursor = NULL;
	bool cursorVisible = true;
	gvec2 cursorPosition;
	bool limitCursorToViewport = false;
	bool hoverEffectEnabled = true;
	grect viewport;
	bool debugEnabled = false;
	hstr defaultTextsPath = "texts";
	hstr defaultLocalization = "";
	hstr localization = "";
	harray<hstr> supportedLocalizations;
	float textureIdleUnloadTime = 0.0f;
	hmap<hstr, float> extensionScales;
	bool defaultDynamicLoading = false;

	void init()
	{
		hlog::write(aprilui::logTag, "Initializing AprilUI.");
		registerLock = false;
		defaultScale = 1.0f;
		cursorVisible = true;
		limitCursorToViewport = true;
		hoverEffectEnabled = true;
		debugEnabled = false;
		defaultTextsPath = "texts";
		localization = "";
		textureIdleUnloadTime = 0.0f;
		viewport.setSize(april::window->getSize());
		harray<april::Key> allowedKeys;
		allowedKeys += april::AK_LBUTTON;
		ButtonBase::setAllowedKeys(allowedKeys);
		harray<april::Button> allowedButtons;
		allowedButtons += april::AB_A;
		ButtonBase::setAllowedButtons(allowedButtons);

		APRILUI_REGISTER_OBJECT_TYPE(CallbackObject);
		APRILUI_REGISTER_OBJECT_TYPE(ColoredQuad);
		APRILUI_REGISTER_OBJECT_TYPE(Container);
		APRILUI_REGISTER_OBJECT_TYPE(EditBox);
		APRILUI_REGISTER_OBJECT_TYPE(ImageBox);
		APRILUI_REGISTER_OBJECT_TYPE(ImageButton);
		APRILUI_REGISTER_OBJECT_TYPE(Label);
		APRILUI_REGISTER_OBJECT_TYPE(ProgressBar);
		APRILUI_REGISTER_OBJECT_TYPE(Slider);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollArea);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarH);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarV);
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
		hlog::write(aprilui::logTag, "Destroying AprilUI.");
		registerLock = true;
		foreach_m (Dataset*, it, gDatasets)
		{
			delete it->second;
		}
		gDatasets.clear();
		gObjectFactories.clear();
		gAnimatorFactories.clear();
		gCursor = NULL;
	}
	
	bool isDebugEnabled()
	{
		return debugEnabled;
	}
	
	void setDebugEnabled(bool value)
	{
		debugEnabled = value;
	}
	
	float getDefaultScale()
	{
		return defaultScale;
	}
	
	void setDefaultScale(float value)
	{
		defaultScale = value;
	}
	
	grect getViewport()
	{
		return viewport;
	}
	
	void setViewport(grect value)
	{
		viewport = value;
	}
	
	bool isLimitCursorToViewport()
	{
		return limitCursorToViewport;
	}
	
	void setLimitCursorToViewport(bool value)
	{
		limitCursorToViewport = value;
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
	
	hstr getDefaultLocalization()
	{
		return defaultLocalization;
	}
	
	void setDefaultLocalization(chstr value)
	{
		defaultLocalization = value;
	}
	
	hstr getLocalization()
	{
		return localization;
	}
	
	void setLocalization(chstr value)
	{
		hlog::write(aprilui::logTag, "Setting localization to: " + value);
		hstr previousLocalization = localization;
		if (supportedLocalizations.size() > 0 && !supportedLocalizations.contains(value) &&
			value != defaultLocalization)
		{
			hlog::warnf(aprilui::logTag, "Localization '%s' not supported, defaulting back to '%s'.",
				value.c_str(), defaultLocalization.c_str());
			localization = defaultLocalization;
		}
		else
		{
			localization = value;
		}
		if (previousLocalization != localization)
		{
			foreach_m (Dataset*, it, gDatasets)
			{
				if (it->second->isLoaded())
				{
					it->second->reloadTexts();
					it->second->reloadTextures();
				}
			}
			// finished localization change, now call the appropriate event
			foreach_m (Dataset*, it, gDatasets)
			{
				if (it->second->isLoaded())
				{
					it->second->notifyEvent("onLocalizationChanged", NULL);
				}
			}
		}
	}

	harray<hstr> getSupportedLocalizations()
	{
		return supportedLocalizations;
	}
	
	void setSupportedLocalizations(harray<hstr> value)
	{
		supportedLocalizations = value;
	}
	
	float getTextureIdleUnloadTime()
	{
		return textureIdleUnloadTime;
	}
	
	void setTextureIdleUnloadTime(float value)
	{
		textureIdleUnloadTime = value;
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
	
	Object* createObject(chstr typeName, chstr name, grect rect)
	{
		if (gObjectFactories.has_key(typeName))
		{
			return (*gObjectFactories[typeName])(name, rect);
		}
		return NULL;
	}
	
	Animator* createAnimator(chstr typeName, chstr name)
	{
		if (gAnimatorFactories.has_key(typeName))
		{
			return (*gAnimatorFactories[typeName])(name);
		}
		return NULL;
	}
	
	gvec2 transformWindowPoint(gvec2 point)
	{
		point.x = (float)(int)(point.x * viewport.w / april::window->getWidth()) - viewport.x;
		point.y = (float)(int)(point.y * viewport.h / april::window->getHeight()) - viewport.y;
		if (limitCursorToViewport)
		{
			point.x = hclamp(point.x, 0.0f, viewport.w - 1);
			point.y = hclamp(point.y, 0.0f, viewport.h - 1);
		}
		return point;
	}
	
	void updateCursorPosition()
	{
		cursorPosition = transformWindowPoint(april::window->getCursorPosition());
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
	
	void notifyEvent(chstr name, void* params)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->notifyEvent(name, params);
		}
	}
	
	void processEvents()
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->processEvents();
		}
	}

	void update(float k)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->update(k);
		}
	}
	
	void updateTextures(float k)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->updateTextures(k);
		}
	}
	
	void unloadUnusedTextures()
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->unloadUnusedTextures();
		}
	}
	
	void reloadTextures()
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->reloadTextures();
		}
	}
	
	bool setTextureExtensionPrefixes(harray<hstr> prefixes, harray<float> scales)
	{
		if (prefixes.size() != scales.size())
		{
			hlog::warn(aprilui::logTag, "setTextureExtensionPrefixes() called with unmatching 'prefixes' and 'scales' sizes.");
			return false;
		}
		harray<hstr> currentExtensions = april::getTextureExtensions();
		// removing all non-default extensions
		harray<hstr> defaultExtensions = currentExtensions;
		foreach (hstr, it, currentExtensions)
		{
			if ((*it).count('.') > 1) // if extension is using a prefix
			{
				defaultExtensions.remove(*it);
			}
		}
		// adding new extensions and scales
		harray<hstr> newExtensions;
		harray<float> newScales;
		for_iter (i, 0, prefixes.size())
		{
			foreach (hstr, it, defaultExtensions)
			{
				newExtensions += prefixes[i] + (*it);
				newScales += scales[i];
			}
		}
		foreach (hstr, it, defaultExtensions)
		{
			newExtensions += (*it);
			newScales += 1.0f;
		}
		hmap<hstr, float> newExtensionScales;
		for_iter (i, 0, newExtensions.size())
		{
			newExtensionScales[newExtensions[i]] = newScales[i];
		}
		// if extension scales have not changed
		if (extensionScales != newExtensionScales)
		{
			extensionScales = newExtensionScales;
			april::setTextureExtensions(newExtensions);
			return true;
		}
		return false;
	}
	
	void setTextureExtensionScales(harray<hstr> extensions, harray<float> scales)
	{
		while (extensions.size() > scales.size())
		{
			scales += 1.0f;
		}
		extensionScales.clear();
		for_iter (i, 0, extensions.size())
		{
			extensionScales[extensions[i]] = scales[i];
		}
		april::setTextureExtensions(extensions);
	}
	
	float getTextureExtensionScale(chstr extension)
	{
		return (extensionScales.has_key(extension) ? extensionScales[extension] : 1.0f);
	}
	
	float findTextureExtensionScale(chstr filename)
	{
		harray<hstr> extensions = april::getTextureExtensions();
		foreach (hstr, it, extensions)
		{
			if (filename.ends_with(*it))
			{
				return getTextureExtensionScale(*it);
			}
		}
		return 1.0f;
	}

	void onMouseDown(april::Key keyCode)
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseDown(keyCode);
		}
	}
	
	void onMouseUp(april::Key keyCode)
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseUp(keyCode);
		}
	}
	
	void onMouseCancel(april::Key keyCode)
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseCancel(keyCode);
		}
	}
	
	void onMouseMove()
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseMove();
		}
	}
	
	void onMouseScroll(float x, float y)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onMouseScroll(x, y);
		}
	}
	
	void onKeyDown(april::Key keyCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onKeyDown(keyCode);
		}
	}
	
	void onKeyUp(april::Key keyCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onKeyUp(keyCode);
		}
	}
	
	void onChar(unsigned int charCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onChar(charCode);
		}
	}
	
	void onTouch(const harray<gvec2>& touches)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onTouch(touches);
		}
	}
	
	void onButtonDown(april::Button buttonCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onButtonDown(buttonCode);
		}
	}
	
	void onButtonUp(april::Button buttonCode)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->onButtonUp(buttonCode);
		}
	}
	
	bool getDefaultDynamicLoading() { return defaultDynamicLoading; }
	void setDefaultDynamicLoading(bool value) { defaultDynamicLoading = value; }
	
}
