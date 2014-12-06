/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can scroll TileImages horizontally.

#ifndef APRILUI_TILE_SCROLLER_X_H
#define APRILUI_TILE_SCROLLER_X_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	class TileImage;

	namespace Animators
	{
		class apriluiExport TileScrollerX : public Animator
		{
			APRILUI_CLONEABLE(TileScrollerX);
		public:
			TileScrollerX(chstr name);
			~TileScrollerX();
			inline hstr getClassName() const { return "TileScrollerX"; }

			static Animator* createInstance(chstr name);
			
			void update(float timeDelta);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			TileImage* _getParentsTileImage();
			
		};

		DEPRECATED_ATTRIBUTE typedef TileScrollerX TiledScrollerX;

	}
}

#endif
