/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can scroll TileImages vertically.

#ifndef APRILUI_TILE_SCROLLER_Y_H
#define APRILUI_TILE_SCROLLER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	class TileImage;

	namespace Animators
	{
		class apriluiExport TileScrollerY : public Animator
		{
			APRILUI_CLONEABLE(TileScrollerY);
		public:
			TileScrollerY(chstr name);
			~TileScrollerY();
			inline hstr getClassName() const { return "TileScrollerY"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			TileImage* _getParentsTileImage() const;
			
			void _update(float timeDelta);

		};

		DEPRECATED_ATTRIBUTE typedef TileScrollerY TiledScrollerY;

	}
}

#endif
