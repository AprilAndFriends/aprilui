/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2011 Boris Mikic                                                       *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef NO_PARTICLE
#ifndef APRILUI_PARTICLE_H
#define APRILUI_PARTICLE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilparticle
{
	class System;
}

namespace aprilui
{
	class apriluiExport Particle : public Object
	{
	public:
		Particle(chstr name, grect rect);
		~Particle();

		hstr getFilename() { return mFilename; }
		void setFilename(chstr value) { mFilename = value; }
		
		void notifyEvent(chstr name, void* params);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
		void update(float k);
		void OnDraw();
		
	protected:
		hstr mFilename;
		aprilparticle::System* mSystem;

	};
}

#endif
#endif