/// @file
/// @version 3.2
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
	class ScrollBarButtonBackground;
	class ScrollBarButtonBackward;
	class ScrollBarButtonForward;
	class ScrollBarButtonSlider;

	class apriluiExport ScrollBar : public Object
	{
	public:
		friend class ScrollArea;
		friend class ScrollBarButtonBackground;
		friend class ScrollBarButtonBackward;
		friend class ScrollBarButtonForward;
		friend class ScrollBarButtonSlider;

		ScrollBar(chstr name, grect rect);
		~ScrollBar();
		hstr getClassName() const { return "ScrollBar"; }


		HL_DEFINE_GETSET(float, gridSize, GridSize);
		HL_DEFINE_ISSET(useFading, UseFading);
		HL_DEFINE_ISSET(heightHide, HeightHide);
		HL_DEFINE_ISSET(useStretchedSlider, UseStretchedSlider);
		HL_DEFINE_GET(hstr, skinName, SkinName);
		void setSkinName(chstr value);

		harray<PropertyDescription> getPropertyDescriptions();

		void update(float timeDelta);
		void OnDraw();

		void notifyEvent(chstr name, void* params);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		bool onMouseMove();
		bool onMouseScroll(float x, float y);

		virtual void addScrollValue(float value) = 0;
		void addScrollValueForward(float multiplier = 1.0f);
		void addScrollValueBackward(float multiplier = 1.0f);

		static float ScrollDistance;
		static float GridSize;

	protected:
		hstr skinName;
		float gridSize;
		bool useFading;
		bool heightHide;
		bool useStretchedSlider;
		float _retainTime;
		ScrollBarButtonBackground* _buttonBackground;
		ScrollBarButtonSlider* _buttonSlider;
		ScrollBarButtonForward* _buttonForward;
		ScrollBarButtonBackward* _buttonBackward;
		ScrollBarButtonBackground* _buttonBackgroundSkinned;
		ScrollBarButtonSlider* _buttonSliderSkinned;
		ScrollBarButtonForward* _buttonForwardSkinned;
		ScrollBarButtonBackward* _buttonBackwardSkinned;

		ScrollBarButtonBackground* _getButtonBackground();
		void _setButtonBackground(ScrollBarButtonBackground* button);
		void _unsetButtonBackground(ScrollBarButtonBackground* button);
		ScrollBarButtonSlider* _getButtonSlider();
		void _setButtonSlider(ScrollBarButtonSlider* button);
		void _unsetButtonSlider(ScrollBarButtonSlider* button);
		ScrollBarButtonForward* _getButtonForward();
		void _setButtonForward(ScrollBarButtonForward* button);
		void _unsetButtonForward(ScrollBarButtonForward* button);
		ScrollBarButtonBackward* _getButtonBackward();
		void _setButtonBackward(ScrollBarButtonBackward* button);
		void _unsetButtonBackward(ScrollBarButtonBackward* button);

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

		/// @note x and y range from 0 to 1
		virtual float _calcScrollJump(float x, float y, gvec2 size) = 0;
		virtual float _calcScrollMove(float x, float y) = 0;
		virtual void _updateChildren() = 0;
		virtual void _moveScrollBar(float x, float y) = 0;
		virtual void _updateBar() = 0;
		virtual void _adjustDragSpeed() = 0;
		virtual bool _checkAreaSize() = 0;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		gvec2 _clickPosition;
		bool _scrolling;

	};
}

#endif
