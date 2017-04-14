/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a horizontal scroll bar.

#ifndef APRILUI_SCROLL_BAR_H_H
#define APRILUI_SCROLL_BAR_H_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectScrollBar.h"

namespace aprilui
{
	class ScrollArea;

	class apriluiExport ScrollBarH : public ScrollBar
	{
		APRILUI_CLONEABLE(ScrollBarH);
	public:
		friend class ScrollArea;

		ScrollBarH(chstr name);
		~ScrollBarH();
		inline hstr getClassName() const { return "ScrollBarH"; }

		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args);

		void addScrollValue(float value);

		static hstr skinNameHBackground;
		static hstr skinNameHSliderNormal;
		static hstr skinNameHSliderHover;
		static hstr skinNameHSliderPushed;
		static hstr skinNameHForwardNormal;
		static hstr skinNameHForwardHover;
		static hstr skinNameHForwardPushed;
		static hstr skinNameHBackwardNormal;
		static hstr skinNameHBackwardHover;
		static hstr skinNameHBackwardPushed;

	protected:
		inline hstr _getSkinNameBackground() const { return skinNameHBackground; }
		inline hstr _getSkinNameSliderNormal() const { return skinNameHSliderNormal; }
		inline hstr _getSkinNameSliderHover() const { return skinNameHSliderHover; }
		inline hstr _getSkinNameSliderPushed() const { return skinNameHSliderPushed; }
		inline hstr _getSkinNameForwardNormal() const { return skinNameHForwardNormal; }
		inline hstr _getSkinNameForwardHover() const { return skinNameHForwardHover; }
		inline hstr _getSkinNameForwardPushed() const { return skinNameHForwardPushed; }
		inline hstr _getSkinNameBackwardNormal() const { return skinNameHBackwardNormal; }
		inline hstr _getSkinNameBackwardHover() const { return skinNameHBackwardHover; }
		inline hstr _getSkinNameBackwardPushed() const { return skinNameHBackwardPushed; }

		grect _getBarDrawRect() const;

		float _calcScrollJump(float x, float y, cgvec2 size);
		float _calcScrollMove(float x, float y);
		void _updateChildren();
		void _moveScrollBar(float x, float y);
		void _updateBar();
		void _adjustDragSpeed();
		bool _checkAreaSize();

	};
}

#endif
