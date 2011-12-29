/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 1.4
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
		enum AnimationFunction
		{
			Linear,
			Sine,
			Square,
			Saw,
			Triangle,
			Random,
			Hover,
			Custom
		};
	
		Animator(chstr name);
		~Animator();
		
		AnimationFunction getAnimationFunction() { return mFunction; }
		void setAnimationFunction(AnimationFunction value) { mFunction = value; }
		float getValue() { return mValue; }
		void setValue(float value) { mValue = value; }
		float getTimer() { return mTimer; }
		void setTimer(float value) { mTimer = value; }
		float getDelay() { return mDelay; }
		void setDelay(float value) { mDelay = value; }
		float getPeriods() { return mPeriods; }
		void setPeriods(float value) { mPeriods = value; }
		float getAmplitude() { return mAmplitude; }
		void setAmplitude(float value) { mAmplitude = value; }
		float getSpeed() { return mSpeed; }
		void setSpeed(float value) { mSpeed = value; }
		float getOffset() { return mOffset; }
		void setOffset(float value) { mOffset = value; }
		float getAcceleration() { return mAcceleration; }
		void setAcceleration(float value) { mAcceleration = value; }
		int getDiscreteStep() { return mDiscreteStep; }
		void setDiscreteStep(int value) { mDiscreteStep = value; }
		bool getReset() { return mReset; }
		void setReset(bool value) { mReset = value; }
		bool getInheritValue() { return mInheritValue; }
		void setInheritValue(bool value) { mInheritValue = value; }
		float getTarget() { return mTarget; }
		void setTarget(float value) { mTarget = value; }
		bool getUseTarget() { return mUseTarget; }
		void setUseTarget(bool value) { mUseTarget = value; }
		float (*getCustomFunction())(Animator*, float) { return mCustomFunction; }
		void setCustomFunction(float (*value)(Animator*, float)) { mCustomFunction = value; }

		bool isAnimated();
		bool isWaitingAnimation();
		bool isExpired();
		void setTime(float value);
		
		bool setProperty(chstr name, chstr value);
		
		void update(float k);
		
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
		
	};
}

#endif
