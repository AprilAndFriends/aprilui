/// @file
/// @author  Boris Mikic
/// @version 2.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
		ScrollBarV(chstr name, grect rect);
		~ScrollBarV();
		static Object* createInstance(chstr name, grect rect);

		void notifyEvent(chstr name, void* params);

		void addScrollValue(float value, bool useAccumulated);

		static hstr SkinNameUpNormal;
		static hstr SkinNameUpHover;
		static hstr SkinNameUpPushed;
		static hstr SkinNameDownNormal;
		static hstr SkinNameDownHover;
		static hstr SkinNameDownPushed;
		static hstr SkinNameBackgroundV;
		static hstr SkinNameBarVNormal;
		static hstr SkinNameBarVHover;
		static hstr SkinNameBarVPushed;

	protected:
		hstr _getSkinNameBeginNormal() { return SkinNameUpNormal; }
		hstr _getSkinNameBeginHover() { return SkinNameUpHover; }
		hstr _getSkinNameBeginPushed() { return SkinNameUpPushed; }
		hstr _getSkinNameEndNormal() { return SkinNameDownNormal; }
		hstr _getSkinNameEndHover() { return SkinNameDownHover; }
		hstr _getSkinNameEndPushed() { return SkinNameDownPushed; }
		hstr _getSkinNameBackground() { return SkinNameBackgroundV; }
		hstr _getSkinNameBarNormal() { return SkinNameBarVNormal; }
		hstr _getSkinNameBarHover() { return SkinNameBarVHover; }
		hstr _getSkinNameBarPushed() { return SkinNameBarVPushed; }

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
