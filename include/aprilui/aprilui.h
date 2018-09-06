/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_H
#define APRILUI_H

#define APRILUI_VER 1.4
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	
	apriluiFnExport void init();
	apriluiFnExport void destroy();
	void log(chstr message, chstr prefix = "aprilui");
	void logMessage(chstr message, chstr prefix = "aprilui");
	void aprilui_writelog(chstr message);
	
#ifdef _DEBUG
	apriluiFnExport void setDebugMode(bool value);
	apriluiFnExport bool isDebugMode();
#endif
	
	//! used to determine width and height of objects that don't have w & h params in xml
	apriluiFnExport void setDefaultScale(float value);
	apriluiFnExport float getDefaultScale();
	
	apriluiFnExport bool hasDataset(chstr name);
	apriluiFnExport Dataset* getDatasetByName(chstr name);
	apriluiFnExport void update(float time);
	apriluiFnExport void updateTextures(float time);
	apriluiFnExport hstr getDefaultTextsPath();
	apriluiFnExport void setDefaultTextsPath(chstr path);
	apriluiFnExport hstr getLocalization();
	apriluiFnExport void setLocalization(chstr localization);
	apriluiFnExport float getTextureIdleUnloadTime();
	apriluiFnExport void setTextureIdleUnloadTime(float value);
	
	apriluiFnExport gvec2f getCursorPosition();
	apriluiFnExport void setCursorPosition(gvec2f position);
	apriluiFnExport void setCursorPosition(float x, float y);
	apriluiFnExport grectf getViewport();
	apriluiFnExport void setViewport(grectf value);
	
	apriluiFnExport void OnMouseDown(float x, float y, int button);
	apriluiFnExport void OnMouseUp(float x, float y, int button);
	apriluiFnExport void OnMouseMove(float x, float y);
	apriluiFnExport void OnKeyDown(unsigned int keyCode);
	apriluiFnExport void OnKeyUp(unsigned int keyCode);
	apriluiFnExport void OnChar(unsigned int charCode);
		
	apriluiFnExport void setMacroCallback(hstr (*callback)(chstr));
	apriluiFnExport hstr expandMacro(chstr macro);

	// TODO - hack, has to be removed completely
	apriluiFnExport bool getForcedDynamicLoading();
	apriluiFnExport void setForcedDynamicLoading(bool value);

}

#endif
