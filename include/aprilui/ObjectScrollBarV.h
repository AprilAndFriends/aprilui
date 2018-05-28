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
/// Defines a vertical scroll bar.

#ifndef APRILUI_SCROLL_BAR_V_H
#define APRILUI_SCROLL_BAR_V_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectScrollBar.h"

namespace aprilui
{
	class ScrollArea;

	class apriluiExport ScrollBarV : public ScrollBar
	{
		APRILUI_CLONEABLE(ScrollBarV);
	public:
		friend class ScrollArea;

		ScrollBarV(chstr name);
		~ScrollBarV();
		inline hstr getClassName() const { return "ScrollBarV"; }
		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args);

		void addScrollValue(float value);

		static hstr skinNameVBackground;
		static hstr skinNameVSliderNormal;
		static hstr skinNameVSliderHover;
		static hstr skinNameVSliderPushed;
		static hstr skinNameVForwardNormal;
		static hstr skinNameVForwardHover;
		static hstr skinNameVForwardPushed;
		static hstr skinNameVBackwardNormal;
		static hstr skinNameVBackwardHover;
		static hstr skinNameVBackwardPushed;

	protected:
		inline hstr _getSkinNameBackground() const { return skinNameVBackground; }
		inline hstr _getSkinNameSliderNormal() const { return skinNameVSliderNormal; }
		inline hstr _getSkinNameSliderHover() const { return skinNameVSliderHover; }
		inline hstr _getSkinNameSliderPushed() const { return skinNameVSliderPushed; }
		inline hstr _getSkinNameForwardNormal() const { return skinNameVForwardNormal; }
		inline hstr _getSkinNameForwardHover() const { return skinNameVForwardHover; }
		inline hstr _getSkinNameForwardPushed() const { return skinNameVForwardPushed; }
		inline hstr _getSkinNameBackwardNormal() const { return skinNameVBackwardNormal; }
		inline hstr _getSkinNameBackwardHover() const { return skinNameVBackwardHover; }
		inline hstr _getSkinNameBackwardPushed() const { return skinNameVBackwardPushed; }

		grectf _getBarDrawRect() const;

		float _calcScrollJump(float x, float y, cgvec2f size);
		float _calcScrollMove(float x, float y);
		void _updateChildren();
		void _moveScrollBar(float x, float y);
		void _updateBar();
		void _adjustDragSpeed();
		bool _checkAreaSize();

	};
}

#endif
