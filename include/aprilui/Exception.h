/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "hltypes/hstring.h"
#include "AprilUIExport.h"

struct xml_node;
namespace AprilUI
{
	
	class Dataset;
	class AprilUIExport _GenericException
	{
	public:
		hstr mErrText,mFile,mType;
		int mLineNumber;

		_GenericException(chstr errorText,chstr type="",chstr file="",int line=0);
		virtual ~_GenericException();
		
		virtual hstr repr();
		
		void writeOutput();
		
		virtual hstr getErrorText() { return mErrText; }
		
		hstr getType(){ return mType; }
	};

	#define GenericException(msg) _GenericException(msg,"GenericException",__FILE__,__LINE__)


	#define exception_cls(name) class AprilUIExport name : public _GenericException \
	{ \
	public: \
		name(chstr errorText,chstr type="",chstr file="",int line=0) : \
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

	class AprilUIExport _ResourceExistsException : public _GenericException
	{
	public:
		_ResourceExistsException(chstr object_name,chstr class_name,Dataset* dict,chstr file="",int line=0);
	};
	class AprilUIExport _ResourceNotExistsException : public _GenericException
	{
	public:
		_ResourceNotExistsException(chstr object_name,chstr class_name,Dataset* dict,chstr file="",int line=0);
	};
	#define ResourceExistsException(name,cls,data) _ResourceExistsException(name,cls,data,__FILE__,__LINE__)
	#define ResourceNotExistsException(name,cls,data) _ResourceNotExistsException(name,cls,data,__FILE__,__LINE__)

	//---------------------------------------------------------------------------------------------------------

	class AprilUIExport _ObjectHasParentException : public _GenericException
	{
	public:
		_ObjectHasParentException(chstr child,chstr parent,chstr file="",int line=0);
	};

	#define ObjectHasParentException(child,parent) _ObjectHasParentException(child,parent,__FILE__,__LINE__)

	//---------------------------------------------------------------------------------------------------------

	class AprilUIExport _ObjectNotChildException : public _GenericException
	{
	public:
		_ObjectNotChildException(chstr child,chstr parent,chstr file="",int line=0);
	};

	#define ObjectNotChildException(child,parent) _ObjectNotChildException(child,parent,__FILE__,__LINE__)

	//---------------------------------------------------------------------------------------------------------

	class AprilUIExport _XMLException : public _GenericException
	{
	public:
		_XMLException(chstr err_msg,xml_node* node,chstr type,chstr file,int line);
	};

	#define XMLException(msg,node) _XMLException(msg,node,type,__FILE__,__LINE__)
	#define XMLPropertyNotExistsException(msg,node) _XMLException(hstr("XML property doesn't exist: ")+msg,node,"XMLException",__FILE__,__LINE__)
	#define XMLUnknownClassException(msg,node) _XMLException(hstr("Unknown class detected in XML file: ")+msg,node,"XMLUnknownClassException",__FILE__,__LINE__)

}

#endif
