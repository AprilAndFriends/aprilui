/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <map>

#include <april/RenderSystem.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>
#include "Animator.h"
#include "CallbackEvent.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Exception.h"
#include "ObjectCallbackObject.h"
#include "AprilUI.h"

namespace AprilUI
{
	Object::Object(chstr type_name,chstr name,float x,float y,float w,float h)
	{
		mTypeName=type_name;
		mName=name;
		mParent=0;
		mZOrder=0;
		mX=x; mY=y; mWidth=w; mHeight=h;
		mVisible=mEnabled=1;
		mClickthrough=0;
		mInheritsAlpha=1;
		mAlpha=1.0f;
		mDataPtr=0;
	}

	Object::~Object()
	{
		foreach_m(Event*,it,mEvents)
		{
			delete it->second;
		}
	}

	bool _objectSortCallback(Object* lhs, Object* rhs)
	{
		return lhs->getZOrder() < rhs->getZOrder();
	}

	void Object::sortChildren()
	{
		mChildren.sort(_objectSortCallback);
	}

	void Object::addChild(Object* o)
	{
		if (o->getParent()) throw ObjectHasParentException(o->getName(),getName());
		mChildren.push_back(o);
		sortChildren();
		o->setParent(this);
	}
	
	void Object::removeChild(Object* o)
	{
		if (o->getParent() != this) throw ObjectNotChildException(o->getName(),getName());
		mChildren.remove(o);
		o->setParent(NULL);
	}

	void Object::removeAllChildren()
	{
		foreach(Object*,it,mChildren)
			(*it)->setParent(0);
		mChildren.clear();
	}

	void Object::setZOrder(int zorder)
	{
		mZOrder=zorder;
		if (mParent)
			mParent->sortChildren();
	}

	float Object::getDerivedAlpha()
	{
		// recursive function that combines all the alpha from the parents (if any)
		float alpha=this->getAlpha();
		if (mInheritsAlpha && mParent) alpha*=mParent->getDerivedAlpha();
		return alpha;
	}

	void Object::draw(float offset_x,float offset_y)
	{
		if (isVisible())
		{
			OnDraw(offset_x,offset_y);
			foreach (Object*, it, mChildren)
				(*it)->draw(offset_x+mX,offset_y+mY);
		}
	}

	void Object::update(float k)
	{
		foreach (Object*, it, mChildren)
			(*it)->update(k);
	}

	bool Object::isCursorInside()
	{
		gtypes::Vector2 pos=getCursorPos();
		
		for (Object* p=mParent;p != 0;p=p->mParent)
			pos-=p->getPosition();
		return isPointInside(pos.x,pos.y);
	}

	bool Object::isPointInside(float x,float y)
	{
		return (x >= mX && y >= mY && x < mX+mWidth && y < mY+mHeight);
	}

	bool Object::OnMouseDown(int button,float x,float y)
	{
		if (mClickthrough || !isVisible() || !getDerivedEnabled()) return false;
		
		if (mDataPtr) mDataPtr->setFocusedObject(0);
		foreach_r (Object*, it, mChildren)
			if ((*it)->isVisible() && (*it)->getDerivedEnabled() && !(*it)->getClickthrough())
				if ((*it)->OnMouseDown(button,x-mX,y-mY)) return true;
		
		return false;
	}

	bool Object::OnMouseUp(int button,float x,float y)
	{
		if (mClickthrough || !isVisible() || !getDerivedEnabled()) return false;
		
		foreach_r (Object*, it, mChildren)
			if ((*it)->isVisible() && (*it)->getDerivedEnabled() && !(*it)->getClickthrough())
				if ((*it)->OnMouseUp(button,x-mX,y-mY)) return true;
		
		return false;
	}

	void Object::OnMouseMove(float x,float y)
	{
		foreach_r (Object*, it, mChildren)
			if ((*it)->isVisible() && (*it)->getDerivedEnabled())
				(*it)->OnMouseMove(x-mX,y-mY);
	}

	void Object::OnKeyDown(unsigned int keycode)
	{
	}

	void Object::OnKeyUp(unsigned int keycode)
	{
	}

	void Object::OnChar(unsigned int charcode)
	{
	}

	void Object::registerEvent(chstr event_name,void (*callback)(EventArgs*))
	{
		mEvents[event_name]=new CallbackEvent(callback);
	}

	void Object::triggerEvent(chstr name,float x,float y,char* extra)
	{
		Event* e=mEvents[name];
		if (e)
		{
			EventArgs args(this,x,y,extra);
			e->execute(&args);
		}
	}

	void Object::notifyEvent(chstr event_name,void* params)
	{
		foreach(Object*,it,mChildren) (*it)->notifyEvent(event_name,params);
	}

	bool Object::getDerivedEnabled()
	{
		return (isEnabled() && (!mParent || mParent->getDerivedEnabled()));
	}
	bool Object::getDerivedClickthrough()
	{
		if (!mClickthrough) return 0;
		else if (!mParent) return 1;
		else return mParent->getDerivedClickthrough();
	}
	
	void Object::setAlpha(float alpha)
	{
		mAlpha=hclamp(alpha, 0.0f, 1.0f);
	}

	void Object::moveToFront()
	{
		if (mParent) mParent->_moveChildToFront(this);
	}

	void Object::moveToBack()
	{
		if (mParent) mParent->_moveChildToBack(this);
	}

	void Object::_moveChildToFront(Object* o)
	{
		mChildren.remove(o);
		mChildren.push_back(o);
	}

	void Object::_moveChildToBack(Object* o)
	{
		mChildren.remove(o);
		mChildren.push_front(o);
	}

	void Object::setProperty(chstr name,chstr value)
	{
		if      (name == "visible")        setVisible(value);
		else if (name == "zorder")         setZOrder(value);
		else if (name == "enabled")        setEnabled(value);
		else if (name == "clickthrough")   setClickthrough(value);
		else if (name == "inherits_alpha") setInheritsAlpha(value);
		else if (name == "alpha")          setAlpha(value);
	}

	Object* Object::getChildUnderPoint(float x,float y)
	{
		if (!isVisible()) return 0;
		bool inside=isPointInside(x,y);
		if (mChildren.size() == 0)
			return inside ? this : 0;
		else if (!inside) return 0;
		Object* o=0;
		foreach_r (Object*,it,mChildren)
		{
			o=(*it)->getChildUnderPoint(x-mX,y-mY);
			
			if (o && !dynamic_cast<AprilUI::Animator*>(o)) break;
		}
		return (o ? o : this);
	}
	Object* Object::getChildUnderPoint(gtypes::Vector2 pos)
	{
		return getChildUnderPoint(pos.x,pos.y);
	}
}
