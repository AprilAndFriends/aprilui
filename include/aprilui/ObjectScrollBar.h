/// @file
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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

		hstr getSkinName() { return this->mSkinName; }
		void setSkinName(chstr value);
		float getGridSize() { return this->mGridSize; }
		void setGridSize(float value) { this->mGridSize = value; }
		bool isUseFading() { return this->mUseFading; }
		void setUseFading(bool value) { this->mUseFading = value; }
		bool isHeightHide() { return this->mHeightHide; }
		void setHeightHide(bool value) { this->mHeightHide = value; }

		void update(float k);
		void OnDraw();

		void notifyEvent(chstr name, void* params);

		hstr getProperty(chstr name, bool* propertyExists);
		bool setProperty(chstr name, chstr value);

		bool onMouseMove();
		bool onMouseScroll(float x, float y);

		virtual void addScrollValue(float value) = 0;

		static float ScrollDistance;
		static float GridSize;

	protected:
		hstr mSkinName;
		float mGridSize;
		bool mUseFading;
		bool mHeightHide;
		ImageButton* mButtonBegin;
		ImageButton* mButtonEnd;
		ImageButton* mButtonBack;
		ImageButton* mButtonBar;
		float _mRetainTime;

		void _initAreaDragging();

		virtual hstr _getSkinNameBeginNormal() = 0;
		virtual hstr _getSkinNameBeginHover() = 0;
		virtual hstr _getSkinNameBeginPushed() = 0;
		virtual hstr _getSkinNameEndNormal() = 0;
		virtual hstr _getSkinNameEndHover() = 0;
		virtual hstr _getSkinNameEndPushed() = 0;
		virtual hstr _getSkinNameBackground() = 0;
		virtual hstr _getSkinNameBarNormal() = 0;
		virtual hstr _getSkinNameBarHover() = 0;
		virtual hstr _getSkinNameBarPushed() = 0;

		virtual grect _getBarDrawRect() = 0;

		virtual float _calcScrollJump(float x, float y) = 0;
		virtual float _calcScrollMove(float x, float y) = 0;
		virtual void _updateChildren() = 0;
		virtual void _moveScrollBar(float x, float y) = 0;
		virtual void _updateBar() = 0;
		virtual void _adjustDragSpeed() = 0;
		virtual bool _checkAreaSize() = 0;

		static void _clickScrollBegin(EventArgs* args);
		static void _clickScrollEnd(EventArgs* args);
		static void _clickScrollBack(EventArgs* args);
		static void _mouseDownScrollBar(EventArgs* args);
		static void _clickScrollBar(EventArgs* args);

	private:
		bool _mScrolling;
		gvec2 _mClickPosition;

	};
}

#endif
