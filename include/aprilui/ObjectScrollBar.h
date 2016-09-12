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
		APRILUI_CLONEABLE_ABSTRACT(ScrollBar);
	public:
		friend class ScrollArea;
		friend class ScrollBarButtonBackground;
		friend class ScrollBarButtonBackward;
		friend class ScrollBarButtonForward;
		friend class ScrollBarButtonSlider;

		ScrollBar(chstr name);
		~ScrollBar();
		inline hstr getClassName() const { return "ScrollBar"; }

		HL_DEFINE_GETSET(float, gridSize, GridSize);
		HL_DEFINE_ISSET(useFading, UseFading);
		HL_DEFINE_ISSET(heightHide, HeightHide);
		HL_DEFINE_ISSET(useStretchedSlider, UseStretchedSlider);
		HL_DEFINE_ISSET(disabledWhileScrolling, DisabledWhileScrolling);
		HL_DEFINE_GETSET(int, maxGridScroll, MaxGridScroll);
		HL_DEFINE_GET(hstr, skinName, SkinName);
		void setSkinName(chstr value);

		harray<PropertyDescription> getPropertyDescriptions() const;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

		virtual void addScrollValue(float value) = 0;
		void addScrollValueBackground(float value);
		void addScrollValueForward(float multiplier = 1.0f);
		void addScrollValueBackward(float multiplier = 1.0f);

		static float defaultScrollDistance;
		static float defaultGridSize;

	protected:
		hstr skinName;
		float gridSize;
		int maxGridScroll;
		bool useFading;
		bool heightHide;
		bool useStretchedSlider;
		bool disabledWhileScrolling;
		ScrollBarButtonBackground* _buttonBackground;
		ScrollBarButtonSlider* _buttonSlider;
		ScrollBarButtonForward* _buttonForward;
		ScrollBarButtonBackward* _buttonBackward;
		ScrollBarButtonBackground* _buttonBackgroundSkinned;
		ScrollBarButtonSlider* _buttonSliderSkinned;
		ScrollBarButtonForward* _buttonForwardSkinned;
		ScrollBarButtonBackward* _buttonBackwardSkinned;

		ScrollBarButtonBackground* _getButtonBackground() const;
		void _setButtonBackground(ScrollBarButtonBackground* button);
		void _unsetButtonBackground(ScrollBarButtonBackground* button);
		ScrollBarButtonSlider* _getButtonSlider() const;
		void _setButtonSlider(ScrollBarButtonSlider* button);
		void _unsetButtonSlider(ScrollBarButtonSlider* button);
		ScrollBarButtonForward* _getButtonForward() const;
		void _setButtonForward(ScrollBarButtonForward* button);
		void _unsetButtonForward(ScrollBarButtonForward* button);
		ScrollBarButtonBackward* _getButtonBackward() const;
		void _setButtonBackward(ScrollBarButtonBackward* button);
		void _unsetButtonBackward(ScrollBarButtonBackward* button);

		void _initAreaDragging();
		bool _canAddScrollValue() const;

		virtual hstr _getSkinNameBackground() const = 0;
		virtual hstr _getSkinNameSliderNormal() const = 0;
		virtual hstr _getSkinNameSliderHover() const = 0;
		virtual hstr _getSkinNameSliderPushed() const = 0;
		virtual hstr _getSkinNameForwardNormal() const = 0;
		virtual hstr _getSkinNameForwardHover() const = 0;
		virtual hstr _getSkinNameForwardPushed() const = 0;
		virtual hstr _getSkinNameBackwardNormal() const = 0;
		virtual hstr _getSkinNameBackwardHover() const = 0;
		virtual hstr _getSkinNameBackwardPushed() const = 0;

		virtual grect _getBarDrawRect() const = 0;

		/// @note x and y range from 0 to 1
		virtual float _calcScrollJump(float x, float y, gvec2 size) = 0;
		virtual float _calcScrollMove(float x, float y) = 0;
		virtual void _updateChildren() = 0;
		virtual void _moveScrollBar(float x, float y) = 0;
		virtual void _updateBar() = 0;
		virtual void _adjustDragSpeed() = 0;
		virtual bool _checkAreaSize() = 0;

		void _update(float timeDelta);
		void _draw();

		bool _mouseMove();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		gvec2 _clickPosition;
		float _retainTime;
		bool _scrolling;

	};
}

#endif
