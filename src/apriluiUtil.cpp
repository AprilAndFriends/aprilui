/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
