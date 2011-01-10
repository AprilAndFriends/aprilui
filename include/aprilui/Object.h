/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_OBJECT_H
#define APRILUI_OBJECT_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	enum Dock
	{
		TopLeft,
		TopCenter,
		TopRight,
		CenterLeft,
		CenterCenter,
		CenterRight,
		BottomLeft,
		BottomCenter,
		BottomRight
	};
	
	class Dataset;
	class Event;
	class EventArgs;
	
	class apriluiExport Object
	{
	public:
		Object(chstr type, chstr name, grect rect);
		virtual ~Object();

		void addChild(Object* object);
		void removeChild(Object* object);
		Object* getParent() { return mParent; }
		void removeAllChildren();
		harray<Object*>& getChildren() { return mChildren; }
		Object* getChildUnderPoint(gvec2 pos);
		Object* getChildUnderPoint(float x, float y);

		int getZOrder() { return mZOrder; }
		void setZOrder(int zorder);
		
		virtual bool isCursorInside();
		bool angleEquals(float angle);
		void registerEvent(chstr name, void (*callback)(EventArgs*));

		float getX() { return mRect.x; }
		void setX(float value) { mRect.x = value; }
		float getY() { return mRect.y; }
		void setY(float value) { mRect.y = value; }
		gvec2 getPosition() { return mRect.getPosition(); }
		void setPosition(gvec2 value) { mRect.setPosition(value); }
		void setPosition(float x, float y) { mRect.x = x; mRect.y = y; }
		float getWidth() { return mRect.w; }
		void setWidth(float value) { mRect.w = value; }
		float getHeight() { return mRect.h; }
		void setHeight(float value) { mRect.h = value; }
		gvec2 getSize() { return mRect.getSize(); }
		void setSize(gvec2 value) { mRect.setSize(value); }
		void setSize(float w, float h) { mRect.w = w; mRect.h = h; }
		grect getRect() { return mRect; }
		void setRect(grect value) { mRect = value; }
		float getAngle() { return mAngle; }
		virtual void setAngle(float value) { mAngle = value; }

		bool isVisible() { return (mVisible && mColor.a > 0); }
		void setVisible(bool value) { mVisible = value; }
		bool isEnabled() { return mEnabled; }
		void setEnabled(bool value) { mEnabled = value; }
		Dock getDock() { return mDock; }
		void setDock(Dock value) { mDock = value; }
		bool isClickthrough() { return mClickthrough; }
		void setClickthrough(bool value) { mClickthrough = value; }
		bool isInheritsAlpha() { return mInheritsAlpha; }
		void setInheritsAlpha(bool value) { mInheritsAlpha = value; }
		bool getVisibilityFlag() { return mVisible; }
		unsigned char getRed() { return mColor.r; }
		void setRed(unsigned char value) { mColor.r = value; }
		unsigned char getGreen() { return mColor.g; }
		void setGreen(unsigned char value) { mColor.g = value; }
		unsigned char getBlue() { return mColor.b; }
		void setBlue(unsigned char value) { mColor.b = value; }
		unsigned char getAlpha() { return mColor.a; }
		void setAlpha(unsigned char value);
		april::Color getColor() { return mColor; }
		void setColor(april::Color value) { mColor = value; }
		void setColor(chstr value) { mColor.set(value); }
		gvec2 getDerivedPosition();
		
		virtual bool isAnimated();

		void moveToFront();
		void moveToBack();
		
		void _setTypeName(chstr type) { mTypeName = type; }
		hstr getType() { return mTypeName; }
		hstr getName() { return mName; }
		
		// if a childs event returns true, event is not propagated to parents
		virtual bool OnMouseDown(float x, float y, int button);
		virtual bool OnMouseUp(float x, float y, int button);
		virtual void OnMouseMove(float x, float y);
		virtual void OnKeyDown(unsigned int keycode);
		virtual void OnKeyUp(unsigned int keycode);
		virtual void OnChar(unsigned int charcode);
		
		virtual void update(float k);
		void draw(gvec2 offset = gvec2());

		virtual void notifyEvent(chstr name, void* params);
		
		virtual void setProperty(chstr name, chstr value);
		// system call, do not use!
		void _setDataset(Dataset* value) { mDataset = value; }
		
	protected:
		hstr mTypeName;
		hstr mName;
		grect mRect;
		Object* mParent;
		harray<Object*> mChildren;
		hmap<hstr, Event*> mEvents;
		int mZOrder;
		bool mEnabled;
		bool mVisible;
		float mAngle;
		bool mClickthrough;
		bool mInheritsAlpha;
		april::Color mColor;
		Dock mDock;
		Dataset* mDataset;
		
		void sortChildren();
		
		void triggerEvent(chstr name, float x = 0.0f, float y = 0.0f, chstr extra = "");
		unsigned char getDerivedAlpha();
		bool isDerivedEnabled();
		bool isDerivedClickThrough();
		
		virtual void OnDraw(gvec2 offset = gvec2(), gvec2 center = gvec2()) { }
		void setParent(Object* value) { mParent = value; }

		void _moveChildToFront(Object* object);
		void _moveChildToBack(Object* object);
	};
	
}

#endif
