/// @file
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
	namespace Animators
	{
		class apriluiExport TiledScrollerY : public Animator
		{
		public:
			TiledScrollerY(chstr name);
			~TiledScrollerY();
			static Animator* createInstance(chstr name);
			
			void update(float k);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			
		};
	}
}

#endif
