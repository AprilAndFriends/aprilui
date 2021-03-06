/// @file
/// @version 5.1
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
	class Dataset;

	class apriluiExport Texture
	{
	public:
		friend class Dataset;

		Texture(chstr name, chstr filename, april::Texture* texture, bool managed = false);
		~Texture();

		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(hstr, originalFilename, OriginalFilename);
		HL_DEFINE_GET(hstr, filename, Filename);
		HL_DEFINE_GET(april::Texture*, texture, Texture);
		void setTexture(april::Texture* const& value);
		HL_DEFINE_GET(april::Texture::LoadMode, loadMode, LoadMode);
		HL_DEFINE_IS(managed, Managed);
		HL_DEFINE_GET(gvec2f, scale, Scale);
		HL_DEFINE_GET(Dataset*, dataset, Dataset);
		HL_DEFINE_GET(float, unusedTime, UnusedTime);
		HL_DEFINE_GET(harray<Texture*>, links, Links);
		int getWidth() const;
		int getHeight() const;
		bool isUploaded() const;
		bool isReadyForUpload() const;
		bool isAsyncLoadQueued() const;
		bool isUnloaded() const;
		bool isValid() const;
		void setFilter(april::Texture::Filter value);
		void setAddressMode(april::Texture::AddressMode value);

		void update(float timeDelta);
		void resetUnusedTime();

		bool loadMetaData(bool ignoreDynamicLinks = false);
		bool load(bool ignoreDynamicLinks = false);
		bool loadAsync(bool ignoreDynamicLinks = false);
		void unload();
		void reload(chstr filename);
		
		void addLink(Texture* link);
		void removeLink(Texture* link);
		
	protected:
		hstr name;
		hstr originalFilename;
		hstr filename;
		april::Texture* texture;
		april::Texture::LoadMode loadMode;
		bool managed;
		april::Texture::Filter filter;
		april::Texture::AddressMode addressMode;
		gvec2f scale;
		Dataset* dataset;
		harray<Texture*> links;
		float unusedTime;

	};

}
#endif
