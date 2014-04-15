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
		static Object* createInstance(chstr name, grect rect);

		void notifyEvent(chstr name, void* params);

		void addScrollValue(float value);

		static hstr SkinNameLeftNormal;
		static hstr SkinNameLeftHover;
		static hstr SkinNameLeftPushed;
		static hstr SkinNameRightNormal;
		static hstr SkinNameRightHover;
		static hstr SkinNameRightPushed;
		static hstr SkinNameBackgroundH;
		static hstr SkinNameBarHNormal;
		static hstr SkinNameBarHHover;
		static hstr SkinNameBarHPushed;

	protected:
		inline hstr _getSkinNameBeginNormal() { return SkinNameLeftNormal; }
		inline hstr _getSkinNameBeginHover() { return SkinNameLeftHover; }
		inline hstr _getSkinNameBeginPushed() { return SkinNameLeftPushed; }
		inline hstr _getSkinNameEndNormal() { return SkinNameRightNormal; }
		inline hstr _getSkinNameEndHover() { return SkinNameRightHover; }
		inline hstr _getSkinNameEndPushed() { return SkinNameRightPushed; }
		inline hstr _getSkinNameBackground() { return SkinNameBackgroundH; }
		inline hstr _getSkinNameBarNormal() { return SkinNameBarHNormal; }
		inline hstr _getSkinNameBarHover() { return SkinNameBarHHover; }
		inline hstr _getSkinNameBarPushed() { return SkinNameBarHPushed; }

		grect _getBarDrawRect();

		float _calcScrollJump(float x, float y);
		float _calcScrollMove(float x, float y);
		void _updateChildren();
		void _moveScrollBar(float x, float y);
		void _updateBar();
		void _adjustDragSpeed();
		bool _checkAreaSize();

	};
}

#endif
