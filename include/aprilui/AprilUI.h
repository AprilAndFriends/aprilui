/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_H
#define APRILUI_H

#include <hltypes/hstring.h>
#include <gtypes/Vector2.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	class Dataset;
	
	AprilUIFnExport void init();
	AprilUIFnExport void destroy();
	void logMessage(chstr message);
	
	//! used to determine width and height of objects who don't have w & h params in xml
	AprilUIFnExport void setDefaultScale(float value);
	AprilUIFnExport float getDefaultScale();
	
	AprilUIFnExport Dataset* getDatasetByName(chstr name);
	AprilUIFnExport void update(float time_increase);
	
	AprilUIFnExport void setCursorPos(float x,float y);
	AprilUIFnExport gvec2 getCursorPos();
}

#endif
