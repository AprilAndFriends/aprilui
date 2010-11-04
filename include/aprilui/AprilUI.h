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

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	class Dataset;
	
	AprilUIFnExport void init();
	AprilUIFnExport void destroy();
	AprilUIFnExport void setLogFunction(void (*fnptr)(chstr));
	void logMessage(chstr message, chstr prefix = "[aprilui] ");
	void aprilui_writelog(chstr message);
	
#ifdef _DEBUG
	AprilUIFnExport void setDebugMode(bool value);
	AprilUIFnExport bool isDebugMode();
#endif
	
	//! used to determine width and height of objects who don't have w & h params in xml
	AprilUIFnExport void setDefaultScale(float value);
	AprilUIFnExport float getDefaultScale();
	
	AprilUIFnExport Dataset* getDatasetByName(chstr name);
	AprilUIFnExport void update(float time);
	AprilUIFnExport void updateTextures(float time);
	AprilUIFnExport hstr getDefaultTextsPath();
	AprilUIFnExport void setDefaultTextsPath(chstr path);
	
	AprilUIFnExport void setCursorPos(float x, float y);
	AprilUIFnExport gvec2 getCursorPos();
}

#endif
