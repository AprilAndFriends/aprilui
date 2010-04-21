/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_H
#define APRILUI_H

#include "AprilUIExport.h"
#include "hltypes/hstring.h"

namespace AprilUI
{
	class Dataset;
	
	void AprilUIExport init();
	void AprilUIExport destroy();
	
	Dataset* AprilUIFnExport getDatasetByName(hstr name);
	void AprilUIFnExport update(float time_increase);
}

#endif
