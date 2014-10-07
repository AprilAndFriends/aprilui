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
		public:
			MoverX(chstr name);
			~MoverX();
			hstr getClassName() const { return "MoverX"; }

			static Animator* createInstance(chstr name);
			
			void update(float timeDelta);
			
		protected:
			float _getObjectValue();
			void _setObjectValue(float value);
			
		};
	}
}

#endif
