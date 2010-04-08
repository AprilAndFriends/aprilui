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
	std::string xmlGetPropString(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		std::string value = nodeValue;
		xmlFree(nodeValue);
		
		return value;
	}

	float xmlGetPropFloat(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		float x=0;
		sscanf_s(nodeValue,"%f",&x);
		xmlFree(nodeValue);
		
		return x;
	}

	int xmlGetPropInt(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		int x=0;
		sscanf_s(nodeValue,"%i",&x);
		xmlFree(nodeValue);
		
		return x;
	}

	unsigned int xmlGetPropHex(_xmlNode* node,const char* prop)
	{
		char* nodeValue = (char*) xmlGetProp(node, (xmlChar*) prop);
		if (!nodeValue) throw XMLPropertyNotExistsException(prop,node);
		unsigned int x=0;
		if (nodeValue[0] == '0' && nodeValue[1] == 'x')
			sscanf_s(nodeValue+2,"%x",&x);
		else sscanf_s(nodeValue,"%x",&x);
		xmlFree(nodeValue);
		
		return x;
	}

	bool xmlPropertyExists(_xmlNode* node,const char* name)
	{
		return xmlGetProp(node, (const xmlChar*) name) != 0;
	}

	std::string str(int i)
	{
		char s[32];
		sprintf_s(s,"%d",i);
		return std::string(s);
	}

	int str_to_int(std::string s)
	{
		int i;
		sscanf_s(s.c_str(),"%d",&i);
		return i;
	}

	float str_to_float(std::string s)
	{
		float f;
		sscanf_s(s.c_str(),"%f",&f);
		return f;
	}

	int hexstr_to_int(const std::string& s)
	{
		int i;
		sscanf_s(s.c_str(),"%x",&i);
		return i;
	}

	std::string str_toupper(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	bool str_split(std::string s,std::string splitter,std::string& out_left,std::string& out_right)
	{
		int index=s.find(splitter);
		if (index < 0) return 0;
		
		out_left=s.substr(0,index);
		out_right=s.substr(index+splitter.size(),1000);
		
		return 1;
	}

	std::vector<std::string> str_split(std::string s,std::string splitter)
	{
		std::vector<std::string> lst;
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

	std::string remove_spaces(std::string s)
	{
		std::string out="";
		for(std::string::iterator it=s.begin();it!=s.end();it++)
		{
			if (*it != ' ') out+=*it;
		}
		return out;
	}

	bool startswith(std::string s,std::string with_what)
	{
		return (s.substr(0,with_what.size()) == with_what);
	}

	bool endswith(std::string s,std::string with_what)
	{
		int size=with_what.size();
		return (s.substr(s.size()-size,size) == with_what);
	}

	std::string pathGetFilename(std::string path,bool with_suffix)
	{
		int index1=path.rfind("/"); if (index1 < 0) index1=path.rfind("\\");
		int index2=(with_suffix) ? path.size() : path.rfind(".");
		std::string name=path.substr(index1+1,index2-index1-1);
		return name;
	}

	std::string pathGetBaseDir(std::string path)
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

void hexstr_to_argb(const std::string& hex,byte* a,byte* r,byte* g,byte* b)
{
	std::string h=(hex.substr(0,2) == "0x") ? hex : "0x"+hex;
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

	void hexstr_to_argb_float(const std::string& hex,float* a,float* r,float* g,float* b)
	{
		byte ba,br,bg,bb;
		hexstr_to_argb(hex,&ba,&br,&bg,&bb);
		*a=ba/255.0f;
		*r=br/255.0f;
		*g=bg/255.0f;
		*b=bb/255.0f;
	}

	std::string generateName(std::string prefix)
	{
		static std::map<std::string,int> counters;
		int cnt=counters[prefix];
		cnt++;
		counters[prefix]=cnt;
		return prefix+str(cnt);
	}
	
	int getdir (std::string dir, std::vector<std::string> &files)
	{
		DIR *dp;
		struct dirent *dirp;
		if((dp  = opendir(dir.c_str())) == NULL) {
			//std::cout << "Error(" << errno << ") opening " << dir << std::endl;
			return 1;
		}

		std::string x;
		while ((dirp = readdir(dp)) != NULL) {
			x=std::string(dirp->d_name);
			if (x == "." || x == ".." || x == ".svn") continue;
			files.push_back(getPWD() + "/" + dir + "/" + x);
		}
		closedir(dp);
		return 0;
	}
	
	std::string getPWD()
	{
		return ".";
	}
	
	void writelog(std::string msg)
	{
		rendersys->logMessage(msg);
	}
}
