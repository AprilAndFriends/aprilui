/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EXPORT_H
#define APRILUI_EXPORT_H

	#ifdef _STATICLIB
		#define AprilUIExport
		#define AprilUIFnExport
	#else
		#ifdef _WIN32
			#ifdef APRILUI_EXPORTS
				#define AprilUIExport __declspec(dllexport)
				#define AprilUIFnExport __declspec(dllexport)
			#else
				#define AprilUIExport __declspec(dllimport)
				#define AprilUIFnExport __declspec(dllimport)
			#endif
		#else
			#define AprilUIExport __attribute__ ((visibility("default")))
			#define AprilUIFnExport
		#endif
	#endif

#endif

