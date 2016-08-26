/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
#include <hltypes/hversion.h>

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "Image.h"
#include "Objects.h"

#define REGISTER_ANIMATOR_TYPE(name) aprilui::registerAnimatorFactory(#name, &Animators::name::createInstance)

namespace aprilui
{
	bool getDefaultDynamicLoading() { return isDefaultManagedTextures(); } // DEPRECATED
	void setDefaultDynamicLoading(bool value) { setDefaultManagedTextures(value); } // DEPRECATED

	hstr logTag = "aprilui";

	static hversion version(4, 1, 0);

	static bool registerLock = false;
	static hmap<hstr, Dataset*> gDatasets;
	static hmap<hstr, Object* (*)(chstr)> gObjectFactories;
	static hmap<hstr, Animator* (*)(chstr)> gAnimatorFactories;
	static BaseImage* gCursor = NULL;
	static bool cursorVisible = true;
	static gvec2 cursorPosition;
	static bool limitCursorToViewport = false;
	static bool hoverEffectEnabled = true;
	static grect viewport;
	static bool debugEnabled = false;
	static hstr defaultTextsPath = "texts";
	static hstr defaultLocalization = "";
	static hstr localization = "";
	static harray<hstr> supportedLocalizations;
	static float textureIdleUnloadTime = 0.0f;
	static bool defaultManagedTextures = false;
	static april::Texture::LoadMode defaultTextureLoadMode = april::Texture::LoadMode::Async;
	static bool useKeyboardAutoOffset = false;
	static hmap<hstr, float> extensionScales;

