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
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	class Event;
	class EventArgs;
	
	class apriluiExport Object
	{
	public:
		Object(chstr type, chstr name);
		virtual ~Object();

		void addChild(Object* object);
		void removeChild(Object* object);
		Object* getParent() { return mParent; }
		void removeAllChildren();
		harray<Object*>& getChildren() { return mChildren; }

		Object* getChildUnderPoint(float x, float y);
		virtual Object* getChildUnderPoint(gvec2 pos);
		
		virtual float getX() { return 0; }
		virtual float getY() { return 0; }
		virtual gvec2 getPosition() { return gvec2(); }
		virtual int getZOrder() { return 0; }
		virtual void setZOrder(int value);
		
		void registerEvent(chstr name, void (*callback)(EventArgs*));

		bool isEnabled() { return mEnabled; }
		void setEnabled(bool value) { mEnabled = value; }
		
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
		virtual void draw(gvec2 offset = gvec2()) { }

		virtual void notifyEvent(chstr name, void* params);
		
		virtual void setProperty(chstr name, chstr value);
		// system call, do not use!
		void _setDataset(Dataset* value) { mDataset = value; }
		
	protected:
		hstr mTypeName;
		hstr mName;
		Object* mParent;
		harray<Object*> mChildren;
		hmap<hstr, Event*> mEvents;
		bool mEnabled;
		Dataset* mDataset;
		
		void _sortChildren();
		
		void triggerEvent(chstr name, float x = 0.0f, float y = 0.0f, chstr extra = "");
		bool isDerivedEnabled();
		
		virtual void OnDraw(gvec2 offset = gvec2()) { }
		void setParent(Object* value) { mParent = value; }

		void _moveChildToFront(Object* object);
		void _moveChildToBack(Object* object);
	};
	
}

#endif
