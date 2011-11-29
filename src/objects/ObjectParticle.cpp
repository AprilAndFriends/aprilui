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
		Object(name, rect)
	{
		mGlobalSpace = false;
		mInitialPosition = getPosition();
		mSystem = NULL;
	}
	
	Particle::~Particle()
	{
		stopSystem();
	}

	Object* Particle::createInstance(chstr name, grect rect)
	{
		return new Particle(name, rect);
	}

	bool Particle::isRunning()
	{
		return (mSystem != NULL && mSystem->isRunning());
	}

	bool Particle::isExpired()
	{
		return (mSystem != NULL && mSystem->isExpired());
	}

	void Particle::load(chstr filename)
	{
		mFilename = filename;
		stopSystem();
		_loadParticleSystem();
	}

	void Particle::update(float k)
	{
		if (mSystem != NULL)
		{
			mSystem->setEnabled(_isDerivedEnabled());
			mSystem->update(k);
		}
		Object::update(k);
	}
	
	void Particle::OnDraw()
	{
		if (mSystem != NULL)
		{
			gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
			mSystem->setVisible(isVisible());
			if (!mGlobalSpace)
			{
				mSystem->draw(gvec2(), april::Color(mColor, getDerivedAlpha()));
			}
			else
			{
				gvec2 position = mInitialPosition - getPosition();
				mSystem->setPosition(mSystemPosition.x - position.x, mSystemPosition.y - position.y, mSystemPosition.z);
				mSystem->draw(position, april::Color(mColor, getDerivedAlpha()));
			}
			april::rendersys->setModelviewMatrix(originalMatrix);
			april::rendersys->setBlendMode(april::DEFAULT);
		}
		Object::OnDraw();
	}

	void Particle::notifyEvent(chstr name, void* params)
	{	
		if (name == "SettingsChanged")
		{
			if (mFilename != "" && aprilui::isParticlesEnabled())
			{
				_loadParticleSystem();
			}
			else
			{
				stopSystem();
			}
		}
		Object::notifyEvent(name, params);
	}

	void Particle::_loadParticleSystem()
	{
		if (mSystem != NULL)
		{
			return;
		}
		hstr filepath = normalize_path(mDataset->_getFilePath() + "/" + aprilui::getDefaultParticlesPath() + "/" + mFilename);
		mSystem = new aprilparticle::System(filepath);
		mSystem->load();
		mSystemPosition = mSystem->getPosition();
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

	void Particle::finishSystem()
	{
		if (mSystem != NULL)
		{
			mSystem->finish();
		}
	}

	void Particle::stopSystem()
	{
		if (mSystem != NULL)
		{
			delete mSystem;
			mSystem = NULL;
		}
	}
	
	void Particle::resetSystem()
	{
		if (mSystem != NULL)
		{
			mSystem->reset();
		}
	}
	
	hstr Particle::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "filename")		return getFilename();
		if (name == "global_space")	return isGlobalSpace();
		return Object::getProperty(name, property_exists);
	}

	bool Particle::setProperty(chstr name, chstr value)
	{
		if		(name == "filename")
		{
			setFilename(value);
			notifyEvent("SettingsChanged", NULL);
		}
		else if	(name == "global_space")	setGlobalSpace(value);
		else return Object::setProperty(name, value);
		return true;
	}

}
#endif
