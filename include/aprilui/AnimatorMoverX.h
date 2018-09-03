/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can move objects horizontally.

#ifndef APRILUI_MOVER_X_H
#define APRILUI_MOVER_X_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport MoverX : public Animator
		{
			APRILUI_CLONEABLE(MoverX);
		public:
			MoverX(chstr name);
			inline hstr getClassName() const { return "MoverX"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};

	}
}
#endif
