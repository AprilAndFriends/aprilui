/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "ObjectFilledRect.h"

namespace aprilui
{
	FilledRect::FilledRect(chstr name) :
		Colored(name)
	{
	}

	FilledRect::FilledRect(const FilledRect& other) :
		Colored(other)
	{
	}

	Object* FilledRect::createInstance(chstr name)
	{
		return new FilledRect(name);
	}

	void FilledRect::_draw()
	{
		grectf drawRect = this->_makeDrawRect();
		this->vertices[0].x = this->vertices[2].x = this->vertices[4].x = drawRect.left();
		this->vertices[0].y = this->vertices[1].y = this->vertices[3].y = drawRect.top();
		this->vertices[1].x = this->vertices[3].x = this->vertices[5].x = drawRect.right();
		this->vertices[2].y = this->vertices[4].y = this->vertices[5].y = drawRect.bottom();
		unsigned int colorTopLeft = april::rendersys->getNativeColorUInt(this->_makeDrawColor());
		if (!this->useAdditionalColors)
		{
			for_iter (i, 0, APRILUI_COLOR_MAX_VERTICES)
			{
				this->vertices[i].color = colorTopLeft;
			}
		}
		else
		{
			this->vertices[0].color = colorTopLeft;
			this->vertices[1].color = this->vertices[3].color = april::rendersys->getNativeColorUInt(this->_makeDrawColor(april::Color(this->colorTopRight, (unsigned char)(this->colorTopRight.a_f() * this->color.a))));
			this->vertices[2].color = this->vertices[4].color = april::rendersys->getNativeColorUInt(this->_makeDrawColor(april::Color(this->colorBottomLeft, (unsigned char)(this->colorBottomLeft.a_f() * this->color.a)))); 
			this->vertices[5].color = april::rendersys->getNativeColorUInt(this->_makeDrawColor(april::Color(this->colorBottomRight, (unsigned char)(this->colorBottomRight.a_f() * this->color.a))));
		}
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		april::rendersys->render(april::RenderOperation::TriangleList, this->vertices, APRILUI_COLOR_MAX_VERTICES);
	}
	
}
