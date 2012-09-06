/// @file
/// @author  Boris Mikic
/// @version 1.93
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RamTexture.h>
#include <april/RenderSystem.h>

#include "aprilui.h"
#include "Texture.h"

namespace aprilui
{
	Texture::Texture(chstr filename, april::Texture* texture)
	{
		this->mOriginalFilename = filename;
		this->mFilename = texture->getFilename();
		this->mTexture = texture;
		this->mFilter = texture->getFilter();
		this->mAddressMode = texture->getAddressMode();
		float scale = aprilui::findTextureExtensionScale(this->mFilename);
		this->mScale.set(scale, scale);
		this->mUnusedTime = 0.0f;
		this->mDynamic = !texture->isLoaded();
	}

	Texture::~Texture()
	{
		if (this->mTexture != NULL)
		{
			delete this->mTexture;
		}
		foreach (Texture*, it, this->mDynamicLinks)
		{
			(*it)->removeDynamicLink(this);
		}
	}

	april::Texture* Texture::getRenderTexture()
	{
		this->mUnusedTime = 0.0f;
		foreach (Texture*, it, this->mDynamicLinks)
		{
			(*it)->mUnusedTime = 0.0f;
		}
		return this->mTexture;
	}

	int Texture::getWidth()
	{
		return (int)(this->mTexture->getWidth() * this->mScale.x);
	}

	int Texture::getHeight()
	{
		return (int)(this->mTexture->getHeight() * this->mScale.y);
	}

	void Texture::setFilter(april::Texture::Filter value)
	{
		this->mFilter = value;
		this->mTexture->setFilter(value);
	}

	void Texture::setAddressMode(april::Texture::AddressMode value)
	{
		this->mAddressMode = value;
		this->mTexture->setAddressMode(value);
	}

	bool Texture::isValid()
	{
		return (this->mTexture != NULL);
	}
	
	void Texture::update(float k)
	{
		if (this->mDynamic && this->mTexture->isLoaded())
		{
			// TODO - change to aprilui variable
			float maxTime = aprilui::getTextureIdleUnloadTime();
			mUnusedTime += k;
			if (maxTime > 0.0f && mUnusedTime > maxTime)
			{
				this->mTexture->unload();
			}
		}
	}

	void Texture::resetUnusedTime()
	{
		this->mUnusedTime = 0.0f;
	}

	void Texture::load()
	{
		this->mUnusedTime = 0.0f;
		this->mTexture->load();
		foreach (Texture*, it, this->mDynamicLinks)
		{
			(*it)->resetUnusedTime();
			(*it)->getRenderTexture()->load();
		}
	}

	void Texture::unload()
	{
		this->mTexture->unload();
	}

	void Texture::reload(chstr filename)
	{
		if (this->mFilename != filename)
		{
			// TODO - remove when RamTexture was removed
			bool isRamTexture = false;
			if (this->mTexture != NULL)
			{
				isRamTexture = (dynamic_cast<april::RamTexture*>(this->mTexture) != NULL);
				delete this->mTexture;
			}
			this->mUnusedTime = 0.0f;
			if (!isRamTexture)
			{
				this->mTexture = april::rendersys->loadTexture(filename, this->mDynamic || aprilui::getForcedDynamicLoading());
			}
			else
			{
				this->mTexture = april::rendersys->loadRamTexture(filename, this->mDynamic || aprilui::getForcedDynamicLoading());
			}
			if (this->mTexture == NULL)
			{
				throw file_not_found(filename);
			}
			this->mFilename = this->mTexture->getFilename();
			this->mTexture->setFilter(this->mFilter);
			this->mTexture->setAddressMode(this->mAddressMode);
			float scale = aprilui::findTextureExtensionScale(this->mFilename);
			this->mScale.set(scale, scale);
		}
	}

	void Texture::addDynamicLink(Texture* link)
	{
		if (!this->mDynamicLinks.contains(link))
		{
			this->mDynamicLinks += link;
			link->addDynamicLink(this);
		}
	}
	
	void Texture::removeDynamicLink(Texture* link)
	{
		if (this->mDynamicLinks.contains(link))
		{
			this->mDynamicLinks -= link;
		}
	}
	
}
