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
	apriluiFnExport void logf(chstr message, ...);
	
#ifdef _DEBUG
	apriluiFnExport void setDebugMode(bool value);
	apriluiFnExport bool isDebugMode();
#endif
	
	//! used to determine width and height of objects that don't have w & h params in xml
	apriluiFnExport float getDefaultScale();
	apriluiFnExport void setDefaultScale(float value);
	apriluiFnExport grect getViewport();
	apriluiFnExport void setViewport(grect value);
	apriluiFnExport bool isLimitCursorToViewport();
	apriluiFnExport void setLimitCursorToViewport(bool value);
	
	apriluiFnExport gvec2 getCursorPosition();
	apriluiFnExport void setCursorImage(Image* image);
	apriluiFnExport void showCursor();
	apriluiFnExport void hideCursor();
	apriluiFnExport void drawCursor();
	
	apriluiFnExport Dataset* getDatasetByName(chstr name);
	apriluiFnExport void update(float time);
	apriluiFnExport void updateTextures(float time);
	apriluiFnExport hstr getDefaultTextsPath();
	apriluiFnExport void setDefaultTextsPath(chstr path);
	
	apriluiFnExport void OnMouseDown(float x, float y, int button);
	apriluiFnExport void OnMouseUp(float x, float y, int button);
	apriluiFnExport void OnMouseMove(float x, float y);
	apriluiFnExport void OnKeyDown(unsigned int keycode);
	apriluiFnExport void OnKeyUp(unsigned int keycode);
	apriluiFnExport void OnChar(unsigned int charcode);
	
}

#endif
