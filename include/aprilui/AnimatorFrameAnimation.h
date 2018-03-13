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
/// Defines an animator that can switch images to display them in a sequence.

#ifndef APRILUI_FRAME_ANIMATION_H
#define APRILUI_FRAME_ANIMATION_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport FrameAnimation : public Animator
		{
			APRILUI_CLONEABLE(FrameAnimation);
		public:
			FrameAnimation(chstr name);
			~FrameAnimation();
			inline hstr getClassName() const { return "FrameAnimation"; }

			static Animator* createInstance(chstr name);

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(hstr, imageBaseName, ImageBaseName);
			HL_DEFINE_GETSET(int, firstFrame, FirstFrame);
			HL_DEFINE_GETSET(int, frameCount, FrameCount);
			bool isAnimated() const;

			bool setProperty(chstr name, chstr value);

			void notifyEvent(chstr type, EventArgs* args);
			
		protected:
			hstr imageBaseName;
			int firstFrame;
			int frameCount;
			
			hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
			hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

			inline float _getObjectValue() const { return 0.0f; }
			inline void _setObjectValue(float value) { }

			void _update(float timeDelta);

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;
			static hmap<hstr, PropertyDescription::Accessor*> _getters;
			static hmap<hstr, PropertyDescription::Accessor*> _setters;

		};

	}
}
#endif
