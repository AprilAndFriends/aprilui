/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_NULL_IMAGE_H
#define APRILUI_NULL_IMAGE_H

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport NullImage : public Image
	{
	public:
		NullImage();
		
		void draw(grect rect, April::Color color) { }
		void draw(grect rect, April::Color color, float angle) { }
		
	};

}
#endif
