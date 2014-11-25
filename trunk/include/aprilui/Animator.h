/// @file
/// @version 3.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an base class for animators.

#ifndef APRILUI_ANIMATOR_H
#define APRILUI_ANIMATOR_H

#include <gtypes/Rectangle.h>

#include "apriluiExport.h"
#include "BaseObject.h"

namespace aprilui
{
	class apriluiExport Animator : public BaseObject
	{
	public:
		enum AnimationFunction
		{
			Linear,
			Sine,
			Square,
			Saw,
			Triangle,
			Noise,
			Custom
		};

		friend class BaseObject;

		Animator(chstr name);
		~Animator();
		inline hstr getClassName() const { return "Animator"; }
		
		HL_DEFINE_GETSET(AnimationFunction, animationFunction, AnimationFunction);
		HL_DEFINE_GETSET(float, value, Value);
		HL_DEFINE_GETSET(float, timer, Timer);
		HL_DEFINE_GETSET(float, delay, Delay);
		HL_DEFINE_GETSET(float, periods, Periods);
		HL_DEFINE_GETSET(float, amplitude, Amplitude);
		HL_DEFINE_GETSET(float, speed, Speed);
		HL_DEFINE_GETSET(float, offset, Offset);
		HL_DEFINE_GETSET(float, acceleration, Acceleration);
		HL_DEFINE_GETSET(int, discreteStep, DiscreteStep);
		HL_DEFINE_ISSET(reset, Reset);
		HL_DEFINE_ISSET(inheritValue, InheritValue);
		HL_DEFINE_GETSET(float, target, Target);
		HL_DEFINE_ISSET(useTarget, UseTarget);
		inline float (*getCustomFunction())(Animator*, float) { return this->customFunction; }
		inline void setCustomFunction(float (*value)(Animator*, float)) { this->customFunction = value; }

		bool isAnimated();
		bool isWaitingAnimation();
		bool isExpired();
		void setTime(float value);

		harray<PropertyDescription> getPropertyDescriptions();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);
		
		void update(float timeDelta);
		
	protected:
		float timeDelta;
		float value;
		AnimationFunction animationFunction;
		float timer;
		float delay;
		float periods;
		float amplitude;
		float speed;
		float offset;
		float acceleration;
		int discreteStep;
		bool reset;
		bool inheritValue;
		float target;
		bool useTarget;
		float (*customFunction)(Animator*, float);
		
		float _calculateValue(float timeDelta);
		bool _checkUpdate(float timeDelta);

		void _valueUpdateSimple(float timeDelta);
		void _valueUpdateUChar(float timeDelta);

		virtual float _getObjectValue() = 0;
		virtual void _setObjectValue(float value) = 0;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}

#endif
