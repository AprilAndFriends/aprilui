/// @file
/// @version 3.2
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
	harray<PropertyDescription> Animator::_propertyDescriptions;

	Animator::Animator(chstr name) : Object(name, grect(0.0f, 0.0f, 1.0f, 1.0f))
	{
		this->timeDelta = 0.0f;
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

	harray<PropertyDescription> Animator::getPropertyDescriptions()
	{
		if (Animator::_propertyDescriptions.size() == 0)
		{
			Animator::_propertyDescriptions += PropertyDescription("function", PropertyDescription::TYPE_ENUM);
			Animator::_propertyDescriptions += PropertyDescription("func", PropertyDescription::TYPE_ENUM);
			Animator::_propertyDescriptions += PropertyDescription("timer", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("delay", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("periods", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("amplitude", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("peak_to_peak", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("speed", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("offset", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("acceleration", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("discrete_step", PropertyDescription::TYPE_INT);
			Animator::_propertyDescriptions += PropertyDescription("reset", PropertyDescription::TYPE_BOOL);
			Animator::_propertyDescriptions += PropertyDescription("inherit_value", PropertyDescription::TYPE_BOOL);
			Animator::_propertyDescriptions += PropertyDescription("target", PropertyDescription::TYPE_FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("time", PropertyDescription::TYPE_FLOAT);
		}
		return (Object::getPropertyDescriptions() + Animator::_propertyDescriptions);
	}

	void Animator::OnDrawDebug()
	{
	}

	void Animator::update(float timeDelta)
	{
		this->timeDelta = timeDelta;
		Object::update(this->timeDelta);
		if (!this->enabled)
		{
			return;
		}
		if (this->delay > 0.0f)
		{
			this->delay -= this->timeDelta;
			if (this->delay > 0.0f)
			{
				return;
			}
			this->notifyEvent("OnDelayEnd", NULL);
			this->timeDelta += this->delay;
		}
		this->timer += this->timeDelta;
		if (!heqf(this->acceleration, 0.0f, (float)HL_E_TOLERANCE))
		{
			this->speed += this->acceleration * this->timeDelta;
		}
	}
	
	bool Animator::_checkUpdate(float timeDelta)
	{
		float delay = this->delay;
		bool animated = this->isAnimated();
		Animator::update(timeDelta);
		return (animated || this->isAnimated() || delay > 0.0f && this->delay <= 0.0f);
	}
	
	float Animator::_calculateValue(float timeDelta)
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
				result = hmin(this->value - this->offset + timeDelta * this->speed, (float)habs(this->amplitude));
			}
			else
			{
				result = hmax(this->value - this->offset - timeDelta * this->speed, -(float)habs(this->amplitude));
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
	
	hstr Animator::getProperty(chstr name)
	{
		if (name == "function" || name == "func")
		{
			if (this->animationFunction == Object::Sine)		return "sine";
			if (this->animationFunction == Object::Saw)			return "saw";
			if (this->animationFunction == Object::Square)		return "square";
			if (this->animationFunction == Object::Triangle)	return "triangle";
			if (this->animationFunction == Object::Linear)		return "linear";
			if (this->animationFunction == Object::Random)		return "random";
			if (this->animationFunction == Object::Hover)		return "hover";
			return "custom";
		}
		if (name == "timer")			return this->getTimer();
		if (name == "delay")			return this->getDelay();
		if (name == "periods")			return this->getPeriods();
		if (name == "amplitude")		return this->getAmplitude();
		if (name == "peak_to_peak")		return (2 * this->getAmplitude());
		if (name == "speed")			return this->getSpeed();
		if (name == "offset")			return this->getOffset();
		if (name == "acceleration")		return this->getAcceleration();
		if (name == "discrete_step")	return this->getDiscreteStep();
		if (name == "reset")			return this->isReset();
		if (name == "inherit_value")	return this->isInheritValue();
		// derived values
		if	(name == "target")			return this->getTarget();
		return Object::getProperty(name);
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
		
	void Animator::_valueUpdateSimple(float timeDelta)
	{
		if (this->_checkUpdate(timeDelta))
		{
			this->value = this->_getObjectValue(); // required because this->_calculateValue may use value
			this->value = this->_calculateValue(this->timeDelta);
			this->_setObjectValue(this->value);
		}
	}
	
	void Animator::_valueUpdateUChar(float timeDelta)
	{
		if (this->_checkUpdate(timeDelta))
		{
			this->value = hclamp(this->_calculateValue(this->timeDelta), 0.0f, 255.0f);
			if ((unsigned char)this->value != (unsigned char)this->_getObjectValue())
			{
				this->_setObjectValue(this->value);
			}
		}
	}
	
}
