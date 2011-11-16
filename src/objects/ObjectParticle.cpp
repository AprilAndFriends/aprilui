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
#include <aprilparticle/Emitter.h>
#include <aprilparticle/System.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
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
			mSystem->setEnabled(this->_isDerivedEnabled());
			mSystem->update(k);
		}
		Object::update(k);
	}
	
	void Particle::OnDraw()
	{
		if (mSystem != NULL)
		{
			gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
			mSystem->setVisible(this->isVisible());
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
				this->_loadParticleSystem();
			}
			else if (mSystem != NULL)
			{
				delete mSystem;
				mSystem = NULL;
			}
		}
		Object::notifyEvent(name, params);
	}

	void Particle::_loadParticleSystem()
	{
		if (mSystem != NULL)
		{
			delete mSystem;
		}
		hstr filepath = normalize_path(mDataset->_getFilePath() + "/" + aprilui::getDefaultParticlesPath() + "/" + mFilename);
		mSystem = new aprilparticle::System(filepath);
		mSystem->load();
		if (mRect.w > 0.0f || mRect.h > 0.0f)
		{
			harray<aprilparticle::Emitter*> emitters = mSystem->getEmitters();
			if (emitters.size() > 0)
			{
				gvec2 rescale(1.0f, 1.0f);
				gvec3 dimensions = emitters.first()->getDimensions();
				if (mRect.w > 0.0f)
				{
					rescale.x = (dimensions.x > 0.0f ? mRect.w / dimensions.x : mRect.w);
				}
				if (mRect.h > 0.0f)
				{
					rescale.y = (dimensions.y > 0.0f ? mRect.h / dimensions.y : mRect.h);
				}
				foreach (aprilparticle::Emitter*, it, emitters)
				{
					dimensions = (*it)->getDimensions();
					dimensions.x = (dimensions.x > 0.0f ? dimensions.x * rescale.x : rescale.x);
					dimensions.y = (dimensions.y > 0.0f ? dimensions.y * rescale.y : rescale.y);
					(*it)->setDimensions(dimensions);
				}
			}
		}
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
