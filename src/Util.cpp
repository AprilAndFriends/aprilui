/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#include <algorithm>
#include <math.h>
#ifdef _WIN32
	#include "_dirent.h"
#else
	#include <dirent.h>
#endif
#include <map>
#include "Util.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "Exception.h"
#include "april/RenderSystem.h"
namespace AprilUI
{
	hstr xmlGetPropString(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		hstr value = nodeValue;
		xmlFree(nodeValue);
		
		return value;
	}

	float xmlGetPropFloat(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		float x=0;
		sscanf(nodeValue,"%f",&x);
		xmlFree(nodeValue);
		
		return x;
	}

	int xmlGetPropInt(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		int x=0;
		sscanf(nodeValue,"%i",&x);
		xmlFree(nodeValue);
		
		return x;
	}

	unsigned int xmlGetPropHex(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		unsigned int x=0;
		if (nodeValue[0] == '0' && nodeValue[1] == 'x')
			sscanf(nodeValue+2,"%x",&x);
		else sscanf(nodeValue,"%x",&x);
		xmlFree(nodeValue);
		
		return x;
	}

	bool xmlPropertyExists(_xmlNode* node,const char* name)
	{
		return xmlGetProp(node, (const xmlChar*) name) != 0;
	}

	std::vector<hstr> str_split(hstr s,hstr splitter)
	{
		std::vector<hstr> lst;
		int index=0,last=0;
		for (;;)
		{
			index=s.find(splitter,last);
			if (index < 0) break;
			lst.push_back(s.substr(last,index-last));
			last=index+splitter.size();
		}
		if (last < (int)s.size()) lst.push_back(s.substr(last,s.size()));
		
		return lst;
	}

	hstr pathGetFilename(hstr path,bool with_suffix)
	{
		int index1=path.rfind("/"); if (index1 < 0) index1=path.rfind("\\");
		int index2=(with_suffix) ? path.size() : path.rfind(".");
		hstr name=path.substr(index1+1,index2-index1-1);
		return name;
	}

	hstr pathGetBaseDir(hstr path)
	{
		int index=path.rfind("/");  if (index < 0) index=path.rfind("\\");
		return path.substr(0,index);
	}

	float sign(float number)
	{
		if (number >= 0) return 1;
		else             return -1;
	}

	float Sin(float angle)
	{
		return sin(angle/57.295779513f);
	}

	float Cos(float angle)
	{
		return cos(angle/57.295779513f);
	}

	float clamp(float value,float min_value,float max_value)
	{
		if      (value < min_value) value=min_value;
		else if (value > max_value) value=max_value;
		return value;
	}

	int hexstr_to_int(chstr s)
	{
		int i;
		sscanf(s.c_str(),"%x",&i);
		return i;
	}

	void hexstr_to_argb(chstr hex,byte* a,byte* r,byte* g,byte* b)
	{
		hstr h=(hex.substr(0,2) == "0x") ? hex : "0x"+hex;
		if (h.size() == 8)
		{
			*r=hexstr_to_int(h.substr(2,2));
			*g=hexstr_to_int(h.substr(4,2));
			*b=hexstr_to_int(h.substr(6,2));
			*a=255;
		}
		else if (h.size() == 10)
		{
			*r=hexstr_to_int(h.substr(4,2));
			*g=hexstr_to_int(h.substr(6,2));
			*b=hexstr_to_int(h.substr(8,2));
			*a=hexstr_to_int(h.substr(2,2));
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

	hstr generateName(hstr prefix)
	{
		static std::map<hstr,int> counters;
		int cnt=counters[prefix];
		cnt++;
		counters[prefix]=cnt;
		return prefix+hstr(cnt);
	}
	
	int getdir (hstr dir, std::vector<hstr> &files)
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
	
	void writelog(hstr msg)
	{
		rendersys->logMessage(msg);
	}
}
