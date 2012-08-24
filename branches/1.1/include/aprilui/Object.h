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

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "EventReceiver.h"

namespace aprilui
{
	class Dataset;
	class Event;
	class EventArgs;
	
	class apriluiExport Object : public EventReceiver
	{
	public:
		Object(chstr name, grect rect);
		Object(chstr type, chstr name, grect rect);
		~Object();

		void addChild(Object* object);
		void removeChild(Object* object);
		Object* getParent() { return mParent; }
		void removeAllChildren();
		harray<Object*>& getChildren() { return mChildren; }
		Object* getChildUnderPoint(gvec2 pos);
		Object* getChildUnderPoint(float x, float y);

		int getZOrder() { return mZOrder; }
		void setZOrder(int zorder);
		virtual void reset() { } // hack
		
		bool isCursorInside();
		virtual bool isPointInside(gvec2 position);
		virtual bool isPointInside(float x, float y);
		void registerEvent(chstr name, void (*callback)(EventArgs*));

		float getX() { return mRect.x; }
		void setX(float value) { mRect.x = value; }
		float getY() { return mRect.y; }
		void setY(float value) { mRect.y = value; }
		gvec2 getPosition() { return gvec2(mRect.x, mRect.y); }
		void setPosition(gvec2 value) { mRect.x = value.x; mRect.y = value.y; }
		void setPosition(float x, float y) { mRect.x = x; mRect.y = y; }
		float getWidth() { return mRect.w; }
		void setWidth(float value) { mRect.w = value; }
		float getHeight() { return mRect.h; }
		void setHeight(float value) { mRect.h = value; }
		gvec2 getSize() { return gvec2(mRect.w, mRect.h); }
		void setSize(gvec2 value) { mRect.w = value.x; mRect.h = value.y; }
		void setSize(float w, float h) { mRect.w = w; mRect.h = h; }
		grect getRect() { return mRect; }
		void setRect(grect value) { mRect = value; }

		bool isVisible() { return (mVisible && mAlpha > 0.0f); }
		void setVisible(bool value) { mVisible = value; }
		bool isEnabled() { return mEnabled; }
		void setEnabled(bool value) { mEnabled = value; }
		bool isClickthrough() { return mClickthrough; }
		void setClickthrough(bool value) { mClickthrough = value; }
		bool isInheritsAlpha() { return mInheritsAlpha; }
		void setInheritsAlpha(bool value) { mInheritsAlpha = value; }
		bool getVisibilityFlag() { return mVisible; }
		
		// TODO
		gvec2 getScale() { return mScale; }
		void setScale(gvec2 scale);
		void setScale(float x, float y);
		void setCenter(gvec2 scale) {}
		void setCenter(float x, float y) {}
		
		float getAlpha() { return mAlpha; }
		void setAlpha(float alpha);

		void moveToFront();
		void moveToBack();
		
		grect getBoundingRect(aprilui::Object* overrideRoot = NULL);
		grect getDerivedRect(aprilui::Object* overrideRoot = NULL) { return getBoundingRect(overrideRoot); }
		gvec2 getDerivedPosition(aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedSize(aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedScale(aprilui::Object* overrideRoot = NULL);
		
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

		bool onMouseDown(int button); // aprilui legacy compatibility
		bool onMouseUp(int button);
		void onMouseMove();
		void onKeyDown(unsigned int keycode);
		void onKeyUp(unsigned int keycode);
		void onChar(unsigned int charcode);
		void registerChild(Object* object);
		
		void unregisterChild(Object* object);

		void notifyEvent(chstr name, void* params);
		
		virtual void update(float k);
		void draw(gvec2 offset = gvec2());

		virtual bool setProperty(chstr name, chstr value);
		// system call, do not use!
		void _setDataset(Dataset* value) { mDataset = value; }
		virtual Dataset* getDataset() { return mDataset; }
		void triggerEvent(chstr name, float x = 0.0f, float y = 0.0f, chstr extra = "");

	protected:
		Object* mParent;
		hstr mTypeName;
		hstr mName;
		grect mRect;
		gvec2 mScale;
		int mZOrder;
		float mAlpha;
		bool mVisible;
		bool mEnabled;
		bool mClickthrough;
		bool mInheritsAlpha;
		harray<Object*> mChildren;
		hmap<hstr, Event*> mEvents;
		Dataset* mDataset;
		
		void sortChildren();
		
		float getDerivedAlpha();
		bool isDerivedEnabled();
		bool isDerivedClickThrough();
		
		virtual void OnDraw(gvec2 offset = gvec2()) { }
		void setParent(Object* value) { mParent = value; }

		void _moveChildToFront(Object* object);
		void _moveChildToBack(Object* object);
	};
	
}

#endif
