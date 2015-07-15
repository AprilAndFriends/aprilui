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
/// Defines an animator that can rotate objects.

#ifndef APRILUI_ROTATOR_H
#define APRILUI_ROTATOR_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Rotator : public Animator
		{
			APRILUI_CLONEABLE(Rotator);
		public:
			Rotator(chstr name);
			~Rotator();
			inline hstr getClassName() const { return "Rotator"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};
	}
}

#endif
