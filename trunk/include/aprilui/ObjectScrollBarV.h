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

		void addScrollValue(float value);

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
		inline hstr _getSkinNameBeginNormal() { return SkinNameUpNormal; }
		inline hstr _getSkinNameBeginHover() { return SkinNameUpHover; }
		inline hstr _getSkinNameBeginPushed() { return SkinNameUpPushed; }
		inline hstr _getSkinNameEndNormal() { return SkinNameDownNormal; }
		inline hstr _getSkinNameEndHover() { return SkinNameDownHover; }
		inline hstr _getSkinNameEndPushed() { return SkinNameDownPushed; }
		inline hstr _getSkinNameBackground() { return SkinNameBackgroundV; }
		inline hstr _getSkinNameBarNormal() { return SkinNameBarVNormal; }
		inline hstr _getSkinNameBarHover() { return SkinNameBarVHover; }
		inline hstr _getSkinNameBarPushed() { return SkinNameBarVPushed; }

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
