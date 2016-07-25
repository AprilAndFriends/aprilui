/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include <april/april.h>
#include <april/Color.h>
#include <april/Image.h>
#include <april/RenderSystem.h>
#include "RamTexture.h"
#include "apriluiUtil.h"

namespace aprilui
{
	// TODO - this entire class needs to be removed
	RamTexture::RamTexture(chstr filename)
	{
		this->filename = filename;
		this->source = NULL;
		hlog::write(aprilui::logTag, "Registering RAM texture." + filename);
	}

	RamTexture::RamTexture(int w, int h)
	{
		this->width = w;
		this->height = h;
		this->source = april::Image::create(w, h, april::Color::Clear, april::Image::FORMAT_RGBA);
		hlog::write(aprilui::logTag, "Creating RAM texture.");
	}

	RamTexture::~RamTexture()
	{
		this->unload();
	}

	bool RamTexture::load()
	{
		if (this->source == NULL)
		{
			hlog::writef(aprilui::logTag, "Loading RAM texture '%s'.", this->filename.cStr());
			if (this->filename != "")
			{
				this->source = april::Image::createFromResource(this->filename);
				this->width = this->source->w;
				this->height = this->source->h;
			}
			else
			{
				this->source = april::Image::create(this->width, this->height, april::Color::Clear, april::Image::FORMAT_RGBA);
			}
			return true;
		}
		return false;
	}
	
	void RamTexture::unload()
	{
		if (this->source != NULL)
		{
			hlog::writef(aprilui::logTag, "Unloading RAM texture '%s'.", this->filename.cStr());
			delete this->source;
			this->source = NULL;
		}
	}
	
	bool RamTexture::isLoaded()
	{
		return (this->source != NULL);
	}
	
	april::Color RamTexture::getInterpolatedPixel(float x, float y)
	{
		april::Color color = april::Color::White;
		if (this->source == NULL) load();
		if (this->source != NULL)
		{
			color = this->source->getInterpolatedPixel(x, y);
		}
		return color;
	}
	
	april::Color RamTexture::getPixel(int x, int y)
	{
		if (this->source == NULL)
		{
			this->load();
		}
		return this->source->getPixel(x, y);
	}
	
	bool RamTexture::setPixel(int x, int y, april::Color color)
	{
		if (this->source == NULL)
		{
			this->load();
		}
		return this->source->setPixel(x, y, color);
	}
		
}
