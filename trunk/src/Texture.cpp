/// @file
/// @version 3.3
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
	void (*Texture::loadListener)(Texture*) = NULL;
	void (*Texture::unloadListener)(Texture*) = NULL;

	Texture::Texture(chstr filename, april::Texture* texture)
	{
		this->originalFilename = filename;
		this->filename = texture->getFilename();
		this->texture = texture;
		this->filter = texture->getFilter();
		this->addressMode = texture->getAddressMode();
		float scale = aprilui::findTextureExtensionScale(this->filename);
		this->scale.set(scale, scale);
		this->unusedTime = 0.0f;
		this->dynamic = !texture->isLoaded();
	}

	Texture::~Texture()
	{
		if (this->texture != NULL)
		{
			delete this->texture;
		}
		foreach (Texture*, it, this->dynamicLinks)
		{
			(*it)->removeDynamicLink(this);
		}
	}

	int Texture::getWidth()
	{
		return (int)(this->texture->getWidth() * this->scale.x);
	}

	int Texture::getHeight()
	{
		return (int)(this->texture->getHeight() * this->scale.y);
	}

	void Texture::setFilter(april::Texture::Filter value)
	{
		this->filter = value;
		this->texture->setFilter(value);
	}

	void Texture::setAddressMode(april::Texture::AddressMode value)
	{
		this->addressMode = value;
		this->texture->setAddressMode(value);
	}

	bool Texture::isLoaded()
	{
		return (this->texture != NULL && this->texture->isLoaded());
	}
	
	bool Texture::isValid()
	{
		return (this->texture != NULL);
	}
	
	april::Texture* Texture::getRenderTexture()
	{
		this->unusedTime = 0.0f;
		foreach (Texture*, it, this->dynamicLinks)
		{
			(*it)->unusedTime = 0.0f;
		}
		return this->texture;
	}

	void Texture::update(float timeDelta)
	{
		if (this->dynamic && this->texture->isLoaded())
		{
			float maxTime = aprilui::getTextureIdleUnloadTime();
			this->unusedTime += timeDelta;
			if (maxTime > 0.0f && unusedTime >= maxTime)
			{
				if (unloadListener != NULL)
				{
					(*unloadListener)(this);
				}
				this->texture->unload();
				this->unusedTime = 0.0f; // safe guard if texture is reloaded externally at some point
			}
		}
	}

	void Texture::resetUnusedTime()
	{
		this->unusedTime = 0.0f;
	}
	
	void Texture::load(bool ignoreDynamicLinks)
	{
		this->unusedTime = 0.0f;
		if (!this->texture->isLoaded())
		{
			this->texture->load();
			if (loadListener != NULL)
			{
				(*loadListener)(this);
			}
		}
		if (!ignoreDynamicLinks)
		{
			foreach (Texture*, it, this->dynamicLinks)
			{
				(*it)->unusedTime = 0.0f;
				if (!(*it)->texture->isLoaded())
				{
					(*it)->texture->load();
					if (loadListener != NULL)
					{
						(*loadListener)(*it);
					}
				}
			}
		}
	}

	void Texture::unload()
	{
		if (unloadListener != NULL)
		{
			(*unloadListener)(this);
		}
		this->texture->unload();
	}

	void Texture::reload(chstr filename)
	{
		if (this->filename != filename)
		{
			if (this->texture != NULL)
			{
				delete this->texture;
			}
			this->unusedTime = 0.0f;
			this->texture = april::rendersys->createTextureFromResource(filename, april::Texture::TYPE_IMMUTABLE, !this->dynamic && !aprilui::getDefaultDynamicLoading());
			if (this->texture == NULL)
			{
				throw file_not_found(filename);
			}
			this->filename = this->texture->getFilename();
			this->texture->setFilter(this->filter);
			this->texture->setAddressMode(this->addressMode);
			float scale = aprilui::findTextureExtensionScale(this->filename);
			this->scale.set(scale, scale);
		}
	}

	void Texture::addDynamicLink(Texture* link)
	{
		if (!this->dynamicLinks.contains(link))
		{
			this->dynamicLinks += link;
			link->addDynamicLink(this);
		}
	}
	
	void Texture::removeDynamicLink(Texture* link)
	{
		if (this->dynamicLinks.contains(link))
		{
			this->dynamicLinks -= link;
		}
	}
	
	void Texture::setLoadListener(void (*callback)(Texture*))
	{
		Texture::loadListener = callback;
	}
	
	void Texture::setUnloadListener(void (*callback)(Texture*))
	{
		Texture::unloadListener = callback;
	}
	
}
