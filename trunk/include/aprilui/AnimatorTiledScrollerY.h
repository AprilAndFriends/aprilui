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
/// Defines an animator that can scroll tiled images vertically.

#ifndef APRILUI_TILED_SCROLLER_Y_H
#define APRILUI_TILED_SCROLLER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	class TiledImage;

	namespace Animators
	{
		class apriluiExport TiledScrollerY : public Animator
		{
		public:
			TiledScrollerY(chstr name);
			~TiledScrollerY();
			hstr getClassName() const { return "TiledScrollerY"; }

			static Animator* createInstance(chstr name);
			
			void update(float timeDelta);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			TiledImage* _getParentsTiledImage();
			
		};
	}
}

#endif
