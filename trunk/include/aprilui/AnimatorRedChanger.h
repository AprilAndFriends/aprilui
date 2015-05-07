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
/// Defines an animator that can change the red channel of objects.

#ifndef APRILUI_RED_CHANGER_H
#define APRILUI_RED_CHANGER_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport RedChanger : public Animator
		{
			APRILUI_CLONEABLE(RedChanger);
		public:
			RedChanger(chstr name);
			~RedChanger();
			inline hstr getClassName() const { return "RedChanger"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};
	}
}

#endif
