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
/// Defines an base class for animators.

#ifndef APRILUI_ANIMATOR_H
#define APRILUI_ANIMATOR_H

#include <hltypes/henum.h>
#include <gtypes/Rectangle.h>

#include "apriluiExport.h"
#include "BaseObject.h"

namespace aprilui
{
	class apriluiExport Animator : public BaseObject
	{
		APRILUI_CLONEABLE_ABSTRACT(Animator)
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, AnimationFunction,
		(
			HL_ENUM_DECLARE(AnimationFunction, Linear);
			HL_ENUM_DECLARE(AnimationFunction, Sine);
			HL_ENUM_DECLARE(AnimationFunction, SineAbs);
			HL_ENUM_DECLARE(AnimationFunction, Square);
			HL_ENUM_DECLARE(AnimationFunction, Saw);
			HL_ENUM_DECLARE(AnimationFunction, Triangle);
			HL_ENUM_DECLARE(AnimationFunction, Noise);
			HL_ENUM_DECLARE(AnimationFunction, Custom);
		));

		friend class BaseObject;

		Animator(chstr name);
		~Animator();
		inline hstr getClassName() const { return "Animator"; }
		
		HL_DEFINE_GETSET(AnimationFunction, animationFunction, AnimationFunction);
		HL_DEFINE_GETSET(float, value, Value);
		HL_DEFINE_GETSET(float, timer, Timer);
		HL_DEFINE_GET(float, delay, Delay);
		void setDelay(const float& value);
		HL_DEFINE_GETSET(float, periods, Periods);
		HL_DEFINE_GETSET(float, amplitude, Amplitude);
		HL_DEFINE_GET(float, speed, Speed);
		void setSpeed(const float& value);
		HL_DEFINE_GETSET(float, offset, Offset);
		HL_DEFINE_GETSET(float, multiplier, Multiplier);
		HL_DEFINE_GETSET(float, acceleration, Acceleration);
		HL_DEFINE_GET(int, discreteStep, DiscreteStep);
		void setDiscreteStep(const int& value);
		HL_DEFINE_ISSET(resetOnExpire, ResetOnExpire);
		HL_DEFINE_ISSET(inheritValue, InheritValue);
		HL_DEFINE_GETSET(float, target, Target);
		HL_DEFINE_ISSET(useTarget, UseTarget);
		inline float (*getCustomFunction())(Animator*, float) { return this->customFunction; }
		inline void setCustomFunction(float (*value)(Animator*, float)) { this->customFunction = value; }

		bool isAnimated() const;
		bool isWaitingAnimation() const;
		bool isExpired() const;
		void setTime(float value);
		/// @note Same as setTimer() but in periods instead of seconds.
		void setPeriodsTimer(const float& value);

		harray<PropertyDescription> getPropertyDescriptions() const;

		/// @note Does not reset delay.
		void reset();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

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
		float multiplier;
		float acceleration;
		int discreteStep;
		bool resetOnExpire;
		bool inheritValue;
		float target;
		bool useTarget;
		float (*customFunction)(Animator*, float);
		
		void _update(float timeDelta);

		float _calculateValue(float timeDelta);
		bool _checkUpdate(float timeDelta);

		void _valueUpdateSimple(float timeDelta);
		void _valueUpdateUChar(float timeDelta);

		virtual float _getObjectValue() const = 0;
		virtual void _setObjectValue(float value) = 0;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}

#endif
