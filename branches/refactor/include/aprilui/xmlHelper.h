/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_XML_HELPER_H
#define APRILUI_XML_HELPER_H

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
public:
	bool pbool(const char* property);
	bool pbool(const char* property, bool defaultValue);
	int pint(const char* property);
	int pint(const char* property, int defaultValue);
	float pfloat(const char* property);
	float pfloat(const char* property, float defaultValue);
	hstr pstr(const char* property);
	hstr pstr(const char* property, chstr defaultValue);
	unsigned int phex(const char* property);
	unsigned int phex(const char* property, unsigned int defaultValue);
	
	bool pexists(const char* property);
	
	xml_node* next();
	xml_node* iter_children();
	xml_prop* iter_properties();
	
	bool operator ==(const char* s);
	bool operator !=(const char* s);
	
protected:
	const char* find_prop(const char* property, bool ignoreError = false);
	
};

class AprilUIExport xml_doc
{
public:
	xml_doc(chstr filename);
	~xml_doc();
	xml_node* root(chstr rootElementQuery = "");
	
protected:
	xmlDocPtr doc;
	
};

#endif
