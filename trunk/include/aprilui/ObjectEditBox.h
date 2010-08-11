/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EDIT_BOX_H
#define APRILUI_EDIT_BOX_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectLabel.h"

namespace AprilUI
{
	class AprilUIExport EditBox : public Label
	{
	protected:
		bool mPushed;
		int mCursorIndex;
		int mMaxLength;
		char mPasswordChar;
		bool mCtrlMode;
		hstr mFilter;
		
		void OnDraw(float offset_x,float offset_y);
		
		void _cursorMoveLeftWord();
		void _cursorMoveRightWord();
		void _deleteLeft(int count=1);
		void _deleteRight(int count=1);
		void _deleteLeftWord();
		void _deleteRightWord();
		void _insertText(chstr text);
	public:
		EditBox(chstr name,float x,float y,float w,float h);
		
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void OnKeyDown(unsigned int keycode);
		void OnKeyUp(unsigned int keycode);
		void OnChar(unsigned int charcode);
		void setProperty(chstr name,chstr value);
		
		int getCursorIndex() { return mCursorIndex; }
		void setCursorIndex(int cursorIndex);
		int getMaxLength() { return mMaxLength; }
		void setMaxLength(int maxLength);
		char getPasswordChar() { return mPasswordChar; }
		void setPasswordChar(char passwordChar) { mPasswordChar=passwordChar; }
		hstr getFilter() { return mFilter; }
		void setFilter(chstr filter);
		void setText(chstr text);
		
	};

}

#endif
