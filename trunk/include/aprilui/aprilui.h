/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_H
#define APRILUI_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	class Image;
	
	apriluiFnExport void init();
	apriluiFnExport void destroy();
	apriluiFnExport void setLogFunction(void (*fnptr)(chstr));
	apriluiFnExport void log(chstr message, chstr prefix = "[aprilui] ");
	
	apriluiFnExport void setDebugMode(bool value);
	apriluiFnExport bool isDebugMode();
	
	//! used to determine width and height of objects that don't have w & h params in xml
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
	
    apriluiFnExport void updateCursorPosition();
    apriluiFnExport void setCursorPosition(gvec2 position);
	apriluiFnExport gvec2 getCursorPosition();
	apriluiFnExport void setCursorImage(Image* image);
	apriluiFnExport void showCursor();
	apriluiFnExport void hideCursor();
	apriluiFnExport void drawCursor();
	
	apriluiFnExport Dataset* getDatasetByName(chstr name);
	apriluiFnExport void update(float time);
	apriluiFnExport void updateTextures(float time);
	apriluiFnExport void unloadUnusedTextures();
	apriluiFnExport hstr getDefaultTextsPath();
	apriluiFnExport void setDefaultTextsPath(chstr path);
#ifndef NO_PARTICLE
	apriluiFnExport hstr getDefaultParticlesPath();
	apriluiFnExport void setDefaultParticlesPath(chstr path);
#endif
	apriluiFnExport hstr getLocalization();
	apriluiFnExport void setLocalization(chstr localization);
	
	apriluiFnExport void onMouseDown(float x, float y, int button);
	apriluiFnExport void onMouseUp(float x, float y, int button);
	apriluiFnExport void onMouseMove(float x, float y);
	apriluiFnExport void onKeyDown(unsigned int keycode);
	apriluiFnExport void onKeyUp(unsigned int keycode);
	apriluiFnExport void onChar(unsigned int charcode);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseDown(float x, float y, int button);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseUp(float x, float y, int button);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnMouseMove(float x, float y);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnKeyDown(unsigned int keycode);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnKeyUp(unsigned int keycode);
	DEPRECATED_ATTRIBUTE apriluiFnExport void OnChar(unsigned int charcode);
	
}

#endif
