/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EDIT_BOX_H
#define APRILUI_EDIT_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class apriluiExport EditBox : public Label
	{
	public:
		EditBox(chstr name, grect rect);
		
		int getCursorIndex() { return mCursorIndex; }
		void setCursorIndex(int cursorIndex);
		void setCursorIndexAt(float x, float y);
		int getMaxLength() { return mMaxLength; }
		void setMaxLength(int maxLength);
		char getPasswordChar() { return mPasswordChar; }
		void setPasswordChar(char value) { mPasswordChar = value; }
		hstr getFilter() { return mFilter; }
		void setFilter(chstr filter);
		void setText(chstr text);
		
		void update(float time);
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		void OnKeyDown(unsigned int keycode);
		void OnKeyUp(unsigned int keycode);
		void OnChar(unsigned int charcode);
		
	protected:
		bool mPushed;
		int mCursorIndex;
		int mOffsetIndex;
		int mMaxLength;
		char mPasswordChar;
		bool mCtrlMode;
        bool mBackground; // TODO - replace with background color and move to LabelBase
		bool mSpaceHack; // TODO - remove
		float mBlinkTimer;
		hstr mFilter;
		harray<unsigned int> mUnicodeChars;
		harray<unsigned int> mFilterChars;
		
		void OnDraw();
		
		void _cursorMoveLeft();
		void _cursorMoveRight();
		void _cursorMoveLeftWord();
		void _cursorMoveRightWord();
		void _deleteLeft(int count = 1);
		void _deleteRight(int count = 1);
		void _deleteLeftWord();
		void _deleteRightWord();
		void _insertChar(unsigned int charcode);

		harray<unsigned int> _convertToUnicodeChars(chstr string);
		
	};

}

#endif
