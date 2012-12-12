/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a text editbox.

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
		~EditBox();
		static Object* createInstance(chstr name, grect rect);
		
		int getCursorIndex() { return this->mCursorIndex; }
		void setCursorIndex(int value);
		void setCursorIndexAt(float x, float y);
		int getMaxLength() { return this->mMaxLength; }
		void setMaxLength(int value);
		char getPasswordChar() { return this->mPasswordChar; }
		void setPasswordChar(char value) { this->mPasswordChar = value; }
		hstr getFilter() { return this->mFilter; }
		void setFilter(chstr value);
		hstr getEmptyText() { return this->mEmptyText; }
		void setEmptyText(chstr value);
		hstr getEmptyTextKey() { return this->mEmptyTextKey; }
		void setEmptyTextKey(chstr value);
		void setText(chstr value);
		void setFocused(bool value);
		
		void update(float time);

		void notifyEvent(chstr name, void* params);
		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);
		
		bool onMouseDown(int button);
		bool onMouseUp(int button);
		bool onKeyDown(unsigned int keyCode);
		bool onKeyUp(unsigned int keyCode);
		bool onChar(unsigned int charCode);
		void cancelMouseDown();
		
	protected:
		hstr mEmptyText;
		hstr mEmptyTextKey;
		bool mPushed;
		int mCursorIndex;
		int mOffsetIndex;
		int mMaxLength;
		char mPasswordChar;
		bool mCtrlMode;
		bool mSpaceHack; // TODO - remove
		float mBlinkTimer;
		hstr mFilter;
		harray<unsigned int> mUnicodeChars;
		harray<unsigned int> mEmptyUnicodeChars;
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
		void _insertChar(unsigned int charCode);

		harray<unsigned int> _convertToUnicodeChars(chstr string);
		
	};

}

#endif
