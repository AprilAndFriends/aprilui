/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can resize objects horizontally.

#ifndef APRILUI_RESIZER_X_H
#define APRILUI_RESIZER_X_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport ResizerX : public Animator
		{
			APRILUI_CLONEABLE(ResizerX);
		public:
			ResizerX(chstr name);
			~ResizerX();
			inline hstr getClassName() const { return "ResizerX"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};
	}
}

#endif
