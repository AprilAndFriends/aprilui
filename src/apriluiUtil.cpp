/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <algorithm>
#include <math.h>
#include <stdio.h>

#include <april/RenderSystem.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hdir.h>
#include <hltypes/hmap.h>

#include "apriluiUtil.h"
#include "Exception.h"

namespace aprilui
{
	const char* logTag = "aprilui";
	
	hstr pathGetBaseDir(chstr path)
	{
		hstr normalized = hdir::normalize(path);
		int index = normalized.rindexOf('/');
		return (index < 0 ? "" : normalized(0, index));
	}

	hstr generateName(chstr prefix)
	{
		static hmap<hstr, int> counters;
		int count = counters[prefix] + 1;
		counters[prefix] = count;
		return prefix + hstr(count);
	}
	
	hstr getPWD()
	{
		return ".";
	}
	
}
