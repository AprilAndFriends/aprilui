/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 3.12
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "aprilui.h"

namespace aprilui
{
	Animator::Animator(chstr name) : Object(name, grect(0.0f, 0.0f, 1.0f, 1.0f))
	{
		this->timeSinceLastFrame = 0.0f;
		this->value = 0.0f;
		this->animationFunction = aprilui::Object::Linear;
		this->timer = 0.0f;
		this->delay = 0.0f;
		this->periods = 1.0f;
		this->amplitude = 0.5f;
		this->speed = 1.0f;
		this->offset = 0.0f;
		this->acceleration = 0.0f;
		this->discreteStep = 0;
		this->reset = false;
		this->inheritValue = false;
		this->target = 0.0f;
		this->useTarget = false;
		this->customFunction = NULL;
	}
	
	Animator::~Animator()
	{
	}

	void Animator::OnDrawDebug()
	{
	}

	void Animator::update(float k)
	{
		this->timeSinceLastFrame = k;
		Object::update(this->timeSinceLastFrame);
		if (!this->enabled)
		{
			return;
		}
		if (this->delay > 0.0f)
		{
			this->delay -= this->timeSinceLastFrame;
			if (this->delay > 0.0f)
			{
				return;
			}
			this->notifyEvent("OnDelayEnd", NULL);
			this->timeSinceLastFrame += this->delay;
		}
		this->timer += this->timeSinceLastFrame;
		if (!heqf(this->acceleration, 0.0f, (float)HL_E_TOLERANCE))
		{
			this->speed += this->acceleration * this->timeSinceLastFrame;
		}
	}
	
	bool Animator::_checkUpdate(float k)
	{
		float delay = this->delay;
		bool animated = this->isAnimated();
		Animator::update(k);
		return (animated || this->isAnimated() || delay > 0.0f && this->delay <= 0.0f);
	}
	
	float Animator::_calculateValue(float k)
	{
		if (this->delay > 0.0f)
		{
			return (this->discreteStep != 0 ? hfloorf(this->offset / this->discreteStep) * this->discreteStep : this->offset);
		}
		float time = this->timer;
		if (this->isExpired())
		{
			if (this->reset)
			{
				return (this->discreteStep != 0 ? hfloorf(this->offset / this->discreteStep) * this->discreteStep : this->offset);
			}
			time = this->periods / habs(this->speed);
		}
		float result = 0.0f;
		switch (this->animationFunction)
		{
		case Object::Linear:
			result = time * this->speed * this->amplitude;
			break;
		case Object::Sine:
			result = (float)dsin(time * this->speed * 360) * this->amplitude;
			break;
		case Object::Square:
			result = (hmodf(time * this->speed, 1.0f) < 0.5f ? this->amplitude : -this->amplitude);
			break;
		case Object::Saw:
			result = (hmodf(time * this->speed + 0.5f, 1.0f) - 0.5f) * 2 * this->amplitude;
			break;
		case Object::Triangle:
			result = hmodf(time * this->speed, 1.0f);
			if (!is_in_range(result, 0.25f, 0.75f))
			{
				result = (hmodf(time * this->speed + 0.5f, 1.0f) - 0.5f) * 4 * this->amplitude;
			}
			else
			{
				result = -(hmodf(time * this->speed - 0.25f, 1.0f) - 0.25f) * 4 * this->amplitude;
			}
			break;
		case Object::Random:
			result = hrandf(-this->speed * this->amplitude, this->speed * this->amplitude);
			break;
		case Object::Hover:
			if ((this->amplitude >= 0.0f) == this->parent->isCursorInside())
			{
				result = hmin(this->value - this->offset + k * this->speed, (float)habs(this->amplitude));
			}
			else
			{
				result = hmax(this->value - this->offset - k * this->speed, -(float)habs(this->amplitude));
			}
			break;
		case Object::Custom:
			result = (this->customFunction != NULL ? this->customFunction(this, time) : this->value);
			break;
		}
		
		return (this->discreteStep != 0 ? hfloorf((result + this->offset) / this->discreteStep) * this->discreteStep : result + this->offset);
	}
	
