/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef XML_HELPER_H
#define XML_HELPER_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <hltypes/hstring.h>

#include "AprilUIExport.h"

class AprilUIExport xml_prop : public _xmlAttr
{
public:
	xml_prop* next();
	hstr name();
	hstr value();
};

struct AprilUIExport xml_node : public _xmlNode
{
	const char* find_prop(const char* property);
	
public:
	bool pbool(const char* property);
	int pint(const char* property);
	float pfloat(const char* property);
	hstr pstr(const char* property);
	unsigned int phex(const char* property);
	
	bool pexists(const char* property);
	
	xml_node* next();
	xml_node* iter_children();
	xml_prop* iter_properties();
	
	bool operator ==(const char* s);
	bool operator !=(const char* s);
};



class AprilUIExport xml_doc
{
	xmlDocPtr doc;
public:
	xml_doc(chstr filename);
	~xml_doc();
	xml_node* root(chstr root_element_query="");
};


#endif
