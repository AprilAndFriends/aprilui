/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.11
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"

namespace aprilui
{
	hstr generateName(chstr prefix)
	{
		static hmap<hstr, int> counters;
		int count = counters[prefix] + 1;
		counters[prefix] = count;
		return prefix + hstr(count);
	}
	
	hstr gvec2_to_hstr(gvec2 vector)
	{
		return hsprintf("%f%c%f", vector.x, aprilui::SeparatorParameter, vector.y);
	}
	
	hstr grect_to_hstr(grect rect)
	{
		return hsprintf("%f%c%f%c%f%c%f", rect.x, aprilui::SeparatorParameter, rect.y, aprilui::SeparatorParameter, rect.w, aprilui::SeparatorParameter, rect.h);
	}
	
	gvec2 hstr_to_gvec2(chstr string)
	{
		harray<hstr> data = string.split(aprilui::SeparatorParameter);
		if (data.size() != 2)
		{
			throw hl_exception("Cannot convert string '" + string + "' to gtypes::Vector2.");
		}
		return gvec2(data[0].trim(), data[1].trim());
	}
	
	grect hstr_to_grect(chstr string)
	{
		harray<hstr> data = string.split(aprilui::SeparatorParameter);
		if (data.size() != 4)
		{
			throw hl_exception("Cannot convert string '" + string + "' to gtypes::Rectangle.");
		}
		return grect(data[0].trim(), data[1].trim(), data[2].trim(), data[3].trim());
	}
	
}
