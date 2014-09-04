/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines all functions used in aprilui.

#ifndef APRILUI_H
#define APRILUI_H

#include <april/Keys.h>
#include <april/Texture.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Event.h"

#define APRILUI_REGISTER_OBJECT_TYPE(name) aprilui::registerObjectFactory(#name, &name::createInstance)
#define APRILUI_REGISTER_ANIMATOR_TYPE(name) aprilui::registerAnimatorFactory(#name, &name::createInstance)

namespace aprilui
{
	class Animator;
	class Dataset;
	class Image;
	class Object;

	static const char SeparatorParameter = ',';
	
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
	apriluiFnExport bool isDefaultManagedTextures();
	apriluiFnExport void setDefaultManagedTextures(bool value);
	apriluiFnExport april::Texture::LoadMode getDefaultTextureLoadMode();
	apriluiFnExport void setDefaultTextureLoadMode(april::Texture::LoadMode value);
	apriluiFnExport hmap<hstr, Dataset*> getDatasets();

	apriluiFnExport void registerObjectFactory(chstr typeName, Object* (*factory)(chstr, grect));
	apriluiFnExport void registerAnimatorFactory(chstr typeName, Animator* (*factory)(chstr));
	apriluiFnExport bool hasObjectFactory(chstr typeName);
	apriluiFnExport bool hasAnimatorFactory(chstr typeName);
	apriluiFnExport void unregisterObjectFactory(chstr typeName);
	apriluiFnExport void unregisterAnimatorFactory(chstr typeName);
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
	
	apriluiFnExport void notifyEvent(chstr type, EventArgs* args);
	apriluiFnExport void processEvents();
	apriluiFnExport void update(float timeDelta);
	apriluiFnExport void updateTextures(float timeDelta);
	apriluiFnExport void unloadUnusedTextures();
	apriluiFnExport void reloadTextures();

	apriluiFnExport bool setTextureExtensionPrefixes(harray<hstr> prefixes, harray<float> scales);
	apriluiFnExport void setTextureExtensionScales(harray<hstr> extensions, harray<float> scales);
	apriluiFnExport float getTextureExtensionScale(chstr extension);
	apriluiFnExport float findTextureExtensionScale(chstr filename);
	
	apriluiFnExport void onMouseDown(april::Key keyCode);
	apriluiFnExport void onMouseUp(april::Key keyCode);
	apriluiFnExport void onMouseCancel(april::Key keyCode);
	apriluiFnExport void onMouseMove();
	apriluiFnExport void onMouseScroll(float x, float y);
	apriluiFnExport void onKeyDown(april::Key keyCode);
	apriluiFnExport void onKeyUp(april::Key keyCode);
	apriluiFnExport void onChar(unsigned int charCode);
	apriluiFnExport void onTouch(const harray<gvec2>& touches);
	apriluiFnExport void onButtonDown(april::Button buttonCode);
	apriluiFnExport void onButtonUp(april::Button buttonCode);

	DEPRECATED_ATTRIBUTE apriluiFnExport bool getDefaultDynamicLoading();
	DEPRECATED_ATTRIBUTE apriluiFnExport void setDefaultDynamicLoading(bool value);

}

#endif
