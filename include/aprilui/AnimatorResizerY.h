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
/// Defines an animator that can resize objects vertically.

#ifndef APRILUI_RESIZER_Y_H
#define APRILUI_RESIZER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport ResizerY : public Animator
		{
		public:
			ResizerY(chstr name);
			~ResizerY();
			hstr getClassName() const { return "ResizerY"; }

			static Animator* createInstance(chstr name);
			
			void update(float timeDelta);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			
		};
	}
}

#endif
