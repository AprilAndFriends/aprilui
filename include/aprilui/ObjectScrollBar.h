/// @file
/// @author  Boris Mikic
/// @version 3.06
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a generic scroll bar.

#ifndef APRILUI_SCROLL_BAR_H
#define APRILUI_SCROLL_BAR_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class ImageButton;
	class ScrollArea;

	class apriluiExport ScrollBar : public Object
	{
	public:
		friend class ScrollArea;

		ScrollBar(chstr name, grect rect);
		~ScrollBar();

		HL_DEFINE_GETSET(float, gridSize, GridSize);
		HL_DEFINE_ISSET(useFading, UseFading);
		HL_DEFINE_ISSET(heightHide, HeightHide);
		HL_DEFINE_GET(hstr, skinName, SkinName);
		void setSkinName(chstr value);

		void update(float k);
		void OnDraw();

		void notifyEvent(chstr name, void* params);

		hstr getProperty(chstr name, bool* propertyExists);
		bool setProperty(chstr name, chstr value);

		bool onMouseMove();
		bool onMouseScroll(float x, float y);

		virtual void addScrollValue(float value) = 0;
		void addScrollValueBegin(float multiplier = 1.0f);
		void addScrollValueEnd(float multiplier = 1.0f);

		static float ScrollDistance;
		static float GridSize;

	protected:
		hstr skinName;
		float gridSize;
		bool useFading;
		bool heightHide;
		ImageButton* buttonBackground;
		ImageButton* buttonSlider;
		ImageButton* buttonForward;
		ImageButton* buttonBackward;
		float _retainTime;

		void _initAreaDragging();

		virtual hstr _getSkinNameBackground() = 0;
		virtual hstr _getSkinNameSliderNormal() = 0;
		virtual hstr _getSkinNameSliderHover() = 0;
		virtual hstr _getSkinNameSliderPushed() = 0;
		virtual hstr _getSkinNameForwardNormal() = 0;
		virtual hstr _getSkinNameForwardHover() = 0;
		virtual hstr _getSkinNameForwardPushed() = 0;
		virtual hstr _getSkinNameBackwardNormal() = 0;
		virtual hstr _getSkinNameBackwardHover() = 0;
		virtual hstr _getSkinNameBackwardPushed() = 0;

		virtual grect _getBarDrawRect() = 0;

		virtual float _calcScrollJump(float x, float y) = 0;
		virtual float _calcScrollMove(float x, float y) = 0;
		virtual void _updateChildren() = 0;
		virtual void _moveScrollBar(float x, float y) = 0;
		virtual void _updateBar() = 0;
		virtual void _adjustDragSpeed() = 0;
		virtual bool _checkAreaSize() = 0;

		static void _clickScrollBackward(EventArgs* args);
		static void _clickScrollForward(EventArgs* args);
		static void _clickScrollBack(EventArgs* args);
		static void _mouseDownScrollBar(EventArgs* args);
		static void _clickScrollBar(EventArgs* args);

	private:
		gvec2 _clickPosition;
		bool _scrolling;

	};
}

#endif
