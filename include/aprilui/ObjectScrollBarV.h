/// @file
/// @version 3.6
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
	class apriluiExport ScrollBarV : public ScrollBar
	{
	public:
		ScrollBarV(chstr name);
		~ScrollBarV();
		inline hstr getClassName() const { return "ScrollBarV"; }

		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args);

		void addScrollValue(float value);

		static hstr SkinNameVBackground;
		static hstr SkinNameVSliderNormal;
		static hstr SkinNameVSliderHover;
		static hstr SkinNameVSliderPushed;
		static hstr SkinNameVForwardNormal;
		static hstr SkinNameVForwardHover;
		static hstr SkinNameVForwardPushed;
		static hstr SkinNameVBackwardNormal;
		static hstr SkinNameVBackwardHover;
		static hstr SkinNameVBackwardPushed;

	protected:
		inline hstr _getSkinNameBackground() { return SkinNameVBackground; }
		inline hstr _getSkinNameSliderNormal() { return SkinNameVSliderNormal; }
		inline hstr _getSkinNameSliderHover() { return SkinNameVSliderHover; }
		inline hstr _getSkinNameSliderPushed() { return SkinNameVSliderPushed; }
		inline hstr _getSkinNameForwardNormal() { return SkinNameVForwardNormal; }
		inline hstr _getSkinNameForwardHover() { return SkinNameVForwardHover; }
		inline hstr _getSkinNameForwardPushed() { return SkinNameVForwardPushed; }
		inline hstr _getSkinNameBackwardNormal() { return SkinNameVBackwardNormal; }
		inline hstr _getSkinNameBackwardHover() { return SkinNameVBackwardHover; }
		inline hstr _getSkinNameBackwardPushed() { return SkinNameVBackwardPushed; }

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
