/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "xmlHelper.h"
#include "Exception.h"

const char* xml_node::find_prop(const char* property, bool ignoreError)
{
	for (_xmlAttr* a = this->properties; a != NULL; a = a->next)
	{
		if (xmlStrcmp(a->name, (const xmlChar*)property) == 0)
		{
			return (const char*)a->children->content;
		}
	}
	if (!ignoreError)
	{
		throw aprilui::XMLPropertyNotExistsException(property, this);
	}
	return 0;
}

bool xml_node::pbool(const char* property)
{
	const char* nodeValue = this->find_prop(property);
	int i;
	sscanf(nodeValue, "%d", &i);
	return (i != 0);
}

bool xml_node::pbool(const char* property, bool defaultValue)
{
	const char* nodeValue = this->find_prop(property, true);
	if (nodeValue == NULL)
	{
		return defaultValue;
	}
	int i;
	sscanf(nodeValue, "%d", &i);
	return (i != 0);
}

int xml_node::pint(const char* property)
{
	const char* nodeValue = this->find_prop(property);
	int i;
	sscanf(nodeValue, "%d", &i);
	return i;
}

int xml_node::pint(const char* property, int defaultValue)
{
	const char* nodeValue = this->find_prop(property, true);
	if (nodeValue == NULL)
	{
		return defaultValue;
	}
	int i;
	sscanf(nodeValue, "%d", &i);
	return i;
}

float xml_node::pfloat(const char* property)
{
	const char* nodeValue = this->find_prop(property);
	float f;
	sscanf(nodeValue, "%f", &f);
	return f;
}

float xml_node::pfloat(const char* property, float defaultValue)
{
	const char* nodeValue = this->find_prop(property, true);
	if (nodeValue == NULL)
	{
		return defaultValue;
	}
	float f;
	sscanf(nodeValue, "%f", &f);
	return f;
}

hstr xml_node::pstr(const char* property)
{
	const char* nodeValue = this->find_prop(property);
	return nodeValue;
}

hstr xml_node::pstr(const char* property, chstr defaultValue)
{
	const char* nodeValue = this->find_prop(property, true);
	if (nodeValue == NULL)
	{
		return defaultValue;
	}
	return hstr(nodeValue);
}

bool xml_node::pexists(const char* property)
{
	return (this->find_prop(property, true) != 0);
}

bool xml_node::operator==(const char* s)
{
	return (xmlStrcmp(this->name, (const xmlChar*)s) == 0);
}

bool xml_node::operator!=(const char* s)
{
	return (xmlStrcmp(this->name, (const xmlChar*)s) != 0);
}

xml_node* xml_node::next()
{
	
	return (xml_node*)((_xmlNode*)this)->next;
}

xml_node* xml_node::iter_children()
{
	return (xml_node*)this->children;
}

xml_prop* xml_node::iter_properties()
{
	return (xml_prop*)this->properties;
}
/*****************************************************************************/
xml_prop* xml_prop::next()
{
	return (xml_prop*)((_xmlAttr*)this)->next;
}

hstr xml_prop::name()
{
	return hstr((const char*)((_xmlAttr*)this)->name);
}

hstr xml_prop::value()
{
	return hstr((const char*)this->children->content);
}
/*****************************************************************************/
xml_doc::xml_doc(chstr filename)
{
	this->doc = xmlParseFile((filename).c_str());
	if (this->doc == NULL)
	{
		throw aprilui::GenericException("Unable to parse xml file '" + filename + "', document does not exist or is invalid");
	}
}

xml_doc::~xml_doc()
{
	xmlFreeDoc(this->doc);
}

xml_node* xml_doc::root(chstr rootElementQuery)
{
	xml_node* root = (xml_node*)xmlDocGetRootElement(this->doc);
	if (root == NULL)
	{
		hstr docname = (char*)this->doc->URL;
		xmlFreeDoc(this->doc);
		throw aprilui::GenericException("No root node found in xml file '" + docname + "'");
	}
	if (rootElementQuery != "" && *root != rootElementQuery.c_str())
	{
		hstr docname = (char*)this->doc->URL;
		xmlFreeDoc(this->doc);
		throw aprilui::GenericException("xml root node type is not '" + rootElementQuery + "' in xml file '" + docname + "'");
	}
	return root;
}