	void init()
	{
		hlog::write(logTag, "Initializing AprilUI: " + version.toString());
		registerLock = false;
		cursorVisible = true;
		limitCursorToViewport = true;
		hoverEffectEnabled = true;
		debugEnabled = false;
		defaultTextsPath = "texts";
		localization = "";
		textureIdleUnloadTime = 0.0f;
		defaultManagedTextures = false;
		defaultTextureLoadMode = april::Texture::LoadMode::Async;
		viewport.setSize(april::window->getSize());
		ButtonBase::allowedKeys.clear();
		ButtonBase::allowedKeys += april::AK_LBUTTON;
		ButtonBase::allowedButtons.clear();
		ButtonBase::allowedButtons += april::AB_A;

		APRILUI_REGISTER_OBJECT_TYPE(CallbackObject);
		APRILUI_REGISTER_OBJECT_TYPE(Container);
		APRILUI_REGISTER_OBJECT_TYPE(EditBox);
		APRILUI_REGISTER_OBJECT_TYPE(FilledRect);
		APRILUI_REGISTER_OBJECT_TYPE(GridView);
		APRILUI_REGISTER_OBJECT_TYPE(GridViewCell);
		APRILUI_REGISTER_OBJECT_TYPE(GridViewRow);
		APRILUI_REGISTER_OBJECT_TYPE(GridViewRowTemplate);
		APRILUI_REGISTER_OBJECT_TYPE(ImageBox);
		APRILUI_REGISTER_OBJECT_TYPE(ImageButton);
		APRILUI_REGISTER_OBJECT_TYPE(Label);
		APRILUI_REGISTER_OBJECT_TYPE(ListBox);
		APRILUI_REGISTER_OBJECT_TYPE(ListBoxItem);
		APRILUI_REGISTER_OBJECT_TYPE(OptionButton);
		APRILUI_REGISTER_OBJECT_TYPE(ProgressBar);
		APRILUI_REGISTER_OBJECT_TYPE(ProgressCircle);
		APRILUI_REGISTER_OBJECT_TYPE(Rect);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollArea);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonBackground);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonBackward);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonForward);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonSlider);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarH);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarV);
		APRILUI_REGISTER_OBJECT_TYPE(TreeView);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewExpander);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewImage);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewLabel);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewNode);
		APRILUI_REGISTER_OBJECT_TYPE(TextImageButton);
		APRILUI_REGISTER_OBJECT_TYPE(ToggleButton);

		REGISTER_ANIMATOR_TYPE(AlphaChanger);
		REGISTER_ANIMATOR_TYPE(BlueChanger);
		REGISTER_ANIMATOR_TYPE(FrameAnimation);
		REGISTER_ANIMATOR_TYPE(GreenChanger);
		REGISTER_ANIMATOR_TYPE(MoverX);
		REGISTER_ANIMATOR_TYPE(MoverY);
		REGISTER_ANIMATOR_TYPE(ProgressChanger);
		REGISTER_ANIMATOR_TYPE(RedChanger);
		REGISTER_ANIMATOR_TYPE(ResizerX);
		REGISTER_ANIMATOR_TYPE(ResizerY);
		REGISTER_ANIMATOR_TYPE(Rotator);
		REGISTER_ANIMATOR_TYPE(ScalerX);
		REGISTER_ANIMATOR_TYPE(ScalerY);
		REGISTER_ANIMATOR_TYPE(TileScrollerX);
		REGISTER_ANIMATOR_TYPE(TileScrollerY);
		REGISTER_ANIMATOR_TYPE(ZOrderChanger);
	}
	
	void destroy()
	{
		hlog::write(logTag, "Destroying AprilUI.");
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
		hlog::write(logTag, "Setting localization to: " + value);
		hstr previousLocalization = localization;
		if (supportedLocalizations.size() > 0 && !supportedLocalizations.has(value) &&
			value != defaultLocalization)
		{
			hlog::warnf(logTag, "Localization '%s' not supported, defaulting back to '%s'.",
				value.cStr(), defaultLocalization.cStr());
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
					it->second->notifyEvent(Event::LocalizationChanged, NULL);
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

	bool isDefaultManagedTextures()
	{
		return defaultManagedTextures;
	}

	void setDefaultManagedTextures(bool value)
	{
		defaultManagedTextures = value;
	}

	april::Texture::LoadMode getDefaultTextureLoadMode()
	{
		return defaultTextureLoadMode;
	}

	void setDefaultTextureLoadMode(april::Texture::LoadMode value)
	{
		if (value == april::Texture::LoadMode::Immediate)
		{
			hlog::warn(logTag, "'setDefaultTextureLoadMode' with 'april::Texture::LoadMode::Immediate' is deprecated. Defaulting to 'april::Texture::LoadMode::Async'"); // DEPRECATED
			value = april::Texture::LoadMode::Async;
		}
		defaultTextureLoadMode = value;
	}

	bool isUseKeyboardAutoOffset()
	{
		return useKeyboardAutoOffset;
	}

	void setUseKeyboardAutoOffset(bool value)
	{
		useKeyboardAutoOffset = value;
	}

	hmap<hstr, Dataset*> getDatasets()
	{
		return gDatasets;
	}
	
	void registerObjectFactory(chstr typeName, Object* (*factory)(chstr))
	{
		if (gObjectFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryExistsException("Object", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		gObjectFactories[typeName] = factory;
	}
	
	void registerAnimatorFactory(chstr typeName, Animator* (*factory)(chstr))
	{
		if (gAnimatorFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryExistsException("Animator", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		gAnimatorFactories[typeName] = factory;
	}
	
	void unregisterObjectFactory(chstr typeName)
	{
		if (!gObjectFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryNotExistsException("Object", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		gObjectFactories.removeKey(typeName);
	}
	
	void unregisterAnimatorFactory(chstr typeName)
	{
		if (!gAnimatorFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryNotExistsException("Animator", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		gAnimatorFactories.removeKey(typeName);
	}

	const hmap<hstr, Object* (*)(chstr)>& getObjectFactories()
	{
		return gObjectFactories;
	}

	const hmap<hstr, Animator* (*)(chstr)>& getAnimatorFactories()
	{
		return gAnimatorFactories;
	}

	bool hasObjectFactory(chstr typeName)
	{
		return gObjectFactories.hasKey(typeName);
	}
	
	bool hasAnimatorFactory(chstr typeName)
	{
		return gAnimatorFactories.hasKey(typeName);
	}
	
	Object* createObject(chstr typeName, chstr name)
	{
		if (gObjectFactories.hasKey(typeName))
		{
			return (*gObjectFactories[typeName])(name);
		}
		hstr switchedTypeName = typeName;
		// DEPRECATED start
		if (switchedTypeName == "ColoredQuad")
		{
			hlog::warn(logTag, "'ColoredQuad' is deprecated. Use 'FilledRect' instead.");
			switchedTypeName = "FilledRect";
		}
		if (gObjectFactories.hasKey(switchedTypeName))
		{
			return (*gObjectFactories[switchedTypeName])(name);
		}
		// DEPRECATED end
		return NULL;
	}
	
	Animator* createAnimator(chstr typeName, chstr name)
	{
		if (gAnimatorFactories.hasKey(typeName))
		{
			return (*gAnimatorFactories[typeName])(name);
		}
		hstr switchedTypeName = typeName;
		// DEPRECATED start
		if (switchedTypeName == "TiledScrollerX")
		{
			hlog::warn(logTag, "'TiledScrollerX' is deprecated. Use 'TileScrollerX' instead.");
			switchedTypeName = "TileScrollerX";
		}
		else if (switchedTypeName == "TiledScrollerY")
		{
			hlog::warn(logTag, "'TiledScrollerY' is deprecated. Use 'TileScrollerY' instead.");
			switchedTypeName = "TileScrollerY";
		}
		else if (switchedTypeName == "CenterMoverX")
		{
			hlog::warn(logTag, "'CenterMoverX' is deprecated. Use 'PivotMoverX' instead.");
			switchedTypeName = "PivotMoverX";
		}
		else if (switchedTypeName == "CenterMoverY")
		{
			hlog::warn(logTag, "'CenterMoverY' is deprecated. Use 'PivotMoverY' instead.");
			switchedTypeName = "PivotMoverY";
		}
		if (gAnimatorFactories.hasKey(switchedTypeName))
		{
			return (*gAnimatorFactories[switchedTypeName])(name);
		}
		// DEPRECATED end
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

	void updateViewportPosition(grect newViewport, bool updateOrthoProjection)
	{
		viewport = newViewport;
		bool keyboardVisible = (useKeyboardAutoOffset && april::window->isVirtualKeyboardVisible());
		if (!keyboardVisible)
		{
			if (updateOrthoProjection)
			{
				april::rendersys->setOrthoProjection(viewport);
			}
			return;
		}
		float viewportHeight = viewport.h;
		float visibleHeight = (float)(int)(viewportHeight * (1.0f - april::window->getVirtualKeyboardHeightRatio()));
		float keyboardHeight = viewportHeight - visibleHeight;
		aprilui::Object* object = NULL;
		aprilui::EditBox* editbox = NULL;
		grect rect;
		float h = 0.0f;
		foreach_m (Dataset*, it, gDatasets)
		{
			object = it->second->getFocusedObject();
			if (object != NULL)
			{
				rect = object->getBoundingRect();
				h = (float)(int)(rect.y + rect.h * 0.5f);
				editbox = dynamic_cast<aprilui::EditBox*>(object);
				if (editbox != NULL)
				{
					h = editbox->getDerivedPoint(editbox->getCaretRect().getPosition()).y;
				}
				viewport.y = hclamp(-h + (float)(int)(visibleHeight * 0.5f), -keyboardHeight, 0.0f);
				break;
			}
		}
		if (updateOrthoProjection)
		{
			april::rendersys->setOrthoProjection(viewport);
		}
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
	
	void setCursorImage(BaseImage* image)
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
			gCursor->draw(grect(getCursorPosition(), gCursor->getSrcSize()));
		}
	}

	void setDebugExceptionsEnabled(bool textureFiles, bool childManipulation, bool creationFactories, bool objectExistence, bool systemConsistency)
	{
#ifdef _DEBUG
		hlog::warn(logTag, "Setting debug-exceptions!");
		aprilui::textureFilesDebugExceptionsEnabled = textureFiles;
		aprilui::childManipulationDebugExceptionsEnabled = childManipulation;
		aprilui::creationFactoriesDebugExceptionsEnabled = creationFactories;
		aprilui::objectExistenceDebugExceptionsEnabled = objectExistence;
		aprilui::systemConsistencyDebugExceptionsEnabled = systemConsistency;
#else
		hlog::warn(logTag, "Setting debug-exceptions is not allowed on release builds and will be ignored!");
#endif
	}
	
	Dataset* getDatasetByName(chstr name)
	{
		if (!gDatasets.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Dataset", name, name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
		}
		return gDatasets[name];
	}
	
	void _registerDataset(chstr name, Dataset* dataset)
	{
		if (!registerLock)
		{
			if (gDatasets.hasKey(name))
			{
				__THROW_EXCEPTION(ObjectExistsException("Dataset", name, name), aprilui::objectExistenceDebugExceptionsEnabled, return);
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
	
	void notifyEvent(chstr type, EventArgs* args)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->notifyEvent(type, args);
		}
	}

	void processEvents()
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->processEvents();
		}
	}

	void update(float timeDelta)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->update(timeDelta);
		}
	}
	
	void updateTextures(float timeDelta)
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->updateTextures(timeDelta);
		}
	}
	
	void unloadUnusedResources()
	{
		foreach_m (Dataset*, it, gDatasets)
		{
			it->second->unloadUnusedResources();
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
			hlog::warn(logTag, "setTextureExtensionPrefixes() called with unmatching 'prefixes' and 'scales' sizes.");
			return false;
		}
		harray<hstr> currentExtensions = april::getTextureExtensions();
		// removing all non-default extensions
		HL_LAMBDA_CLASS(_defaultExtensions, bool, ((hstr const& extension) { return (extension.count('.') <= 1); }));
		harray<hstr> defaultExtensions = currentExtensions.findAll(&_defaultExtensions::lambda);
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
		return (extensionScales.hasKey(extension) ? extensionScales[extension] : 1.0f);
	}
	
	float findTextureExtensionScale(chstr filename)
	{
		harray<hstr> extensions = april::getTextureExtensions();
		foreach (hstr, it, extensions)
		{
			if (filename.endsWith(*it))
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
	
}
