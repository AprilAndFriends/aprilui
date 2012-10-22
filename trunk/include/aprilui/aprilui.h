/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.25
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
	
	extern hstr logTag;

	apriluiFnExport void init();
	apriluiFnExport void destroy();
	
	apriluiFnExport bool isDebugEnabled();
	apriluiFnExport void setDebugEnabled(bool value);
	
	apriluiFnExport float getDefaultScale();
	apriluiFnExport void setDefaultScale(float value);
	apriluiFnExport grect getViewport();
	apriluiFnExport void setViewport(grect value);
	apriluiFnExport bool isLimitCursorToViewport();
	apriluiFnExport void setLimitCursorToViewport(bool value);
	apriluiFnExport bool isHoverEffectEnabled();
	apriluiFnExport void setHoverEffectEnabled(bool value);
	apriluiFnExport hstr getDefaultTextsPath();
	apriluiFnExport void setDefaultTextsPath(chstr value);
	apriluiFnExport hstr getDefaultLocalization();
	apriluiFnExport void setDefaultLocalization(chstr value);
	apriluiFnExport hstr getLocalization();
	apriluiFnExport void setLocalization(chstr value);
	apriluiFnExport harray<hstr> getSupportedLocalizations();
	apriluiFnExport void setSupportedLocalizations(harray<hstr> value);
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
	apriluiFnExport void reloadTextures();

	apriluiFnExport bool setTextureExtensionPrefixes(harray<hstr> prefixes, harray<float> scales);
	apriluiFnExport void setTextureExtensionScales(harray<hstr> extensions, harray<float> scales);
	apriluiFnExport float getTextureExtensionScale(chstr extension);
	apriluiFnExport float findTextureExtensionScale(chstr filename);
	
	apriluiFnExport void onMouseDown(int button);
	apriluiFnExport void onMouseUp(int button);
	apriluiFnExport void onMouseMove();
	apriluiFnExport void onMouseScroll(float x, float y);
	apriluiFnExport void onKeyDown(unsigned int keyCode);
	apriluiFnExport void onKeyUp(unsigned int keyCode);
	apriluiFnExport void onChar(unsigned int charCode);

	DEPRECATED_ATTRIBUTE apriluiFnExport bool isDebugMode();
	DEPRECATED_ATTRIBUTE apriluiFnExport void setDebugMode(bool value);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseDown(float x, float y, int button);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseUp(float x, float y, int button);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseMove(float x, float y);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnKeyDown(unsigned int keyCode);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnKeyUp(unsigned int keyCode);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnChar(unsigned int charCode);
	
	DEPRECATED_ATTRIBUTE apriluiFnExport void setLogFunction(void (*fnptr)(chstr));
	DEPRECATED_ATTRIBUTE apriluiFnExport void log(chstr message, chstr prefix = "[aprilui] ");

	// TODO - hack, has to be removed completely
	apriluiFnExport bool getForcedDynamicLoading();
	apriluiFnExport void setForcedDynamicLoading(bool value);

}

#endif
