/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines an base class for animators.

#ifndef APRILUI_ANIMATOR_H
#define APRILUI_ANIMATOR_H

#include <gtypes/Rectangle.h>

#include "Object.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport Animator : public Object
	{
	public:
		friend class Object;

		DEPRECATED_ATTRIBUTE static AnimationFunction Linear;
		DEPRECATED_ATTRIBUTE static AnimationFunction Sine;
		DEPRECATED_ATTRIBUTE static AnimationFunction Square;
		DEPRECATED_ATTRIBUTE static AnimationFunction Saw;
		DEPRECATED_ATTRIBUTE static AnimationFunction Triangle;
		DEPRECATED_ATTRIBUTE static AnimationFunction Random;
		DEPRECATED_ATTRIBUTE static AnimationFunction Hover;
		DEPRECATED_ATTRIBUTE static AnimationFunction Custom;

		Animator(chstr name);
		~Animator();
		
		AnimationFunction getAnimationFunction() { return this->mFunction; }
		void setAnimationFunction(AnimationFunction value) { this->mFunction = value; }
		float getValue() { return this->mValue; }
		void setValue(float value) { this->mValue = value; }
		float getTimer() { return this->mTimer; }
		void setTimer(float value) { this->mTimer = value; }
		float getDelay() { return this->mDelay; }
		void setDelay(float value) { this->mDelay = value; }
		float getPeriods() { return this->mPeriods; }
		void setPeriods(float value) { this->mPeriods = value; }
		float getAmplitude() { return this->mAmplitude; }
		void setAmplitude(float value) { this->mAmplitude = value; }
		float getSpeed() { return this->mSpeed; }
		void setSpeed(float value) { this->mSpeed = value; }
		float getOffset() { return this->mOffset; }
		void setOffset(float value) { this->mOffset = value; }
		float getAcceleration() { return this->mAcceleration; }
		void setAcceleration(float value) { this->mAcceleration = value; }
		int getDiscreteStep() { return this->mDiscreteStep; }
		void setDiscreteStep(int value) { this->mDiscreteStep = value; }
		bool isReset() { return this->mReset; }
		void setReset(bool value) { this->mReset = value; }
		bool getInheritValue() { return this->mInheritValue; }
		void setInheritValue(bool value) { this->mInheritValue = value; }
		float getTarget() { return this->mTarget; }
		void setTarget(float value) { this->mTarget = value; }
		bool isUseTarget() { return this->mUseTarget; }
		void setUseTarget(bool value) { this->mUseTarget = value; }
		float (*getCustomFunction())(Animator*, float) { return this->mCustomFunction; }
		void setCustomFunction(float (*value)(Animator*, float)) { this->mCustomFunction = value; }

		bool isAnimated();
		bool isWaitingAnimation();
		bool isExpired();
		void setTime(float value);
		
		hstr getProperty(chstr name, bool* propertyExists);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr name, void* params);
		
		void update(float k);

		void OnDrawDebug();
		
		DEPRECATED_ATTRIBUTE bool getUseTarget() { return this->isUseTarget(); }
		DEPRECATED_ATTRIBUTE bool getReset() { return this->isReset(); }

	protected:
		float mTimeSinceLastFrame;
		float mValue;
		AnimationFunction mFunction;
		float mTimer;
		float mDelay;
		float mPeriods;
		float mAmplitude;
		float mSpeed;
		float mOffset;
		float mAcceleration;
		int mDiscreteStep;
		bool mReset;
		bool mInheritValue;
		float mTarget;
		bool mUseTarget;
		float (*mCustomFunction)(Animator*, float);
		
		float _calculateValue(float k);
		bool _checkUpdate(float k);

		void _valueUpdateSimple(float k);
		void _valueUpdateUChar(float k);

		virtual float _getObjectValue() = 0;
		virtual void _setObjectValue(float value) = 0;

	};

}

#endif