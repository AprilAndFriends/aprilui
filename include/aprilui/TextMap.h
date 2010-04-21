/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef TEXT_MAP_H
#define TEXT_MAP_H

#include "hltypes/hstring.h"
#include <map>
#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport TextMap
	{
		char* mBuffer;
		int mBufferPos;
		std::map<hstr,unsigned int> mTexts;
		
	public:
		TextMap();
		~TextMap();
		
		void load(chstr folder);
		void destroy();
		bool exists(chstr name);

		const char* operator [] (chstr name);
		
	};
}
#endif
