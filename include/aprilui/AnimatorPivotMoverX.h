/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can move object pivots horizontally.

#ifndef APRILUI_PIVOT_MOVER_X_H
#define APRILUI_PIVOT_MOVER_X_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport PivotMoverX : public Animator
		{
			APRILUI_CLONEABLE(PivotMoverX);
		public:
			PivotMoverX(chstr name);
			~PivotMoverX();
			inline hstr getClassName() const { return "PivotMoverX"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};
	}
}

#endif
