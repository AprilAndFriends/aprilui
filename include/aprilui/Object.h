/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef OBJECT_H
#define OBJECT_H

#include <map>

#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	class Dataset;
	class Event;
	class EventArgs;
	
	enum VertFormatting
	{
		VERT_TOP,VERT_CENTER,VERT_BOTTOM
	};
	
	class AprilUIExport Object
	{
	protected:
		Object* mParent;
		hstr mTypeName,mName;
		int mZOrder;
		float mX,mY,mWidth,mHeight;
		harray<Object*> mChildren;
		
		std::map<hstr,Event*> mEvents;
		bool mVisible,mEnabled,mClickthrough;
		float mAlpha;
		
		Dataset* mDataPtr;
		
		void sortChildren();
		
		void triggerEvent(chstr name,float x=0,float y=0,char* extra=0);
		float getDerivedAlpha();
		bool getDerivedEnabled();
		bool getDerivedClickthrough();

		
		
		virtual void OnDraw(float offset_x,float offset_y) { };
		virtual void OnUpdate(float k) { };
		void setParent(Object* parent) { mParent=parent; }

		void _moveChildToFront(Object* o);
		void _moveChildToBack(Object* o);
	public:

		Object(chstr type_name,chstr name,float x,float y,float w,float h);
		virtual ~Object();

		void addChild(Object* o);
		void removeChild(Object* o);
		Object* getParent() { return mParent; }
		void removeAllChildren();
		harray<Object*>& getChildren() { return mChildren; }
		Object* getChildUnderPoint(float x,float y);

		int getZOrder() { return mZOrder; }
		void setZOrder(int zorder);
		
		bool isCursorInside();
		virtual bool isPointInside(float x,float y);
		void registerEvent(chstr event_name,void (*callback)(EventArgs*));

		gtypes::Vector2 getPosition() { return gtypes::Vector2(mX,mY); };
		float getXPosition() { return mX; }
		float getYPosition() { return mY; }
		gtypes::Vector2 getSize() { return gtypes::Vector2(mWidth,mHeight); }
		float getWidth() { return mWidth; }
		float getHeight() { return mHeight; }
		void setPosition(float x,float y) { mX=x; mY=y; }
		void setPosition(gtypes::Vector2 v) { mX=v.x; mY=v.y; }
		void setXPosition(float x) { mX=x; }
		void setYPosition(float y) { mY=y; }

		void setSize(float w,float h) { mWidth=w; mHeight=h; }

		void setVisible(bool visible) { mVisible=visible; }
		bool isVisible() { return mVisible && mAlpha > 0; }
		void setEnabled(bool enabled) { mEnabled=enabled; }
		bool isEnabled() { return mEnabled; }
		void setClickthrough(bool clickthrough) { mClickthrough=clickthrough; }
		bool getClickthrough() { return mClickthrough; }
		bool getVisibilityFlag() { return mVisible; }
		
		void setAlpha(float alpha) { mAlpha=alpha; }
		float getAlpha() { return mAlpha; }

		void moveToFront();
		void moveToBack();
		
		void _setTypeName(chstr type) { mTypeName=type; }
		hstr getType() { return mTypeName; }
		hstr getName() { return mName; }
		
		// if a childs event returns true, event is not propagated to parents
		virtual bool OnMouseDown(int button,float x,float y);
		virtual bool OnMouseUp(int button,float x,float y);
		virtual void OnMouseMove(float x,float y);
		
		
		virtual void update(float k);
		void draw(float offset_x=0,float offset_y=0);
		

		virtual void notifyEvent(chstr event_name,void* params);
		
		virtual void setProperty(chstr name,chstr value);
		// system call, do not use!
		void _setDataset(Dataset* d) { mDataPtr=d; }
	};
	
}

#endif
