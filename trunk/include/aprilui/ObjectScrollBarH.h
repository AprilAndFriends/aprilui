/// @file
/// @version 3.4
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
	class apriluiExport ScrollBarH : public ScrollBar
	{
	public:
		ScrollBarH(chstr name, grect rect);
		~ScrollBarH();
		hstr getClassName() const { return "ScrollBarH"; }

		static Object* createInstance(chstr name, grect rect);

		void notifyEvent(chstr type, EventArgs* args);

		void addScrollValue(float value);

		static hstr SkinNameHBackground;
		static hstr SkinNameHSliderNormal;
		static hstr SkinNameHSliderHover;
		static hstr SkinNameHSliderPushed;
		static hstr SkinNameHForwardNormal;
		static hstr SkinNameHForwardHover;
		static hstr SkinNameHForwardPushed;
		static hstr SkinNameHBackwardNormal;
		static hstr SkinNameHBackwardHover;
		static hstr SkinNameHBackwardPushed;

	protected:
		inline hstr _getSkinNameBackground() { return SkinNameHBackground; }
		inline hstr _getSkinNameSliderNormal() { return SkinNameHSliderNormal; }
		inline hstr _getSkinNameSliderHover() { return SkinNameHSliderHover; }
		inline hstr _getSkinNameSliderPushed() { return SkinNameHSliderPushed; }
		inline hstr _getSkinNameForwardNormal() { return SkinNameHForwardNormal; }
		inline hstr _getSkinNameForwardHover() { return SkinNameHForwardHover; }
		inline hstr _getSkinNameForwardPushed() { return SkinNameHForwardPushed; }
		inline hstr _getSkinNameBackwardNormal() { return SkinNameHBackwardNormal; }
		inline hstr _getSkinNameBackwardHover() { return SkinNameHBackwardHover; }
		inline hstr _getSkinNameBackwardPushed() { return SkinNameHBackwardPushed; }

		grect _getBarDrawRect();

		float _calcScrollJump(float x, float y, gvec2 size);
		float _calcScrollMove(float x, float y);
		void _updateChildren();
		void _moveScrollBar(float x, float y);
		void _updateBar();
		void _adjustDragSpeed();
		bool _checkAreaSize();

	};
}

#endif
