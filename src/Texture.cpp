/// @file
/// @version 1.34
///
/// @section LICENSE
///
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>

#include "aprilui.h"
#include "Texture.h"

namespace aprilui
{
	// TODO - refactor april to support a "TextureInterface" class or something like that
	Texture::Texture(chstr filename, april::Texture* texture)
	{
		mName = filename;
		mOriginalFilename = filename;
		mFilename = filename;
		mTexture = texture;
		mFilter = texture->getFilter();
		mAddressMode = texture->getAddressMode();
		mScale.set(1.0f, 1.0f);
		mUnusedTime = 0.0f;
		mDynamic = texture->isUnloaded();
	}

	Texture::Texture(chstr name, chstr filename, april::Texture* texture, bool managed) // aprilui trunk compatibility
	{
		mName = name;
		mOriginalFilename = filename;
		mFilename = filename;
		mTexture = texture;
		mFilter = texture->getFilter();
		mAddressMode = texture->getAddressMode();
		mScale.set(1.0f, 1.0f);
		mUnusedTime = 0.0f;
		mDynamic = managed;
	}

	Texture::~Texture()
	{
	if (mTexture != NULL)
		{
			april::rendersys->destroyTexture(mTexture);
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

	april::Texture* Texture::getTexture() // aprilui trunk compatibility
	{
		return getRenderTexture();
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
		if (mDynamic && !mTexture->isUnloaded() && mTexture != NULL)
		{
			// TODO - change to aprilui variable
			float maxTime = aprilui::getTextureIdleUnloadTime();
			mUnusedTime += k;
			if (maxTime > 0.0f && mUnusedTime >= maxTime)
			{
				mTexture->unload();
				mUnusedTime = 0.0f;
			}
		}
	}

	void Texture::resetUnusedTime()
	{
		mUnusedTime = 0.0f;
	}

	void Texture::load()
	{
		mUnusedTime = 0.0f;
		mTexture->loadAsync();
		foreach (Texture*, it, mDynamicLinks)
		{
			(*it)->resetUnusedTime();
			(*it)->getRenderTexture()->loadAsync();
		}
	}

	void Texture::unload()
	{
		if (mTexture != NULL)
		{
			mTexture->unload();
		}
	}

	void Texture::reload(chstr filename)
	{
		if (mFilename != filename)
		{
			if (mTexture != NULL)
			{
				april::rendersys->destroyTexture(mTexture);
			}
			mUnusedTime = 0.0f;
			mFilename = filename;
			mTexture = april::rendersys->createTextureFromResource(mFilename, april::Texture::Type::Immutable, april::Texture::LoadMode::OnDemand);
			if (mTexture == NULL)
			{
				throw FileNotOpenException(mFilename);
			}
			mTexture->setFilter(mFilter);
			mTexture->setAddressMode(mAddressMode);
		}
	}

	void Texture::addDynamicLink(Texture* link)
	{
		if (!mDynamicLinks.has(link))
		{
			mDynamicLinks += link;
			link->addDynamicLink(this);
		}
	}
	
	void Texture::removeDynamicLink(Texture* link)
	{
		if (mDynamicLinks.has(link))
		{
			mDynamicLinks -= link;
		}
	}
	
}
