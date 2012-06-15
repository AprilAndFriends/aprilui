/// @file
/// @author  Boris Mikic
/// @version 1.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>

#include "Texture.h"

namespace aprilui
{
	// TODO - refactor april to support a "TextureInterface" class or something like that
	Texture::Texture(chstr filename, april::Texture* texture) : EventReceiver()
	{
		mFilename = filename;
		mTexture = texture;
		mFilter = texture->getFilter();
		mAddressMode = texture->getAddressMode();
		mScale.set(1.0f, 1.0f);
		mUnusedTime = 0.0f;
		mDynamic = !texture->isLoaded();
	}

	Texture::~Texture()
	{
		if (mTexture != NULL)
		{
			delete mTexture;
		}
		foreach (Texture*, it, mDynamicLinks)
		{
			(*it)->removeDynamicLink(this);
		}
	}

	april::Texture* Texture::getRenderTexture()
	{
		mUnusedTime = 0.0f;
		foreach (Texture*, it, mDynamicLinks)
		{
			(*it)->mUnusedTime = 0.0f;
		}
		return mTexture;
	}

	int Texture::getWidth()
	{
		return (int)(mTexture->getWidth() * mScale.x);
	}

	int Texture::getHeight()
	{
		return (int)(mTexture->getHeight() * mScale.y);
	}

	void Texture::setFilter(april::Texture::Filter value)
	{
		mFilter = value;
		mTexture->setFilter(value);
	}

	void Texture::setAddressMode(april::Texture::AddressMode value)
	{
		mAddressMode = value;
		mTexture->setAddressMode(value);
	}

	bool Texture::isValid()
	{
		return (mTexture != NULL);
	}
	
	void Texture::update(float k)
	{
		if (mDynamic && mTexture->isLoaded())
		{
			// TODO - change to aprilui variable
			float maxTime = april::rendersys->getTextureIdleUnloadTime();
			if (maxTime > 0.0f)
			{
				mUnusedTime += k;
				if (mUnusedTime > maxTime)
				{
					mTexture->unload();
				}
			}
		}
	}

	void Texture::load()
	{
		mUnusedTime = 0.0f;
		mTexture->load();
		foreach (Texture*, it, mDynamicLinks)
		{
			(*it)->load();
		}
	}

	void Texture::unload()
	{
		mTexture->unload();
	}

	void Texture::reload(chstr filename)
	{
		if (mTexture != NULL)
		{
			delete mTexture;
		}
		mUnusedTime = 0.0f;
		mFilename = filename;
		mTexture = april::rendersys->loadTexture(filename, mDynamic);
		mTexture->setFilter(mFilter);
		mTexture->setAddressMode(mAddressMode);
	}

	void Texture::addDynamicLink(Texture* link)
	{
		if (!mDynamicLinks.contains(link))
		{
			mDynamicLinks += link;
			link->addDynamicLink(this);
		}
	}
	
	void Texture::removeDynamicLink(Texture* link)
	{
		if (mDynamicLinks.contains(link))
		{
			mDynamicLinks -= link;
		}
	}
	
}
