/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.8
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
		bool isMultiLine() { return this->mMultiLine; }
		void setMultiLine(bool value) { this->mMultiLine = value; }
		hstr getFilter() { return this->mFilter; }
		void setFilter(chstr value);
		hstr getEmptyText() { return this->mEmptyText; }
		void setEmptyText(chstr value);
		hstr getEmptyTextKey() { return this->mEmptyTextKey; }
		void setEmptyTextKey(chstr value);
		gvec2 getCaretPosition() { return this->mCaretPosition; }
		void setText(chstr value);
		void setFocused(bool value);
		gvec2 getCaretPosition();
		
		void update(float time);

		void notifyEvent(chstr name, void* params);
		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);
		
		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onKeyDown(april::Key keyCode);
		bool onKeyUp(april::Key keyCode);
		bool onChar(unsigned int charCode);
		void mouseCancel();
		
	protected:
		gvec2 mCaretPosition;
		hstr mEmptyText;
		hstr mEmptyTextKey;
		bool mPushed;
		int mCursorIndex;
		int mMaxLength;
		char mPasswordChar;
		bool mMultiLine;
		bool mCtrlMode;
		bool mSpaceHack; // TODO - remove
		float mBlinkTimer;
		hstr mFilter;
		
		void OnDraw();
		
		void _cursorMoveLeft();
		void _cursorMoveRight();
		void _cursorMoveUp();
		void _cursorMoveDown();
		void _cursorMoveLeftWord();
		void _cursorMoveRightWord();
		void _deleteLeft(int count = 1);
		void _deleteRight(int count = 1);
		void _deleteLeftWord();
		void _deleteRightWord();
		void _insertChar(unsigned int charCode);
		gvec2 _makeCaretPosition(chstr text, chstr originalText);

	};

}

#endif
