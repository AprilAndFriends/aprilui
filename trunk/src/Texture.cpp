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

	Texture::Texture(chstr filename, april::Texture* texture, bool managed)
	{
		this->originalFilename = filename;
		this->filename = texture->getFilename();
		this->texture = texture;
		this->loadMode = texture->getLoadMode();
		this->managed = managed;
		this->filter = texture->getFilter();
		this->addressMode = texture->getAddressMode();
		float scale = aprilui::findTextureExtensionScale(this->filename);
		this->scale.set(scale, scale);
		this->unusedTime = 0.0f;
	}

	Texture::~Texture()
	{
		if (this->texture != NULL)
		{
			delete this->texture;
		}
		foreach (Texture*, it, this->links)
		{
			(*it)->removeLink(this);
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

	bool Texture::isLoadedAsync()
	{
		return (this->texture != NULL && this->texture->isLoadedAsync());
	}

	bool Texture::isAsyncLoadQueued()
	{
		return (this->texture != NULL && this->texture->isAsyncLoadQueued());
	}

	bool Texture::isValid()
	{
		return (this->texture != NULL);
	}
	
	april::Texture* Texture::getRenderTexture()
	{
		this->unusedTime = 0.0f;
		foreach (Texture*, it, this->links)
		{
			(*it)->unusedTime = 0.0f;
		}
		return this->texture;
	}

	void Texture::update(float timeDelta)
	{
		if (this->managed && (this->texture->isLoaded() || this->texture->isLoadedAsync()))
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
		if (!this->isLoaded())
		{
			this->texture->load();
			if (loadListener != NULL)
			{
				(*loadListener)(this);
			}
		}
		if (!ignoreDynamicLinks)
		{
			Texture* tex;
			foreach (Texture*, it, this->links)
			{
				tex = *it;
				tex->unusedTime = 0.0f;
				if (!tex->isLoaded() && !tex->isLoadedAsync() && !tex->isAsyncLoadQueued())
				{
					tex->texture->loadAsync();
					if (loadListener != NULL)
					{
						(*loadListener)(tex);
					}
				}
			}
		}
	}

	void Texture::loadAsync(bool ignoreDynamicLinks)
	{
		this->unusedTime = 0.0f;
		if (this->texture != NULL && !this->texture->isLoaded() && !this->texture->isLoadedAsync() && !this->texture->isAsyncLoadQueued())
		{
			this->texture->loadAsync();
		}
		if (!ignoreDynamicLinks)
		{
			foreach (Texture*, it, this->links)
			{
				(*it)->unusedTime = 0.0f;
				if ((*it)->texture != NULL && !(*it)->texture->isLoaded() && !(*it)->texture->isLoadedAsync() && !(*it)->texture->isAsyncLoadQueued())
				{
					(*it)->texture->loadAsync();
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
			this->texture = april::rendersys->createTextureFromResource(filename, april::Texture::TYPE_IMMUTABLE, this->loadMode);
			if (this->texture == NULL)
			{
				throw file_could_not_open(filename);
			}
			this->filename = this->texture->getFilename();
			this->texture->setFilter(this->filter);
			this->texture->setAddressMode(this->addressMode);
			float scale = aprilui::findTextureExtensionScale(this->filename);
			this->scale.set(scale, scale);
		}
	}

	void Texture::addLink(Texture* link)
	{
		if (!this->links.contains(link))
		{
			this->links += link;
			link->addLink(this);
		}
	}
	
	void Texture::removeLink(Texture* link)
	{
		if (this->links.contains(link))
		{
			this->links -= link;
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
