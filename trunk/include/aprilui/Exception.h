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
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

struct _xmlNode;
namespace AprilUI
{
	
	class Dataset;
	class _GenericException
	{
	public:
		std::string mErrText,mFile,mType;
		int mLineNumber;

		_GenericException(const std::string& errorText,std::string type="",std::string file="",int line=0);
		virtual ~_GenericException() {}
		
		virtual std::string repr();
		
		void writeOutput();
		
		virtual const std::string& getErrorText() { return mErrText; }
		
		const std::string getType(){ return mType; }
	};

	#define GenericException(msg) _GenericException(msg,"GenericException",__FILE__,__LINE__)


	#define exception_cls(name) class name : public _GenericException \
	{ \
	public: \
		name(const std::string& errorText,std::string type="",std::string file="",int line=0) : \
		  _GenericException(errorText,type,file,line){} \
	}

	exception_cls(_KeyException);
	#define KeyException(msg) _KeyException(msg,"KeyException",__FILE__,__LINE__)

	exception_cls(_ScriptException);
	#define ScriptException(msg) _ScriptException(msg,"ScriptException",__FILE__,__LINE__)

	exception_cls(_ObjectExistsException);
	#define ObjectExistsException(msg) _ObjectExistsException("Object already exists: "+msg,"ObjectExistsException",__FILE__,__LINE__)

	exception_cls(_FileNotFoundException);
	#define FileNotFoundException(msg) _FileNotFoundException("File not found: "+msg,"FileNotFoundException",__FILE__,__LINE__)

	exception_cls(_ObjectNotExistsException);
	#define ObjectNotExistsException(msg) _ObjectNotExistsException("Object doesn't exist: "+msg,"ObjectNotExistsException",__FILE__,__LINE__)

	exception_cls(_FontExistsException);
	#define FontExistsException(msg) _FontExistsException("Unable to add new font, it already exists: "+msg,"FontExistsException",__FILE__,__LINE__)

	exception_cls(_FontNotExistsException);
	#define FontNotExistsException(msg) _FontNotExistsException("Font doesn't exist: "+msg,"FontNotExistsException",__FILE__,__LINE__)

	exception_cls(_UnknownVariableException);
	#define UnknownVariableException(msg) _UnknownVariableException(msg+" does not exist","UnknownVariableException ",__FILE__,__LINE__)
	//---------------------------------------------------------------------------------------------------------

	class _ResourceExistsException : public _GenericException
	{
	public:
		_ResourceExistsException(const std::string object_name,std::string class_name,Dataset* dict,std::string file="",int line=0);
	};
	class _ResourceNotExistsException : public _GenericException
	{
	public:
		_ResourceNotExistsException(const std::string object_name,std::string class_name,Dataset* dict,std::string file="",int line=0);
	};
	#define ResourceExistsException(name,cls,data) _ResourceExistsException(name,cls,data,__FILE__,__LINE__)
	#define ResourceNotExistsException(name,cls,data) _ResourceNotExistsException(name,cls,data,__FILE__,__LINE__)

	//---------------------------------------------------------------------------------------------------------

	class _ObjectHasParentException : public _GenericException
	{
	public:
		_ObjectHasParentException(const std::string child,std::string parent,std::string file="",int line=0);
	};

	#define ObjectHasParentException(child,parent) _ObjectHasParentException(child,parent,__FILE__,__LINE__)

	//---------------------------------------------------------------------------------------------------------

	class _ObjectNotChildException : public _GenericException
	{
	public:
		_ObjectNotChildException(const std::string child,std::string parent,std::string file="",int line=0);
	};

	#define ObjectNotChildException(child,parent) _ObjectNotChildException(child,parent,__FILE__,__LINE__)

	//---------------------------------------------------------------------------------------------------------

	class _XMLException : public _GenericException
	{
	public:
		_XMLException(const std::string err_msg,_xmlNode* node,std::string type,std::string file,int line);
	};

	#define XMLException(msg,node) _XMLException(msg,node,type,__FILE__,__LINE__)
	#define XMLPropertyNotExistsException(msg,node) _XMLException("XML property doesn't exist: "+msg,node,"XMLException",__FILE__,__LINE__)
	#define XMLUnknownClassException(msg,node) _XMLException("Unknown class detected in XML file: "+msg,node,"XMLUnknownClassException",__FILE__,__LINE__)

}

#endif
