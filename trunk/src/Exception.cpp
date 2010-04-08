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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "Exception.h"
#include "Dataset.h"
#include "Util.h"

namespace AprilUI
{
	_GenericException::_GenericException(chstr errorText,hstr type,hstr file,int line)
	{
		mErrText = errorText;
		int src=file.find("src");
		if (src >= 0) file=file.substr(src+4,1000);
		mLineNumber=line;
		mFile=file;
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
		writelog("----------------\nException Error!\n\n"+repr()+"\n----------------");
	}


	_ResourceExistsException::_ResourceExistsException(const hstr object_name,hstr class_name,
													   Dataset* dict,hstr file,int line) :
							  _GenericException("","ResourceExistsException",file,line)
	{
		mErrText=class_name+" already exists: "+object_name+" in dataset "+dict->getName();
	}

	_ResourceNotExistsException::_ResourceNotExistsException(const hstr object_name,hstr class_name,
														  Dataset* dict,hstr file,int line) :
								 _GenericException("","ResourceNotExistsException",file,line)
	{
		mErrText=class_name+" doesn't exist: "+object_name+" in dataset "+dict->getName();
	}

	_ObjectHasParentException::_ObjectHasParentException(const hstr child,hstr parent,
														 hstr file,int line) :
							   _GenericException("","ObjectHasParentException",file,line)
	{
		mErrText="Cannot attach object '"+child+"' to object '"+parent+"', object already attached to another parent";
	}

	_ObjectNotChildException::_ObjectNotChildException(const hstr child,hstr parent,
														 hstr file,int line) :
							   _GenericException("","ObjectNotChildException",file,line)
	{
		mErrText="Cannot detach object '"+child+"' from object '"+parent+"', object is not a child";
	}


	_XMLException::_XMLException(const hstr err_msg,_xmlNode* node,hstr type,hstr file,int line):
				   _GenericException("",type,file,line)
	{
		mErrText=err_msg+" in file "+hstr((char*) node->doc->URL)+", line "+hstr(node->line);
	}
}
