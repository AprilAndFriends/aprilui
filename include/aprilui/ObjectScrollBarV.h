/// @file
/// @version 4.1
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
		inline hstr _getSkinNameBackground() { return skinNameVBackground; }
		inline hstr _getSkinNameSliderNormal() { return skinNameVSliderNormal; }
		inline hstr _getSkinNameSliderHover() { return skinNameVSliderHover; }
		inline hstr _getSkinNameSliderPushed() { return skinNameVSliderPushed; }
		inline hstr _getSkinNameForwardNormal() { return skinNameVForwardNormal; }
		inline hstr _getSkinNameForwardHover() { return skinNameVForwardHover; }
		inline hstr _getSkinNameForwardPushed() { return skinNameVForwardPushed; }
		inline hstr _getSkinNameBackwardNormal() { return skinNameVBackwardNormal; }
		inline hstr _getSkinNameBackwardHover() { return skinNameVBackwardHover; }
		inline hstr _getSkinNameBackwardPushed() { return skinNameVBackwardPushed; }

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
