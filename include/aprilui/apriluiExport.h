/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines macros for DLL exports/imports.

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
	#ifndef DEPRECATED_ATTRIBUTE
		#ifdef _MSC_VER
			#define DEPRECATED_ATTRIBUTE __declspec(deprecated("function is deprecated"))
		#else
			#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
		#endif
	#endif

#endif

