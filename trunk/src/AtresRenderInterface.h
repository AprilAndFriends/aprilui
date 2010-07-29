/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRIL_ATRES_RENDER_IFACE_H
#define APRIL_ATRES_RENDER_IFACE_H

#include <atres/RenderingInterface.h>

namespace AprilUI
{
class AtresAprilInterface : public Atres::RenderInterface
{
public:
	AtresAprilInterface();
	~AtresAprilInterface();
	
	unsigned int loadResource(const char* filename);
	void render(Atres::CharacterRenderOp* rops,int n);
};
}

#endif
