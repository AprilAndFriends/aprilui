/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	
	struct PropertyDescription // april trunk compatibility
	{
	
	};
	
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
		void setRect(float x, float y, float w, float h) { mRect.set(x, y, w, h); }

		bool isVisible() { return (mVisible && mAlpha > 0.0f); }
		void setVisible(bool value) { mVisible = value; }
		bool isEnabled() { return mEnabled; }
		void setEnabled(bool value) { mEnabled = value; }
		bool isClickthrough() { return mClickthrough; }
		void setClickthrough(bool value) { mClickthrough = value; }
		bool isInheritsAlpha() { return mInheritsAlpha; }
		void setInheritsAlpha(bool value) { mInheritsAlpha = value; }
		bool getVisibilityFlag() { return mVisible; }
		
		gvec2 getScale() { return mScale; }
		void setScale(gvec2 value) { mScale = value; }
		void setScale(float x, float y) { mScale.set(x, y); }
		bool isUseScale() { return mUseScale; }
		void setUseScale(bool value) { mUseScale = value; }
		void setCenter(gvec2 value) { }
		void setCenter(float x, float y) { }
		inline void setPivot(gvec2 value) { } // aprilui trunk compatibility
		inline void setPivot(float x, float y) { } // aprilui trunk compatibility

		float getAlpha() { return mAlpha; }
		void setAlpha(float alpha);
		void setAnchors(bool a, bool b, bool c, bool d) {}

		harray<Object*> getDescendants();

		void moveToFront();
		void moveToBack();
		
		grect getBoundingRect(aprilui::Object* overrideRoot = NULL);
		grect getDerivedRect(aprilui::Object* overrideRoot = NULL) { return getBoundingRect(overrideRoot); }
		gvec2 getDerivedPosition(aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedSize(aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedScale(aprilui::Object* overrideRoot = NULL);
		hstr getProperty(chstr prop, bool* prop_exists = NULL); // new-april compatibility
		
		void _setTypeName(chstr type) { mTypeName = type; }
		hstr getType() { return mTypeName; }
		hstr getName() { return mName; }
		
		// if a childs event returns true, event is not propagated to parents
		virtual bool OnMouseDown(float x, float y, int button);
		virtual bool OnMouseUp(float x, float y, int button);
		virtual bool OnMouseMove(float x, float y);
		virtual bool OnKeyDown(unsigned int keyCode);
		virtual bool OnKeyUp(unsigned int keyCode);
		virtual bool OnChar(unsigned int charCode);

		virtual bool onMouseDown(int button); // aprilui legacy compatibility
		virtual bool onMouseUp(int button);
		virtual bool onMouseMove();
		virtual bool onKeyDown(unsigned int keyCode);
		virtual bool onKeyUp(unsigned int keyCode);
		virtual bool onChar(unsigned int charCode);
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

		gvec2 transformToLocalSpace(gvec2 point, aprilui::Object* overrideRoot = NULL);
		virtual void OnDraw(gvec2 offset = gvec2()) { }

	protected:
		Object* mParent;
		hstr mTypeName;
		hstr mName;
		grect mRect;
		gvec2 mScale;
		bool mUseScale;
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
		grect _getDrawRect();
		
		float getDerivedAlpha();
		bool isDerivedEnabled();
		bool isDerivedClickThrough();
		
		void setParent(Object* value) { mParent = value; }

		void _moveChildToFront(Object* object);
		void _moveChildToBack(Object* object);
	};
	
}

#endif
