/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <xmlHelper.h>
#include "Exception.h"
#include "Dataset.h"
#include "Util.h"

namespace AprilUI
{
	_GenericException::_GenericException(chstr errorText,chstr type,chstr file,int line)
	{
		hstr filename = file;
		mErrText = errorText;
		int src=filename.find("src");
		if (src >= 0) filename=filename(src+4,1000);
		mLineNumber=line;
		mFile=filename;
	}
	
	_GenericException::~_GenericException()
	{
		
	}


	hstr _GenericException::repr()
	{
		hstr text=getType();
		if (text != "") text+=": ";

		if (mFile != "") text+="["+mFile+":"+hstr(mLineNumber)+"] - ";

		return text + getErrorText();
	}

	void _GenericException::writeOutput()
	{
		writelog("\n----------------\nException Error!\n\n"+repr()+"\n----------------");
	}


	_ResourceExistsException::_ResourceExistsException(chstr object_name,chstr class_name,
													   Dataset* dict,chstr file,int line) :
							  _GenericException("","ResourceExistsException",file,line)
	{
		mErrText=class_name+" already exists: "+object_name+" in dataset "+dict->getName();
	}

	_ResourceNotExistsException::_ResourceNotExistsException(chstr object_name,chstr class_name,
														  Dataset* dict,chstr file,int line) :
								 _GenericException("","ResourceNotExistsException",file,line)
	{
		mErrText=class_name+" doesn't exist: "+object_name+" in dataset "+dict->getName();
	}

	_ObjectHasParentException::_ObjectHasParentException(chstr child,chstr parent,
														 chstr file,int line) :
							   _GenericException("","ObjectHasParentException",file,line)
	{
		mErrText="Cannot attach object '"+child+"' to object '"+parent+"', object already attached to another parent";
	}

	_ObjectNotChildException::_ObjectNotChildException(chstr child,chstr parent,
														 chstr file,int line) :
							   _GenericException("","ObjectNotChildException",file,line)
	{
		mErrText="Cannot detach object '"+child+"' from object '"+parent+"', object is not a child";
	}


	_XMLException::_XMLException(chstr err_msg,xml_node* node,chstr type,chstr file,int line):
				   _GenericException("",type,file,line)
	{
		mErrText=err_msg+", in file "+hstr((char*) node->doc->URL)+", line "+hstr(node->line);
	}
}
