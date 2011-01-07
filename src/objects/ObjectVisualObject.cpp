/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/RenderSystem.h>
#include <april/Window.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Animator.h"
#include "aprilui.h"
#include "CallbackEvent.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Exception.h"
#include "ObjectVisualObject.h"

namespace aprilui
{
	VisualObject::VisualObject(chstr type, chstr name, grect rect) :
		Object(type, name)
	{
		mRect = rect;
		mZOrder = 0;
		mVisible = true;
		mAngle = 0.0f;
		mClickthrough = false;
		mInheritsAlpha = true;
		mDock = aprilui::TopLeft;
	}

	VisualObject::~VisualObject()
	{
	}
	
	void VisualObject::setZOrder(int value)
	{
		if (mZOrder != value)
		{
			mZOrder = value;
			Object::setZOrder(value);
		}
	}

	unsigned char VisualObject::getDerivedAlpha()
	{
		// recursive function that combines all the alpha from the parents (if any)
		float factor = 1.0f;
		if (mInheritsAlpha && mParent != NULL)
		{
			factor *= dynamic_cast<VisualObject*>(mParent)->getDerivedAlpha() / 255.0f;
		}
		return (unsigned char)(this->getAlpha() * factor);
	}

	void VisualObject::draw(gvec2 offset)
	{
		if (isVisible())
		{
			OnDraw(offset);
			offset += mRect.getPosition();
			gvec2 position = offset;
			VisualObject* child;
			foreach (Object*, it, mChildren)
			{
				position = offset;
				child = dynamic_cast<VisualObject*>(*it);
				if (child != NULL)
				{
					switch (child->getDock())
					{
					case TopLeft:
						break;
					case TopCenter:
						position.x += (mRect.w - child->getWidth()) / 2;
						break;
					case TopRight:
						position.x += mRect.w - child->getWidth();
						break;
					case CenterLeft:
						position.y += (mRect.h - child->getHeight()) / 2;
						break;
					case CenterCenter:
						position += (mRect.getSize() - child->getSize()) / 2;
						break;
					case CenterRight:
						position.x += mRect.w - child->getWidth();
						position.y += (mRect.h - child->getHeight()) / 2;
						break;
					case BottomLeft:
						position.y += mRect.h - child->getHeight();
						break;
					case BottomCenter:
						position.x += (mRect.w - child->getWidth()) / 2;
						position.y += mRect.h - child->getHeight();
						break;
					case BottomRight:
						position += mRect.getSize() - child->getSize();
						break;
					}
				}
				(*it)->draw(position);
			}
		}
	}
	
	bool VisualObject::isCursorInside()
	{
		gvec2 position = getCursorPosition();
		for (Object* p = mParent; p != NULL; p = p->getParent())
		{
			position -= p->getPosition();
		}
		return mRect.isPointInside(position);
	}

	bool VisualObject::OnMouseDown(float x, float y, int button)
	{
		if (mClickthrough || !isVisible() || !isDerivedEnabled())
		{
			return false;
		}
		return Object::OnMouseDown(x, y, button);
	}

	bool VisualObject::OnMouseUp(float x, float y, int button)
	{
		if (mClickthrough || !isVisible() || !isDerivedEnabled())
		{
			return false;
		}
		return Object::OnMouseUp(x, y, button);
	}

	void VisualObject::OnMouseMove(float x, float y)
	{
		if (isVisible() && isDerivedEnabled())
		{
			Object::OnMouseMove(x, y);
		}
	}

	bool VisualObject::isDerivedClickThrough()
	{
		VisualObject* parent = dynamic_cast<VisualObject*>(mParent);
		return (mClickthrough && (parent == NULL || parent->isDerivedClickThrough()));
	}
	
	void VisualObject::setAlpha(unsigned char value)
	{
		mColor.a = value;
	}

	void VisualObject::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if      (name == "visible")			setVisible(value);
		else if (name == "zorder")			setZOrder(value);
		else if (name == "clickthrough")	setClickthrough(value);
		else if (name == "inherits_alpha")	setInheritsAlpha(value);
		else if (name == "red")				setRed((int)value);
		else if (name == "green")			setGreen((int)value);
		else if (name == "blue")			setBlue((int)value);
		else if (name == "alpha")			setAlpha((int)value);
		else if (name == "color")			setColor(value);
		else if (name == "angle")			setAngle(value);
		else if (name == "dock")
		{
			if      (value == "top_left")		setDock(aprilui::TopLeft);
			else if (value == "top_center")		setDock(aprilui::TopCenter);
			else if (value == "top_right")		setDock(aprilui::TopRight);
			else if (value == "center_left")	setDock(aprilui::CenterLeft);
			else if (value == "center_center")	setDock(aprilui::CenterCenter);
			else if (value == "center_right")	setDock(aprilui::CenterRight);
			else if (value == "bottom_left")	setDock(aprilui::BottomLeft);
			else if (value == "bottom_center")	setDock(aprilui::BottomCenter);
			else if (value == "bottom_right")	setDock(aprilui::BottomRight);
		}
	}

	bool VisualObject::angleEquals(float angle)
	{
		float s1 = (float)dsin(angle);
		float s2 = (float)dsin(mAngle);
		float c1 = (float)dcos(angle);
		float c2 = (float)dcos(mAngle);
		return (fabs(s1 - s2) < 0.01f && fabs(c1 - c2) < 0.01f);
	}
	
	Object* VisualObject::getChildUnderPoint(gvec2 pos)
	{
		if (!isVisible() || !mRect.isPointInside(pos))
		{
			return NULL;
		}
		Object* object = Object::getChildUnderPoint(pos - mRect.getPosition());
		return (object != NULL ? object : this);
	}
	
	gvec2 VisualObject::getDerivedPosition()
	{
		gvec2 position = mRect.getPosition();
		for (Object* p = mParent; p != NULL; p = p->getParent())
		{
			position += p->getPosition();
		}
		return position;
	}
	
}
