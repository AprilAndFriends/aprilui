/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Event.h"
#include "Texture.h"

namespace aprilui
{
	void _registerTexture(Texture* texture);
	void _unregisterTexture(Texture* texture);

	Texture::Texture(chstr name, chstr filename, april::Texture* texture, bool managed)
	{
		this->name = name;
		this->originalFilename = filename;
		this->filename = texture->getFilename();
		if (this->filename == "")
		{
			this->filename = this->originalFilename;
		}
		this->texture = texture;
		this->loadMode = texture->getLoadMode();
		this->managed = managed;
		this->filter = texture->getFilter();
		this->addressMode = texture->getAddressMode();
		float factor = aprilui::findTextureExtensionScale(this->filename);
		this->scale.set(factor, factor);
		this->dataset = NULL;
		this->unusedTime = 0.0f;
		_registerTexture(this);
	}

	Texture::~Texture()
	{
		_unregisterTexture(this);
		if (this->texture != NULL)
		{
			april::rendersys->destroyTexture(this->texture);
		}
		foreach (Texture*, it, this->links)
		{
			(*it)->removeLink(this);
		}
	}

	int Texture::getWidth() const
	{
		return (int)(this->texture->getWidth() * this->scale.x);
	}

	int Texture::getHeight() const
	{
		return (int)(this->texture->getHeight() * this->scale.y);
	}

	bool Texture::isUploaded() const
	{
		return (this->texture != NULL && this->texture->isUploaded());
	}

	bool Texture::isReadyForUpload() const
	{
		return (this->texture != NULL && this->texture->isReadyForUpload());
	}

	bool Texture::isAsyncLoadQueued() const
	{
		return (this->texture != NULL && this->texture->isAsyncLoadQueued());
	}

	bool Texture::isUnloaded() const
	{
		return (this->texture != NULL && this->texture->isUnloaded());
	}

	bool Texture::isValid() const
	{
		return (this->texture != NULL);
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

	void Texture::update(float timeDelta)
	{
		if (this->managed && this->texture != NULL && !this->texture->isUnloaded())
		{
			float maxTime = aprilui::getTextureIdleUnloadTime();
			this->unusedTime += timeDelta;
			if (maxTime > 0.0f && unusedTime >= maxTime)
			{
				this->texture->unload();
				if (this->dataset != NULL)
				{
					this->dataset->triggerEvent(Event::TextureUnloaded, this);
				}
				this->unusedTime = 0.0f; // safe guard if texture is reloaded externally at some point
			}
		}
	}

	void Texture::resetUnusedTime()
	{
		this->unusedTime = 0.0f;
		foreach (Texture*, it, this->links)
		{
			(*it)->unusedTime = 0.0f;
		}
	}
	
	bool Texture::loadMetaData(bool ignoreDynamicLinks)
	{
		bool result = this->texture->loadMetaData();
		if (!ignoreDynamicLinks)
		{
			foreach (Texture*, it, this->links)
			{
				(*it)->texture->loadMetaData();
			}
		}
		return result;
	}

	bool Texture::load(bool ignoreDynamicLinks)
	{
		bool result = true;
		this->unusedTime = 0.0f;
		if (!this->isUploaded())
		{
			bool unloaded = this->texture->isUnloaded();
			result = this->texture->loadAsync();
			if (unloaded && result)
			{
				if (this->dataset != NULL)
				{
					this->dataset->triggerEvent(Event::TextureLoaded, this);
				}
			}
		}
		if (!ignoreDynamicLinks)
		{
			foreach (Texture*, it, this->links)
			{
				(*it)->unusedTime = 0.0f;
				if ((*it)->isUnloaded())
				{
					// using loadAsync() here on purpose as the linked textures are probably not required right now so they can be loaded asynchronously
					if ((*it)->texture->loadAsync())
					{
						if (this->dataset != NULL)
						{
							this->dataset->triggerEvent(Event::TextureLoaded, (*it));
						}
					}
				}
			}
		}
		return result;
	}

	bool Texture::loadAsync(bool ignoreDynamicLinks)
	{
		bool result = false;
		this->unusedTime = 0.0f;
		if (this->isUnloaded())
		{
			result = this->texture->loadAsync();
			if (result)
			{
				if (this->dataset != NULL)
				{
					this->dataset->triggerEvent(Event::TextureLoaded, this);
				}
			}
		}
		if (!ignoreDynamicLinks)
		{
			foreach (Texture*, it, this->links)
			{
				(*it)->unusedTime = 0.0f;
				if ((*it)->isUnloaded())
				{
					if ((*it)->texture->loadAsync())
					{
						if (this->dataset != NULL)
						{
							this->dataset->triggerEvent(Event::TextureLoaded, (*it));
						}
					}
				}
			}
		}
		return result;
	}

	void Texture::unload()
	{
		if (this->texture != NULL)
		{
			bool previouslyUnloaded = this->texture->isUnloaded();
			this->texture->unload();
			if (!previouslyUnloaded)
			{
				if (this->dataset != NULL)
				{
					this->dataset->triggerEvent(Event::TextureUnloaded, this);
				}
			}
		}
	}

	void Texture::reload(chstr filename)
	{
		if (this->filename != filename)
		{
			if (this->texture != NULL)
			{
				april::rendersys->destroyTexture(this->texture);
			}
			this->unusedTime = 0.0f;
			this->texture = april::rendersys->createTextureFromResource(filename, april::Texture::Type::Immutable, this->loadMode);
			if (this->texture == NULL)
			{
				__THROW_EXCEPTION(ResourceFileCouldNotOpenException(filename), aprilui::textureFilesDebugExceptionsEnabled, return);
			}
			this->filename = this->texture->getFilename();
			this->texture->setFilter(this->filter);
			this->texture->setAddressMode(this->addressMode);
			float factor = aprilui::findTextureExtensionScale(this->filename);
			this->scale.set(factor, factor);
		}
	}

	void Texture::addLink(Texture* link)
	{
		if (!this->links.has(link))
		{
			this->links += link;
			link->addLink(this);
		}
	}
	
	void Texture::removeLink(Texture* link)
	{
		if (this->links.has(link))
		{
			this->links -= link;
		}
	}
	
}
