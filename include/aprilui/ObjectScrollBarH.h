/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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

		void addScrollValue(float value, bool useAccumulated = false);

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
		hstr _getSkinNameBeginNormal() { return SkinNameLeftNormal; }
		hstr _getSkinNameBeginHover() { return SkinNameLeftHover; }
		hstr _getSkinNameBeginPushed() { return SkinNameLeftPushed; }
		hstr _getSkinNameEndNormal() { return SkinNameRightNormal; }
		hstr _getSkinNameEndHover() { return SkinNameRightHover; }
		hstr _getSkinNameEndPushed() { return SkinNameRightPushed; }
		hstr _getSkinNameBackground() { return SkinNameBackgroundH; }
		hstr _getSkinNameBarNormal() { return SkinNameBarHNormal; }
		hstr _getSkinNameBarHover() { return SkinNameBarHHover; }
		hstr _getSkinNameBarPushed() { return SkinNameBarHPushed; }

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
