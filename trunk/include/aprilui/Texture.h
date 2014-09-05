/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a texture class for wrapping the texture class in the underlying system.

#ifndef APRILUI_TEXTURE_H
#define APRILUI_TEXTURE_H

#include <april/Texture.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport Texture
	{
	public:
		Texture(chstr filename, april::Texture* texture, bool managed = false);
		~Texture();

		HL_DEFINE_GET(hstr, originalFilename, OriginalFilename);
		HL_DEFINE_GET(hstr, filename, Filename);
		HL_DEFINE_GET(april::Texture::LoadMode, loadMode, LoadMode);
		HL_DEFINE_IS(managed, Managed);
		HL_DEFINE_GET(gvec2, scale, Scale);
		HL_DEFINE_GET(float, unusedTime, UnusedTime);
		HL_DEFINE_GET(harray<Texture*>, links, Links);
		int getWidth();
		int getHeight();
		bool isLoaded();
		bool isLoadedAsync();
		bool isAsyncLoadQueued();
		bool isValid();
		void setFilter(april::Texture::Filter value);
		void setAddressMode(april::Texture::AddressMode value);
		april::Texture* getRenderTexture();

		void update(float timeDelta);
		void resetUnusedTime();

		void load(bool ignoreDynamicLinks = false);
		void loadAsync(bool ignoreDynamicLinks = false);
		void unload();
		void reload(chstr filename);
		
		void addLink(Texture* link);
		void removeLink(Texture* link);
		
		static void setLoadListener(void (*callback)(Texture*));
		static void setUnloadListener(void (*callback)(Texture*));

	protected:
		hstr originalFilename;
		hstr filename;
		april::Texture* texture;
		april::Texture::LoadMode loadMode;
		bool managed;
		gvec2 scale;
		float unusedTime;
		harray<Texture*> links;
		april::Texture::Filter filter;
		april::Texture::AddressMode addressMode;

		static void (*loadListener)(Texture*);
		static void (*unloadListener)(Texture*);

	};

}
#endif
