/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can scroll tiled images horizontally.

#ifndef APRILUI_TILED_SCROLLER_X_H
#define APRILUI_TILED_SCROLLER_X_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	class TiledImage;

	namespace Animators
	{
		class apriluiExport TiledScrollerX : public Animator
		{
		public:
			TiledScrollerX(chstr name);
			~TiledScrollerX();
			inline hstr getClassName() const { return "TiledScrollerX"; }

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
