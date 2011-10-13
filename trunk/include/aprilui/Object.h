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

#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Animator;
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
		void registerChild(Object* object);
		void unregisterChild(Object* object);
		void attach(Object* object);
		void detach();
		Object* getParent() { return mParent; }
		void removeAllChildren(bool recursive = false);
        void destroyAllChildren(bool recursive = false);
		harray<Object*>& getChildren() { return mChildren; }
		Object* getChildUnderPoint(gvec2 pos);
		Object* getChildUnderPoint(float x, float y);
		Object* getChildUnderCursor();
		
		int getZOrder() { return mZOrder; }
		void setZOrder(int zorder);
		
		bool isCursorInside();
		virtual bool isPointInside(gvec2 position);
		bool angleEquals(float angle);
		void unregisterEvent(chstr name);
		void registerEvent(chstr name, void (*callback)(EventArgs*));
		void registerEvent(chstr name, Event* e);
		
		float getX() { return mRect.x; }
		void setX(float value) { mRect.x = value; }
		float getY() { return mRect.y; }
		void setY(float value) { mRect.y = value; }
		gvec2 getPosition() { return mRect.getPosition(); }
		void setPosition(gvec2 value) { mRect.setPosition(value); }
		void setPosition(float x, float y) { mRect.x = x; mRect.y = y; }
		float getWidth() { return mRect.w; }
		void setWidth(float value);
		float getHeight() { return mRect.h; }
		void setHeight(float value);
		gvec2 getSize() { return mRect.getSize(); }
		void setSize(gvec2 value);
		void setSize(float w, float h);
		grect getRect() { return mRect; }
		void setRect(grect value);
		gvec2 getScale() { return mScale; }
		void setScale(gvec2 value) { mScale = value; }
		void setScale(float x, float y) { mScale.x = x; mScale.y = y; }
		float getScaleX() { return mScale.x; }
		void setScaleX(float value) { mScale.x = value; }
		float getScaleY() { return mScale.y; }
		void setScaleY(float value) { mScale.y = value; }
		gvec2 getCenter() { return mCenter; }
		void setCenter(gvec2 value) { mCenter = value; }
		void setCenter(float x, float y) { mCenter.x = x; mCenter.y = y; }
		float getCenterX() { return mCenter.x; }
		void setCenterX(float value) { mCenter.x = value; }
		float getCenterY() { return mCenter.y; }
		void setCenterY(float value) { mCenter.y = value; }
		
		float getAngle() { return mAngle; }
		void setAngle(float value) { mAngle = value; }
		bool isEnabled() { return mEnabled; }
		void setEnabled(bool value) { mEnabled = value; }
		bool isAnchorLeft() { return mAnchorLeft; }
		void setAnchorLeft(bool value) { mAnchorLeft = value; }
		bool isAnchorRight() { return mAnchorRight; }
		void setAnchorRight(bool value) { mAnchorRight = value; }
		bool isAnchorTop() { return mAnchorTop; }
		void setAnchorTop(bool value) { mAnchorTop = value; }
		bool isAnchorBottom() { return mAnchorBottom; }
		void setAnchorBottom(bool value) { mAnchorBottom = value; }
		bool isClickThrough() { return mClickThrough; }
		void setClickThrough(bool value) { mClickThrough = value; }
		bool isInheritsAlpha() { return mInheritsAlpha; }
		void setInheritsAlpha(bool value) { mInheritsAlpha = value; }
		april::Color getColor() { return mColor; }
		void setColor(april::Color value) { mColor = value; }
		void setColor(chstr value) { mColor.set(value); }
		
		unsigned char getRed() { return mColor.r; }
		void setRed(unsigned char value) { mColor.r = value; }
		unsigned char getGreen() { return mColor.g; }
		void setGreen(unsigned char value) { mColor.g = value; }
		unsigned char getBlue() { return mColor.b; }
		void setBlue(unsigned char value) { mColor.b = value; }
		unsigned char getAlpha() { return mColor.a; }
		unsigned char getDerivedAlpha();
		void setAlpha(unsigned char value);
		bool isVisible() { return (mVisible && mColor.a > 0); }
		void setVisible(bool value) { mVisible = value; }
		bool getVisibilityFlag() { return mVisible; }
		grect getDerivedRect();
		gvec2 getDerivedPosition();
		gvec2 getDerivedSize();
		gvec2 getDerivedCenter();
		gvec2 getDerivedScale();
		
		virtual bool isAnimated();
		virtual bool isWaitingAnimation();
		bool hasDynamicAnimation();
		
		void _setTypeName(chstr type) { mTypeName = type; }
		hstr getType() { return mTypeName; }
		hstr getName() { return mName; }
		
		// if a childs event returns true, event is not propagated to parents
		virtual bool onMouseDown(float x, float y, int button);
		virtual bool onMouseUp(float x, float y, int button);
		virtual void onMouseMove(float x, float y);
		virtual void onKeyDown(unsigned int keycode);
		virtual void onKeyUp(unsigned int keycode);
		virtual void onChar(unsigned int charcode);

		virtual void update(float k);
		void draw(gvec2 offset = gvec2());
		
		Object* findChildByName(chstr name);
		
		virtual void notifyEvent(chstr name, void* params);

		void resetCenter();
		
		virtual hstr getProperty(chstr name, bool* property_exists = NULL);
		virtual bool setProperty(chstr name, chstr value);
		Dataset* getDataset() { return mDataset; }
		void _setDataset(Dataset* value) { mDataset = value; }
		
		// dynamic animators
		
		Animator* moveX(float x, float speed);
		Animator* moveY(float y, float speed);
		Animator* scaleX(float x, float speed);
		Animator* scaleY(float y, float speed);
		Animator* resizeX(float x, float speed);
		Animator* resizeY(float y, float speed);
		Animator* rotate(float angle, float speed);
		Animator* fadeRed(unsigned char r, float speed);
		Animator* fadeGreen(unsigned char g, float speed);
		Animator* fadeBlue(unsigned char b, float speed);
		Animator* fadeAlpha(unsigned char a, float speed);
		void move(float x, float y, float speed);
		void scale(float x, float y, float speed);
		void resize(float x, float y, float speed);
		void fadeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		
		Animator* moveXQueue(float x, float speed, float delay = 0.0f);
		Animator* moveYQueue(float y, float speed, float delay = 0.0f);
		Animator* scaleXQueue(float x, float speed, float delay = 0.0f);
		Animator* scaleYQueue(float y, float speed, float delay = 0.0f);
		Animator* resizeXQueue(float x, float speed, float delay = 0.0f);
		Animator* resizeYQueue(float y, float speed, float delay = 0.0f);
		Animator* rotateQueue(float angle, float speed, float delay = 0.0f);
		Animator* fadeRedQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeGreenQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeBlueQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeAlphaQueue(unsigned char a, float speed, float delay = 0.0f);
		void moveQueue(float x, float y, float speed, float delay = 0.0f);
		void scaleQueue(float x, float y, float speed, float delay = 0.0f);
		void resizeQueue(float x, float y, float speed, float delay = 0.0f);
		void fadeColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		
		void moveXStop();
		void moveYStop();
		void scaleXStop();
		void scaleYStop();
		void resizeXStop();
		void resizeYStop();
		void rotateStop();
		void fadeRedStop();
		void fadeGreenStop();
		void fadeBlueStop();
		void fadeAlphaStop();
		void moveStop();
		void scaleStop();
		void resizeStop();
		void fadeColorStop();
		
		DEPRECATED_ATTRIBUTE void moveToFront() { if (mParent != NULL) { mParent->getChildren().remove(this); mParent->getChildren().push_back(this); } }
		DEPRECATED_ATTRIBUTE void moveToBack() { if (mParent != NULL) { mParent->getChildren().remove(this); mParent->getChildren().push_front(this); } }
		DEPRECATED_ATTRIBUTE bool isClickthrough() { return mClickThrough; }
		DEPRECATED_ATTRIBUTE void setClickthrough(bool value) { mClickThrough = value; }
		DEPRECATED_ATTRIBUTE bool OnMouseDown(float x, float y, int button) { return onMouseDown(x, y, button); }
		DEPRECATED_ATTRIBUTE bool OnMouseUp(float x, float y, int button) { return onMouseUp(x, y, button); }
		DEPRECATED_ATTRIBUTE void OnMouseMove(float x, float y) { onMouseMove(x, y); }
		DEPRECATED_ATTRIBUTE void OnKeyDown(unsigned int keycode) { onKeyDown(keycode); }
		DEPRECATED_ATTRIBUTE void OnKeyUp(unsigned int keycode) { onKeyUp(keycode); }
		DEPRECATED_ATTRIBUTE void OnChar(unsigned int charcode) { onChar(charcode); }

	protected:
		hstr mTypeName;
		hstr mName;
		grect mRect;
		gvec2 mScale;
		gvec2 mCenter;
		Object* mParent;
		harray<Object*> mChildren;
		harray<Animator*> mDynamicAnimators;
		hmap<hstr, Event*> mEvents;
		int mZOrder;
		bool mEnabled;
		bool mVisible;
		float mAngle;
		bool mClickThrough;
		bool mInheritsAlpha;
		april::Color mColor;
		bool mAnchorLeft;
		bool mAnchorRight;
		bool mAnchorTop;
		bool mAnchorBottom;
		Dataset* mDataset;
		
		void _sortChildren();

		void _updateChildrenHorizontal(float difference);
		void _updateChildrenVertical(float difference);
		
		void _triggerEvent(chstr name, float x = 0.0f, float y = 0.0f, unsigned int keycode = 0, chstr extra = "");
		float _getDerivedAngle();
		bool _isDerivedEnabled();
		bool _isDerivedClickThrough();
		grect _getDrawRect();
		void _setParent(Object* value) { mParent = value; }

		virtual void OnDraw() { }

	};
	
}

#endif
