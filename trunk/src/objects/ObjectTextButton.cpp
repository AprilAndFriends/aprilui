/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <atres/atres.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectTextButton.h"

namespace aprilui
{
	TextButton::TextButton(chstr name, grect rect) :
		ButtonBase(),
		Label(name, rect)
	{
		mText = "TextButton: " + name;
		mTypeName = "TextButton";
		mBackground = true;
		mPushedTextColor.set(51, 51, 51);
		mHoverTextColor.set(127, 127, 127);
		mDisabledTextColor.set(127, 127, 127);
	}

	void TextButton::setTextKey(chstr key)
	{
		setText(mDataset->getText(key));
	}
	
	void TextButton::update(float k)
	{
		Object::update(k);
		ButtonBase::update(k);
	}

	void TextButton::OnDraw(gvec2 offset, gvec2 center)
	{
		if (mBackground)
		{
			grect rect = mRect + offset;
			// TODO - remove after implementing proper global rotation
			gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
			april::rendersys->setIdentityTransform();
			april::rendersys->translate(rect.x + rect.w / 2, rect.y + rect.h / 2);
			april::rendersys->rotate(getAngle());
			april::rendersys->drawColoredQuad(grect(-rect.getSize() / 2, rect.getSize()), april::Color(0, 0, 0, ((mHover && mPushed) ? 255 : 191)));
			april::rendersys->setModelviewMatrix(originalMatrix);
		}
		april::Color color = mTextColor;
		if (!isDerivedEnabled())
		{
			mTextColor = mDisabledTextColor;
		}
		else if (mHover)
		{
			mTextColor = (mPushed ? mPushedTextColor : mHoverTextColor);
		}
		Label::OnDraw(offset, center);
		mTextColor = color;
	}
	
	void TextButton::setProperty(chstr name, chstr value)
	{
		Label::setProperty(name, value);
		if		(name == "background")		mBackground = (bool)value;
		else if (name == "hover_color")		setHoverTextColor(value);
		else if (name == "pushed_color")	setPushedTextColor(value);
		else if (name == "disabled_color")	setDisabledTextColor(value);
	}
	
	bool TextButton::isCursorInside()
	{
		return Object::isCursorInside();
	}

	bool TextButton::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		return ButtonBase::OnMouseDown(x, y, button);
	}

	bool TextButton::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
		{
			return true;
		}
		bool result = ButtonBase::OnMouseUp(x, y, button);
		if (result)
		{
			triggerEvent("Click", x, y, 0);
		}
		return result;
	}

	void TextButton::OnMouseMove(float x, float y)
	{
		Object::OnMouseMove(x, y);
		ButtonBase::OnMouseMove(x, y);
	}

}
