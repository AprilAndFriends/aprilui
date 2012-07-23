/// @file
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
		Texture(chstr filename, april::Texture* texture);
		~Texture();

		hstr getOriginalFilename() { return this->mOriginalFilename; }
		hstr getFilename() { return this->mFilename; }
		gvec2 getScale() { return this->mScale; }
		float getUnusedTime() { return this->mUnusedTime; }
		bool isDynamic() { return this->mDynamic; }

		april::Texture* getRenderTexture();
		int getWidth();
		int getHeight();
		bool isValid();
		void setFilter(april::Texture::Filter value);
		void setAddressMode(april::Texture::AddressMode value);

		void update(float k);
		void resetUnusedTime();

		void load();
		void unload();
		void reload(chstr filename);
		
		void addDynamicLink(Texture* link);
		void removeDynamicLink(Texture* link);

	protected:
		hstr mOriginalFilename;
		hstr mFilename;
		april::Texture* mTexture;
		gvec2 mScale;
		float mUnusedTime;
		bool mDynamic;
		harray<Texture*> mDynamicLinks;
		april::Texture::Filter mFilter;
		april::Texture::AddressMode mAddressMode;

	};

}
#endif
