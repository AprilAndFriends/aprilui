/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <algorithm>
#include <math.h>
#include <stdio.h>
#ifdef _WIN32
	#include "_dirent.h"
#else
	#include <dirent.h>
#endif

#include <april/RenderSystem.h>
#include <hltypes/hmap.h>

#include "Util.h"
#include "Exception.h"

namespace AprilUI
{
	hstr pathGetFilename(chstr path,bool with_suffix)
	{
		int index1=path.rfind("/"); if (index1 < 0) index1=path.rfind("\\");
		int index2=(with_suffix) ? path.size() : path.rfind(".");
		hstr name=path(index1+1,index2-index1-1);
		return name;
	}

	hstr pathGetBaseDir(chstr path)
	{
		int index=path.rfind("/");  if (index < 0) index=path.rfind("\\");
		return path(0,index);
	}

	int hexstr_to_int(chstr s)
	{
		int i;
		sscanf(s.c_str(),"%x",&i);
		return i;
	}

	void hexstr_to_argb(chstr hex,byte* a,byte* r,byte* g,byte* b)
	{
		hstr h=(hex(0,2) == "0x") ? hex : "0x"+hex;
		if (h.size() == 8)
		{
			*r=hexstr_to_int(h(2,2));
			*g=hexstr_to_int(h(4,2));
			*b=hexstr_to_int(h(6,2));
			*a=255;
		}
		else if (h.size() == 10)
		{
			*r=hexstr_to_int(h(4,2));
			*g=hexstr_to_int(h(6,2));
			*b=hexstr_to_int(h(8,2));
			*a=hexstr_to_int(h(2,2));
		}
		else throw AprilUI::GenericException("Color format must be either 0xAARRGGBB or 0xRRGGBB");
	}

	void hexstr_to_argb_float(chstr hex,float* a,float* r,float* g,float* b)
	{
		byte ba,br,bg,bb;
		hexstr_to_argb(hex,&ba,&br,&bg,&bb);
		*a=ba/255.0f;
		*r=br/255.0f;
		*g=bg/255.0f;
		*b=bb/255.0f;
	}

	hstr generateName(chstr prefix)
	{
		static hmap<hstr,int> counters;
		int cnt=counters[prefix];
		cnt++;
		counters[prefix]=cnt;
		return prefix+hstr(cnt);
	}
	
	int getdir (chstr dir, harray<hstr> &files)
	{
		DIR *dp;
		struct dirent *dirp;
		if((dp  = opendir(dir.c_str())) == NULL) {
			//std::cout << "Error(" << errno << ") opening " << dir << std::endl;
			return 1;
		}

		hstr x;
		while ((dirp = readdir(dp)) != NULL) {
			x=hstr(dirp->d_name);
			if (x == "." || x == ".." || x == ".svn") continue;
			files.push_back(getPWD() + "/" + dir + "/" + x);
		}
		closedir(dp);
		return 0;
	}
	
	hstr getPWD()
	{
		return ".";
	}
	
}
