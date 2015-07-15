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
/// Defines an animator that can move object centers horizontally.

#ifndef APRILUI_CENTER_MOVER_X_H
#define APRILUI_CENTER_MOVER_X_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport CenterMoverX : public Animator
		{
			APRILUI_CLONEABLE(CenterMoverX);
		public:
			CenterMoverX(chstr name);
			~CenterMoverX();
			inline hstr getClassName() const { return "CenterMoverX"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};
	}
}

#endif
