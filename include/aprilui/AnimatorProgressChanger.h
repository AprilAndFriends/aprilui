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
/// Defines an animator that can change the progress value on ProgressBase objects.

#ifndef APRILUI_PROGRESS_CHANGER_H
#define APRILUI_PROGRESS_CHANGER_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport ProgressChanger : public Animator
		{
			APRILUI_CLONEABLE(ProgressChanger);
		public:
			ProgressChanger(chstr name);
			~ProgressChanger();
			inline hstr getClassName() const { return "ProgressChanger"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};
	}
}

#endif
