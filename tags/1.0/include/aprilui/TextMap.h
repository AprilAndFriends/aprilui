/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TEXT_MAP_H
#define APRILUI_TEXT_MAP_H

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport TextMap
	{
		char* mBuffer;
		int mBufferPos;
		hstr mName;
		hmap<hstr,unsigned int> mTexts;
		
	public:
		TextMap();
		~TextMap();
		
		void load(chstr folder);
		void unload();
		bool exists(chstr name);

		const char* operator [] (chstr name);
		
	};
}
#endif
