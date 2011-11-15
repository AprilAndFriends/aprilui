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
#include <april/RenderSystem.h>
#include <aprilparticle/System.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector3.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectParticle.h"

namespace aprilui
{
	Particle::Particle(chstr name, grect rect) :
		Object("Particle", name, rect)
	{
		mSystem = NULL;
	}
	
	Particle::~Particle()
	{
		if (mSystem != NULL)
		{
			delete mSystem;
		}
	}

	void Particle::update(float k)
	{
		if (mSystem != NULL)
		{
			mSystem->update(k);
		}
		Object::update(k);
	}
	
	void Particle::OnDraw()
	{
		if (mSystem != NULL)
		{
			gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
			mSystem->draw2D();
			april::rendersys->setModelviewMatrix(originalMatrix);
			april::rendersys->setBlendMode(april::DEFAULT);
		}
		Object::OnDraw();
	}

	void Particle::notifyEvent(chstr name, void* params)
	{	
		if (name == "FilenameLoaded")
		{
			if (mFilename != "")
			{
				if (mSystem != NULL)
				{
					delete mSystem;
				}
				hstr filepath = normalize_path(mDataset->_getFilePath() + "/" + aprilui::getDefaultParticlesPath() + "/" + mFilename);
				mSystem = new aprilparticle::System(filepath);
				mSystem->load();
			}
			else if (mSystem != NULL)
			{
				delete mSystem;
				mSystem = NULL;
			}
		}
		Object::notifyEvent(name, params);
	}
	
	hstr Particle::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "filename")	return getFilename();
		return Object::getProperty(name, property_exists);
	}

	bool Particle::setProperty(chstr name, chstr value)
	{
		if (name == "filename")
		{
			setFilename(value);
			notifyEvent("FilenameLoaded", NULL);
		}
		else return Object::setProperty(name, value);
		return true;
	}

}
#endif
