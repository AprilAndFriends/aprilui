/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.93
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines all functions used in aprilui.

#ifndef APRILUI_H
#define APRILUI_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

#define APRILUI_REGISTER_OBJECT_TYPE(name) aprilui::registerObjectFactory(#name, &name::createInstance)
#define APRILUI_REGISTER_ANIMATOR_TYPE(name) aprilui::registerAnimatorFactory(#name, &name::createInstance)

namespace aprilui
{
	class Animator;
	class Dataset;
	class Image;
	class Object;
	
	/// @brief Initializes AprilUI.
	/// @param[in] hdEnabled The HD-enabled parameter.
	/// @note HD-enabled textures are textures with .hd.EXT extensions with twice the size of the original texture.
	/// Ideally "april::getSystemInfo().max_texture.size > 1024" should be passed on as hdEnabled parameter if you are using HD textures that have any dimension higher than 1024 px.
	apriluiFnExport void init(bool hdEnabled = true);
	apriluiFnExport void destroy();
	apriluiFnExport void setLogFunction(void (*fnptr)(chstr));
	apriluiFnExport void log(chstr message, chstr prefix = "[aprilui] ");
	
	apriluiFnExport bool isDebugEnabled();
	apriluiFnExport void setDebugEnabled(bool value);
	
	apriluiFnExport float getDefaultScale();
	apriluiFnExport void setDefaultScale(float value);
	apriluiFnExport grect getViewport();
	apriluiFnExport void setViewport(grect value);
	apriluiFnExport grect getScreenViewport();
	apriluiFnExport void setScreenViewport(grect value);
	apriluiFnExport bool isLimitCursorToViewport();
	apriluiFnExport void setLimitCursorToViewport(bool value);
	apriluiFnExport bool isLimitCursorToScreenViewport();
	apriluiFnExport void setLimitCursorToScreenViewport(bool value);
	apriluiFnExport bool isHoverEffectEnabled();
	apriluiFnExport void setHoverEffectEnabled(bool value);
	apriluiFnExport hstr getDefaultTextsPath();
	apriluiFnExport void setDefaultTextsPath(chstr value);
	apriluiFnExport hstr getDefaultLocalization();
	apriluiFnExport void setDefaultLocalization(chstr value);
	apriluiFnExport hstr getLocalization();
	apriluiFnExport void setLocalization(chstr value);
	apriluiFnExport float getTextureIdleUnloadTime();
	apriluiFnExport void setTextureIdleUnloadTime(float value);
	apriluiFnExport hmap<hstr, Dataset*> getDatasets();

	apriluiFnExport void registerObjectFactory(chstr typeName, Object* (*factory)(chstr, grect));
	apriluiFnExport void registerAnimatorFactory(chstr typeName, Animator* (*factory)(chstr));
	apriluiFnExport Object* createObject(chstr type, chstr name, grect rect);
	apriluiFnExport Animator* createAnimator(chstr type, chstr name);
	
	apriluiFnExport gvec2 transformWindowPoint(gvec2 pt);
	apriluiFnExport void updateCursorPosition();
	apriluiFnExport gvec2 getCursorPosition();
	apriluiFnExport void setCursorPosition(gvec2 position);
	apriluiFnExport void setCursorImage(Image* image);
	apriluiFnExport void showCursor();
	apriluiFnExport void hideCursor();
	apriluiFnExport void drawCursor();
	
	apriluiFnExport Dataset* getDatasetByName(chstr name);
	apriluiFnExport void update(float k);
	apriluiFnExport void updateTextures(float k);
	apriluiFnExport void unloadUnusedTextures();
	apriluiFnExport void addTextureExtensionScale(chstr extension, float scale);
	apriluiFnExport float getTextureExtensionScale(chstr extensions);
	apriluiFnExport float findTextureExtensionScale(chstr filename);
	apriluiFnExport harray<hstr> getTextureExtensions();
	apriluiFnExport void setTextureExtensionScales(harray<hstr> extensions, harray<float> scales);
	
	apriluiFnExport void onMouseDown(int button);
	apriluiFnExport void onMouseUp(int button);
	apriluiFnExport void onMouseMove();
	apriluiFnExport void onMouseScroll(float x, float y);
	apriluiFnExport void onKeyDown(unsigned int keycode);
	apriluiFnExport void onKeyUp(unsigned int keycode);
	apriluiFnExport void onChar(unsigned int charcode);

	DEPRECATED_ATTRIBUTE apriluiFnExport bool isDebugMode();
	DEPRECATED_ATTRIBUTE apriluiFnExport void setDebugMode(bool value);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseDown(float x, float y, int button);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseUp(float x, float y, int button);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseMove(float x, float y);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnKeyDown(unsigned int keycode);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnKeyUp(unsigned int keycode);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnChar(unsigned int charcode);
	
	// TODO - hack, has to be removed completely
	apriluiFnExport bool getForcedDynamicLoading();
	apriluiFnExport void setForcedDynamicLoading(bool value);

}

#endif
