/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "xmlHelper.h"
#include "Exception.h"

const char* xml_node::find_prop(const char* property)
{
	for (_xmlAttr* a=this->properties;a != 0;a=a->next)
		if (xmlStrcmp(a->name, (const xmlChar *) property) == 0)
			return (const char*) a->children->content;
	
	return 0;
}

bool xml_node::pbool(const char* property)
{
	int i;
	const char* prop=this->find_prop(property);
	if (!prop) throw AprilUI::XMLPropertyNotExistsException(property,this);
	sscanf(prop,"%d",&i);
	return (i != 0);
}

bool xml_node::pbool(const char* property, bool defaultValue)
{
	const char* nodeValue=this->find_prop(property);
	if (!nodeValue)
	{
		return defaultValue;
	}
	int i;
	sscanf(nodeValue,"%d",&i);
	return (i != 0);
}

int xml_node::pint(const char* property)
{
	int i;
	const char* prop=this->find_prop(property);
	if (!prop) throw AprilUI::XMLPropertyNotExistsException(property,this);
	sscanf(prop,"%d",&i);
	return i;
}

int xml_node::pint(const char* property, int defaultValue)
{
	const char* nodeValue=this->find_prop(property);
	if (!nodeValue)
	{
		return defaultValue;
	}
	int i;
	sscanf(nodeValue,"%d",&i);
	return i;
}

float xml_node::pfloat(const char* property)
{
	float f;
	const char* prop=this->find_prop(property);
	if (!prop) throw AprilUI::XMLPropertyNotExistsException(property,this);
	sscanf(prop,"%f",&f);
	return f;
}

float xml_node::pfloat(const char* property, float defaultValue)
{
	const char* nodeValue=this->find_prop(property);
	if (!nodeValue)
	{
		return defaultValue;
	}
	float f;
	sscanf(nodeValue,"%f",&f);
	return f;
}

hstr xml_node::pstr(const char* property)
{
	const char* prop=this->find_prop(property);
	if (!prop) throw AprilUI::XMLPropertyNotExistsException(property,this);
	return prop;
}

hstr xml_node::pstr(const char* property, chstr defaultValue)
{
	const char* nodeValue=this->find_prop(property);
	if (!nodeValue)
	{
		return defaultValue;
	}
	return hstr(nodeValue);
}

unsigned int xml_node::phex(const char* property)
{
	const char* prop=this->find_prop(property);
	if (!prop) throw AprilUI::XMLPropertyNotExistsException(property,this);
	const char* nodeValue=prop;
	unsigned int x=0;
	if (nodeValue[0] == '0' && nodeValue[1] == 'x')
		sscanf(nodeValue+2,"%x",&x);
	else sscanf(nodeValue,"%x",&x);
	return x;
}

unsigned int xml_node::phex(const char* property, unsigned int defaultValue)
{
	const char* nodeValue=this->find_prop(property);
	if (!nodeValue)
	{
		return defaultValue;
	}
	unsigned int x=0;
	if (nodeValue[0] == '0' && nodeValue[1] == 'x')
		sscanf(nodeValue+2,"%x",&x);
	else sscanf(nodeValue,"%x",&x);
	return x;
}

bool xml_node::pexists(const char* property)
{
	return (this->find_prop(property) != 0);
}

bool xml_node::operator ==(const char* s)
{
	return (xmlStrcmp(this->name, (const xmlChar *) s) == 0);
}

bool xml_node::operator !=(const char* s)
{
	return (xmlStrcmp(this->name, (const xmlChar *) s) != 0);
}

xml_node* xml_node::next()
{
	
	return (xml_node*) ((_xmlNode*) this)->next;
}

xml_node* xml_node::iter_children()
{
	return (xml_node*) this->children;
}

xml_prop* xml_node::iter_properties()
{
	return (xml_prop*) this->properties;
}
/*****************************************************************************/
xml_prop* xml_prop::next()
{
	return (xml_prop*) ((_xmlAttr*) this)->next;
}

hstr xml_prop::name()
{
	return hstr((const char*) ((_xmlAttr*) this)->name);
}

hstr xml_prop::value()
{
	return hstr((const char*) this->children->content);
}
/*****************************************************************************/
xml_doc::xml_doc(chstr filename)
{
	this->doc=xmlParseFile((filename).c_str());
	if (this->doc == NULL)
		throw AprilUI::GenericException("Unable to parse xml file '"+filename+"', document does not exist or is invalid");
}

xml_doc::~xml_doc()
{
	xmlFreeDoc(this->doc);
}

xml_node* xml_doc::root(chstr root_element_query)
{
	xml_node* root=(xml_node*) xmlDocGetRootElement(this->doc);
	if (!root)
	{
		hstr docname=(char*) this->doc->URL;
		xmlFreeDoc(this->doc);
		throw AprilUI::GenericException("No root node found in xml file '"+docname+"'");
	}
	if (root_element_query != "" && *root != root_element_query.c_str())
	{
		hstr docname=(char*) this->doc->URL;
		xmlFreeDoc(this->doc);
		throw AprilUI::GenericException("xml root node type is not '"+root_element_query+"' in xml file '"+docname+"'");
	}
	return root;
}


