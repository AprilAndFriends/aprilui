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
#ifndef APRILUI_UTIL_H
#define UTIL_H

#include <string>
#include <vector>
struct _xmlNode;

namespace AprilUI
{
	typedef unsigned char byte;

	#define XML_EQ(a,b) xmlStrcmp(a->name, (const xmlChar *) b) == 0

	#define foreach(type,lst) for (std::list<type>::iterator it=lst.begin();it != lst.end(); it++)
	#define foreach_reverse(type,lst) for (std::list<type>::reverse_iterator it=lst.rbegin();it != lst.rend(); it++)
	#define foreach_v(type,lst) for (std::vector<type>::iterator it=lst.begin();it != lst.end(); it++)
	#define foreach_rv(type,lst) for (std::vector<type>::reverse_iterator it=lst.rbegin();it != lst.rend(); it++)

	#define foreach_in_map(type,list) for (std::map<std::string,type>::iterator it=list.begin();it != list.end(); it++)

	float sign(float number);
	std::string xmlGetPropString(_xmlNode* node,const char* prop);
	float xmlGetPropFloat(_xmlNode* node,const char* prop);
	int xmlGetPropInt(_xmlNode* node,const char* prop);
	bool xmlPropertyExists(_xmlNode* node,const char* name);
	unsigned int xmlGetPropHex(_xmlNode* node,const char* prop);
	std::string str(int i);
	std::string str_toupper(std::string s);
	int str_to_int(std::string s);
	float str_to_float(std::string s);
	bool str_split(std::string s,std::string splitter,std::string& out_left,std::string& out_right);
	std::vector<std::string> str_split(std::string s,std::string splitter);


	bool startswith(std::string s,std::string with_what);
	bool endswith(std::string s,std::string with_what);
	std::string remove_spaces(std::string s);

	std::string pathGetFilename(std::string path,bool with_suffix=1);
	std::string pathGetBaseDir(std::string path);

	int hexstr_to_int(const std::string& s);
	void hexstr_to_argb(const std::string& hex,byte* a,byte* r,byte* g,byte* b);
	void hexstr_to_argb_float(const std::string& hex,float* a,float* r,float* g,float* b);

	float Sin(float angle);
	float Cos(float angle);

	float clamp(float value,float min_value,float max_value);

	std::string generateName(std::string prefix);
	int getdir (std::string dir, std::vector<std::string> &files);
	
	void writelog(std::string msg);
	
	std::string getPWD();
}
#endif
