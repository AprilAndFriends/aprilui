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
/// Defines an animator that can move object centers vertically.

#ifndef APRILUI_CENTER_MOVER_Y_H
#define APRILUI_CENTER_MOVER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport CenterMoverY : public Animator
		{
		public:
			CenterMoverY(chstr name);
			~CenterMoverY();
			hstr getClassName() const { return "CenterMoverY"; }

			static Animator* createInstance(chstr name);
			
			void update(float timeDelta);

		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			
		};
	}
}

#endif
