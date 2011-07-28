/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2011 Boris Mikic                                                       *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifdef _HAVE_PARTICLE
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectParticle.h"

namespace aprilui
{
	Particle::Particle(chstr name, grect rect) :
		Object("Particle", name, rect)
	{
	}
	
	void Particle::OnDraw()
	{
		Object::OnDraw();
	}

	bool Particle::setProperty(chstr name, chstr value)
	{
		return Object::setProperty(name, value);
	}

}
#endif
