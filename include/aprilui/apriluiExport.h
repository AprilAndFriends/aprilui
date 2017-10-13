/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_EXPORT_H
#define APRILUI_EXPORT_H

	#ifdef _LIB
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
			#define apriluiFnExport __attribute__ ((visibility("default")))
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

