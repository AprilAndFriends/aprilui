/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		Texture(chstr name, chstr filename, april::Texture* texture, bool managed); // aprilui trunk compatibility
		~Texture();

		hstr getOriginalFilename() { return mOriginalFilename; }
		hstr getFilename() { return mFilename; }
		gvec2 getScale() { return mScale; }
		float getUnusedTime() { return mUnusedTime; }
		bool isDynamic() { return mDynamic; }

		april::Texture* getRenderTexture();
		april::Texture* getTexture(); // aprilui trunk compatibility
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
		hstr mName;
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