	bool Animator::isAnimated()
	{
		if (!this->enabled)
		{
			return false;
		}
		if (this->animationFunction == Object::Hover)
		{
			return true;
		}
		if (this->delay > 0.0f)
		{
			return false;
		}
		if (this->isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isWaitingAnimation()
	{
		if (!this->enabled)
		{
			return false;
		}
		if (this->animationFunction == Object::Hover)
		{
			return true;
		}
		if (this->isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isExpired()
	{
		return (!this->enabled || this->periods >= 0.0f && this->timer * habs(this->speed) > this->periods);
	}
	
	void Animator::setTime(float value)
	{
		if (value > 0.0f)
		{
			this->speed = 1.0f / value;
		}
		else
		{
			hlog::warn(aprilui::logTag, "Cannot set \"time\" to 0 or less.");
		}
	}
	
	hstr Animator::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if		(name == "function" || name == "func")
		{
			if		(this->animationFunction == Object::Sine)		return "sine";
			else if	(this->animationFunction == Object::Saw)		return "saw";
			else if	(this->animationFunction == Object::Square)		return "square";
			else if	(this->animationFunction == Object::Triangle)	return "triangle";
			else if	(this->animationFunction == Object::Linear)		return "linear";
			else if	(this->animationFunction == Object::Random)		return "random";
			else if	(this->animationFunction == Object::Hover)		return "hover";
			else													return "custom";
		}
		else if	(name == "timer")			return this->getTimer();
		else if	(name == "delay")			return this->getDelay();
		else if	(name == "periods")			return this->getPeriods();
		else if	(name == "amplitude")		return this->getAmplitude();
		else if	(name == "amp")
		{
			hlog::warn(aprilui::logTag, "\"amp\" is deprecated. Use \"amplitude\" instead."); // DEPRECATED
			return this->getAmplitude();
		}
		else if	(name == "peak_to_peak")	return (2 * this->getAmplitude());
		else if	(name == "speed")			return this->getSpeed();
		else if	(name == "offset")			return this->getOffset();
		else if	(name == "acceleration")	return this->getAcceleration();
		else if	(name == "discrete_step")	return this->getDiscreteStep();
		else if	(name == "reset")			return this->isReset();
		else if	(name == "inherit_value")	return this->isInheritValue();
		// derived values
		else if	(name == "target")
		{
			return this->getTarget();
		}
		return Object::getProperty(name, propertyExists);
	}
	
	bool Animator::setProperty(chstr name, chstr value)
	{
		if		(name == "function" || name == "func")
		{
			if		(value == "sine")		this->setAnimationFunction(Object::Sine);
			else if	(value == "saw")		this->setAnimationFunction(Object::Saw);
			else if	(value == "square")		this->setAnimationFunction(Object::Square);
			else if	(value == "triangle")	this->setAnimationFunction(Object::Triangle);
			else if	(value == "linear")		this->setAnimationFunction(Object::Linear);
			else if	(value == "random")		this->setAnimationFunction(Object::Random);
			else if	(value == "hover")		this->setAnimationFunction(Object::Hover);
			else if	(value == "custom")		this->setAnimationFunction(Object::Custom);
		}
		else if	(name == "timer")			this->setTimer(value);
		else if	(name == "delay")			this->setDelay(value);
		else if	(name == "periods")			this->setPeriods(value);
		else if	(name == "amplitude")		this->setAmplitude(value);
		else if	(name == "amp")
		{
			hlog::warn(aprilui::logTag, "\"amp=\" is deprecated. Use \"amplitude=\" instead."); // DEPRECATED
			this->setAmplitude(value);
		}
		else if	(name == "peak_to_peak")	this->setAmplitude((float)value * 0.5f);
		else if	(name == "speed")			this->setSpeed(value);
		else if	(name == "offset")			this->setOffset(value);
		else if	(name == "acceleration")	this->setAcceleration(value);
		else if	(name == "discrete_step")	this->setDiscreteStep(value);
		else if	(name == "reset")			this->setReset(value);
		else if	(name == "inherit_value")	this->setInheritValue(value);
		// derived values
		else if	(name == "target")
		{
			this->setTarget(value);
			this->setUseTarget(true);
			this->setInheritValue(true);
		}
		else if	(name == "time")			this->setTime(value);
		else return Object::setProperty(name, value);
		return true;
	}
	
	void Animator::notifyEvent(chstr name, void* params)
	{
		if (name == "AttachToObject" || name == "OnDelayEnd" && this->inheritValue)
		{
			this->value = this->offset = this->_getObjectValue();
			if (this->useTarget)
			{
				this->amplitude = this->target - this->value;
			}
		}
		Object::notifyEvent(name, params);
	}
		
	void Animator::_valueUpdateSimple(float k)
	{
		if (this->_checkUpdate(k))
		{
			this->value = this->_getObjectValue(); // required because this->_calculateValue may use value
			this->value = this->_calculateValue(this->timeSinceLastFrame);
			this->_setObjectValue(this->value);
		}
	}
	
	void Animator::_valueUpdateUChar(float k)
	{
		if (this->_checkUpdate(k))
		{
			this->value = hclamp(this->_calculateValue(this->timeSinceLastFrame), 0.0f, 255.0f);
			if ((unsigned char)this->value != (unsigned char)this->_getObjectValue())
			{
				this->_setObjectValue(this->value);
			}
		}
	}
	
}
