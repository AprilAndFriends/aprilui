/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EXPORT_H
#define APRILUI_EXPORT_H

	#ifdef _STATICLIB
		#define apriluiExport
		#define apriluiFnExport
	#else
		#ifdef _WIN32
			#ifdef APRILUI_EXPORTS
				#define apriluiExport __declspec(dllexport)
				#define apriluiFnExport __declspec(dllexport)
			#else
				#define apriluiExport __declspec(dllimport)
				#define apriluiFnExport __declspec(dllimport)
			#endif
		#else
			#define apriluiExport __attribute__ ((visibility("default")))
			#define apriluiFnExport
		#endif
	#endif
	#ifndef DEPRECATED_ATTRIBUTE
		#ifdef _MSC_VER
			#define DEPRECATED_ATTRIBUTE
		#else
			#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
		#endif
	#endif


	// GLOBAL HACKS:
	
	// hex value fixer:
	//  AARRGGBB => RRGGBBAA
	#define HEXCOLOR_FIX_FOR_NEW_APRIL(value) \
		((value.size() == 8) ? (value.substr(2,6) + value.substr(0,2)) : hstr(value))

	// new april does not accept float color components
	#define COLOR_COMP_FOR_NEW_APRIL(value) \
		((unsigned char)(value * 255))

#endif
