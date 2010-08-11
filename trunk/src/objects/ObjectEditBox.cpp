/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/Keys.h>
#include <atres/Atres.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Dataset.h"
#include "ObjectEditBox.h"

namespace AprilUI
{
	EditBox::EditBox(chstr name,float x,float y,float w,float h) :
		Label(name,x,y,w,h)
	{
		mHorzFormatting=Atres::CENTER;
		mVertFormatting=VERT_CENTER;
		mFontEffect=Atres::NONE;
		mText="EditBox: "+name;
		mTypeName="EditBox";
		mPushed=0;
		mMaxLength=0;
		mPasswordChar=0;
		mCursorIndex=0;
		mCtrlMode=0;
		mFilter="";
	}

	void EditBox::OnDraw(float offset_x,float offset_y)
	{
		rendersys->drawColoredQuad(mX+offset_x, mY+offset_y, mWidth, mHeight, 0, 0, 0, 0.7f+0.3f*mPushed);
		hstr text=mText;
		if (mPasswordChar && mText != "")
		{
			mText=hstr(mPasswordChar,mText.size());
		}
		Label::OnDraw(offset_x,offset_y);
		//2DO - add cursor drawing
		//rendersys->drawColoredQuad(x, y, 1, 0.8*font_h, 1, 1, 1, 1);
		mText=text;
	}
	
	void EditBox::setProperty(chstr name,chstr value)
	{
		Label::setProperty(name,value);
		if      (name == "max_length")    setMaxLength(value);
		else if (name == "password_char") setPasswordChar(value.c_str()[0]);
		else if (name == "filter")        setFilter(value);
	}
	
	void EditBox::setCursorIndex(int cursorIndex)
	{
		mCursorIndex=hclamp(cursorIndex,0,mText.size());
	}
	
	void EditBox::setMaxLength(int maxLength)
	{
		mMaxLength=maxLength;
		if (mMaxLength > 0 && mText.size() > mMaxLength)
		{
			mText=mText(0,mMaxLength);
			setCursorIndex(mCursorIndex);
		}
	}
	
	void EditBox::setFilter(chstr filter)
	{
		mFilter=filter;
		setText(mText);
	}
	
	void EditBox::setText(chstr text)
	{
		Label::setText(text);
		harray<char> chars=harray<char>(mText.c_str(),mText.size());
		foreach (char,it,chars)
		{
			if (!mFilter.contains(*it))
			{
				mText.replace(*it,"");
			}
		}
	}

	bool EditBox::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			mPushed=true;
			return true;
		}
		return false;
	}

	bool EditBox::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (mDataPtr) mDataPtr->setFocusedObject(this);
		if (mPushed && isPointInside(x,y))
		{
			mPushed=false;
			triggerEvent("Click",x,y,0);
			return true;
		}
		mPushed=false;
		return false;
	}
	
	void EditBox::OnKeyDown(unsigned int keycode)
	{
		switch (keycode)
		{
		case AK_LEFT:
			mCtrlMode ? _cursorMoveLeftWord() : setCursorIndex(mCursorIndex-1);
			break;
		case AK_RIGHT:
			mCtrlMode ? _cursorMoveRightWord() : setCursorIndex(mCursorIndex+1);
			break;
		case AK_BACK:
			mCtrlMode ? _deleteLeftWord() : _deleteLeft();
			break;
		case AK_DELETE:
			mCtrlMode ? _deleteRightWord() : _deleteRight();
			break;
		case AK_HOME:
			setCursorIndex(0);
			break;
		case AK_END:
			setCursorIndex(mText.size());
			break;
		case AK_CONTROL:
			mCtrlMode=1;
			break;
		}
	}

	void EditBox::OnKeyUp(unsigned int keycode)
	{
		switch (keycode)
		{
		case AK_CONTROL:
			mCtrlMode=0;
			break;
		}
	}

	void EditBox::OnChar(unsigned int charcode)
	{
		char c=(char)charcode;
		//2DO - add: if charcode exists in Font Manager
		if (mFilter == "" || mFilter.contains(c))
		{
			_insertText(c);
		}
	}
	
	void EditBox::_cursorMoveLeftWord()
	{
		while (mCursorIndex > 0 && mText[mCursorIndex-1] == ' ') mCursorIndex--;
		while (mCursorIndex > 0 && mText[mCursorIndex-1] != ' ') mCursorIndex--;
	}
	
	void EditBox::_cursorMoveRightWord()
	{
		while (mCursorIndex <= mText.size() && mText[mCursorIndex] != ' ') mCursorIndex++;
		while (mCursorIndex <= mText.size() && mText[mCursorIndex] == ' ') mCursorIndex++;
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (mCursorIndex==0)
		{
			return;
		}
		count=hmin(count,mCursorIndex);
		hstr left=(mCursorIndex > count ? mText(0,mCursorIndex-count) : "");
		hstr right=(mCursorIndex < mText.size() ? mText(mCursorIndex,mText.size()-mCursorIndex) : "");
		mCursorIndex-=count;
		mText=left+right;
	}
	
	void EditBox::_deleteRight(int count)
	{
		count=hmin(count,mText.size()-mCursorIndex);
		mCursorIndex+=count;
		_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index=mCursorIndex;
		while (index > 0 && mText[index-1] == ' ') index--;
		while (index > 0 && mText[index-1] != ' ') index--;
		if (mCursorIndex > index)
		{
			_deleteLeft(mCursorIndex-index);
		}
	}
	
	void EditBox::_deleteRightWord()
	{
		int index=mCursorIndex;
		while (index <= mText.size() && mText[index] != ' ') index++;
		while (index <= mText.size() && mText[index] == ' ') index++;
		if (index > mCursorIndex)
		{
			_deleteLeft(index-mCursorIndex);
		}
	}
	
	void EditBox::_insertText(chstr text)
	{
		hstr new_text=text;
		if (mMaxLength > 0)
		{
			if (mText.size() >= mMaxLength)
			{
				return;
			}
			if (mText.size() + new_text.size() > mMaxLength)
			{
				new_text=new_text(0,mMaxLength-mText.size());
			}
		}
		hstr left=(mCursorIndex > 0 ? mText(0,mCursorIndex) : "");
		hstr right=(mCursorIndex < mText.size() ? mText(mCursorIndex,mText.size()-mCursorIndex) : "");
		mCursorIndex+=new_text.size();
		mText=left+new_text+right;
	}
	

}
