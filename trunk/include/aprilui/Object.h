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
/// Defines a base object as a node in a tree-like structure for GUI elements and other objects.

#ifndef APRILUI_OBJECT_H
#define APRILUI_OBJECT_H

#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "EventReceiver.h"

namespace aprilui
{
	class Animator;
	class Dataset;
	class Event;
	class EventArgs;
	
	class apriluiExport Object : public EventReceiver
	{
	public:
		friend class Dataset;

		Object(chstr name, grect rect);
		~Object();
		
		void addChild(Object* object);
		void removeChild(Object* object);
		void registerChild(Object* object);
		void unregisterChild(Object* object);
		void attach(Object* object);
		void detach();
		Object* getParent() { return this->mParent; }
		void removeChildren(bool recursive = false);
		void destroyChildren();
		harray<Object*>& getChildren() { return this->mChildren; }
		hmap<hstr, Event*>& getEvents() { return this->mEvents; }
		Object* getChildByName(chstr name, bool recursive = false);
		Object* getChildUnderPoint(gvec2 pos);
		Object* getChildUnderPoint(float x, float y);
		Object* getChildUnderCursor();
		void clearChildUnderCursor();
		/// @returns Whether or not a given object is a direct child of this object
		bool isChild(Object* obj);
		/// @returns whether or not a given object is a descendant of this object (child or child of a child etc recursively)
		bool isDescendant(Object* obj);
		/// @returns whether or not a given object is a direct parent of this object
		bool isParent(Object* obj);
		/// @returns whether or not a given object is an ancestor of a this object
		bool isAncestor(Object* obj);
		harray<Object*> getAncestors();
		harray<Object*> getDescendants();
		hstr getName() { return this->mName; }
		hstr getFullName();
		bool isCursorInside();
		virtual bool isPointInside(gvec2 position);
		bool angleEquals(float angle);
		void unregisterEvent(chstr name);
		void registerEvent(chstr name, void (*callback)(EventArgs*));
		void registerEvent(chstr name, Event* e);
		
		float getX() { return this->mRect.x; }
		void setX(float value) { this->mRect.x = value; }
		float getY() { return this->mRect.y; }
		void setY(float value) { this->mRect.y = value; }
		gvec2 getPosition() { return this->mRect.getPosition(); }
		void setPosition(gvec2 value) { this->mRect.setPosition(value); }
		void setPosition(float x, float y) { this->mRect.setPosition(x, y); }
		float getWidth() { return this->mRect.w; }
		void setWidth(float value);
		float getHeight() { return this->mRect.h; }
		void setHeight(float value);
		gvec2 getSize() { return this->mRect.getSize(); }
		void setSize(gvec2 value);
		void setSize(float w, float h);
		grect getRect() { return this->mRect; }
		void setRect(grect value);

		int getZOrder() { return this->mZOrder; }
		void setZOrder(int zorder);
		
		gvec2 getScale() { return this->mScale; }
		void setScale(gvec2 value) { this->mScale = value; }
		void setScale(float x, float y) { this->mScale.set(x, y); }
		float getScaleX() { return this->mScale.x; }
		void setScaleX(float value) { this->mScale.x = value; }
		float getScaleY() { return this->mScale.y; }
		void setScaleY(float value) { this->mScale.y = value; }

		gvec2 getCenter() { return this->mCenter; }
		void setCenter(gvec2 value) { this->mCenter = value; }
		void setCenter(float x, float y) { this->mCenter.set(x, y); }
		float getCenterX() { return this->mCenter.x; }
		void setCenterX(float value) { this->mCenter.x = value; }
		float getCenterY() { return this->mCenter.y; }
		void setCenterY(float value) { this->mCenter.y = value; }
		
		bool isEnabled() { return this->mEnabled; }
		void setEnabled(bool value) { this->mEnabled = value; }
		float getAngle() { return this->mAngle; }
		void setAngle(float value) { this->mAngle = value; }
		bool isAnchorLeft() { return this->mAnchorLeft; }
		void setAnchorLeft(bool value) { this->mAnchorLeft = value; }
		bool isAnchorRight() { return this->mAnchorRight; }
		void setAnchorRight(bool value) { this->mAnchorRight = value; }
		bool isAnchorTop() { return this->mAnchorTop; }
		void setAnchorTop(bool value) { this->mAnchorTop = value; }
		bool isAnchorBottom() { return this->mAnchorBottom; }
		void setAnchorBottom(bool value) { this->mAnchorBottom = value; }
		bool isRetainAnchorAspect() { return this->mRetainAnchorAspect; }
		void setRetainAnchorAspect(bool value) { this->mRetainAnchorAspect = value; }
		bool isClickThrough() { return this->mClickThrough; }
		void setClickThrough(bool value) { this->mClickThrough = value; }
		bool isInheritsAlpha() { return this->mInheritsAlpha; }
		void setInheritsAlpha(bool value) { this->mInheritsAlpha = value; }
		april::Color getColor() { return this->mColor; }
		void setColor(april::Color value) { this->mColor = value; }
		void setColor(chstr value) { this->mColor.set(value); }
		
		unsigned char getRed() { return this->mColor.r; }
		void setRed(unsigned char value) { this->mColor.r = value; }
		unsigned char getGreen() { return this->mColor.g; }
		void setGreen(unsigned char value) { this->mColor.g = value; }
		unsigned char getBlue() { return this->mColor.b; }
		void setBlue(unsigned char value) { this->mColor.b = value; }
		unsigned char getAlpha() { return this->mColor.a; }
		unsigned char getDerivedAlpha(aprilui::Object* overrideRoot = NULL);
		void setAlpha(unsigned char value);
		bool isVisible() { return (this->mVisible && this->mColor.a > 0); }
		void setVisible(bool value) { this->mVisible = value; }
		bool getVisibilityFlag() { return this->mVisible; }
		bool isClip() { return this->mClip; }
		void setClip(bool value) { this->mClip = value; }
		bool isUseDisabledAlpha() { return this->mUseDisabledAlpha; }
		void setUseDisabledAlpha(bool value) { this->mUseDisabledAlpha = value; }
		virtual Dataset* getDataset() { return mDataset; }
		
		harray<gvec2> transformToLocalSpace(harray<gvec2> points, aprilui::Object* overrideRoot = NULL);
		gvec2 transformToLocalSpace(gvec2 point, aprilui::Object* overrideRoot = NULL);

		harray<gvec2> getDerivedPoints(harray<gvec2> points, aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedPoint(gvec2 point, aprilui::Object* overrideRoot = NULL);
		grect getBoundingRect(aprilui::Object* overrideRoot = NULL);
		/// @note The points are ordered as top-left, top-right, bottom-left, bottom-right within the local space.
		harray<gvec2> getDerivedCorners(aprilui::Object* overrideRoot = NULL);

		gvec2 getDerivedPosition(aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedSize(aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedCenter(aprilui::Object* overrideRoot = NULL);
		gvec2 getDerivedScale(aprilui::Object* overrideRoot = NULL);
		bool isDerivedEnabled();
		bool isDerivedVisible();
		virtual bool isAnimated();
		virtual bool isWaitingAnimation();
		bool hasDynamicAnimation();
		
		// if a childs event returns true, event is not propagated to parents
		virtual bool onMouseDown(int button);
		virtual bool onMouseUp(int button);
		virtual bool onMouseMove();
		virtual bool onMouseScroll(float x, float y);
		virtual bool onKeyDown(unsigned int keyCode);
		virtual bool onKeyUp(unsigned int keyCode);
		virtual bool onChar(unsigned int charCode);
		virtual void cancelMouseDown();

		virtual void update(float k);
		void draw();
		
		Object* findChildByName(chstr name);
		
		void resetCenter();
		
		virtual hstr getProperty(chstr name, bool* propertyExists = NULL);
		virtual bool setProperty(chstr name, chstr value);
		
		// dynamic animators
		
		Animator* moveX(float x, float speed);
		Animator* moveY(float y, float speed);
		Animator* scaleX(float x, float speed);
		Animator* scaleY(float y, float speed);
		Animator* resizeX(float x, float speed);
		Animator* resizeY(float y, float speed);
		Animator* rotate(float angle, float speed);
		Animator* moveCenterX(float x, float speed);
		Animator* moveCenterY(float y, float speed);
		Animator* fadeRed(unsigned char r, float speed);
		Animator* fadeGreen(unsigned char g, float speed);
		Animator* fadeBlue(unsigned char b, float speed);
		Animator* fadeAlpha(unsigned char a, float speed);
		void move(float x, float y, float speed);
		void move(gvec2 position, float speed);
		void scale(float x, float y, float speed);
		void scale(gvec2 scale, float speed);
		void resize(float x, float y, float speed);
		void resize(gvec2 size, float speed);
		void moveCenter(float x, float y, float speed);
		void moveCenter(gvec2 center, float speed);
		void fadeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeColor(april::Color color, float speed);
		
		Animator* moveXQueue(float x, float speed, float delay = 0.0f);
		Animator* moveYQueue(float y, float speed, float delay = 0.0f);
		Animator* moveCenterXQueue(float x, float speed, float delay = 0.0f);
		Animator* moveCenterYQueue(float y, float speed, float delay = 0.0f);
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
		void moveQueue(gvec2 position, float speed, float delay = 0.0f);
		void scaleQueue(float x, float y, float speed, float delay = 0.0f);
		void scaleQueue(gvec2 scale, float speed, float delay = 0.0f);
		void resizeQueue(float x, float y, float speed, float delay = 0.0f);
		void resizeQueue(gvec2 size, float speed, float delay = 0.0f);
		void moveCenterQueue(float x, float y, float speed, float delay = 0.0f);
		void moveCenterQueue(gvec2 center, float speed, float delay = 0.0f);
		void fadeColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeColorQueue(april::Color color, float speed, float delay = 0.0f);
		
		void moveXStop();
		void moveYStop();
		void scaleXStop();
		void scaleYStop();
		void resizeXStop();
		void resizeYStop();
		void rotateStop();
		void moveCenterXStop();
		void moveCenterYStop();
		void fadeRedStop();
		void fadeGreenStop();
		void fadeBlueStop();
		void fadeAlphaStop();
		void moveStop();
		void scaleStop();
		void resizeStop();
		void moveCenterStop();
		void fadeColorStop();
		
		DEPRECATED_ATTRIBUTE void moveToFront() { if (this->mParent != NULL) { this->mParent->getChildren().remove(this); this->mParent->getChildren().push_back(this); } }
		DEPRECATED_ATTRIBUTE void moveToBack() { if (this->mParent != NULL) { this->mParent->getChildren().remove(this); this->mParent->getChildren().push_front(this); } }
		DEPRECATED_ATTRIBUTE bool isClickthrough() { return this->mClickThrough; }
		DEPRECATED_ATTRIBUTE void setClickthrough(bool value) { this->mClickThrough = value; }
		DEPRECATED_ATTRIBUTE grect getDerivedRect(aprilui::Object* overrideRoot = NULL) { return this->getBoundingRect(overrideRoot); }
		
		// TODO - this needs to be seriously refactored
		bool triggerEvent(chstr name, unsigned int keyCode = 0, chstr extra = "");
		// TODO - this needs to be seriously refactored
		bool triggerEvent(chstr name, float x, float y, unsigned int keyCode = 0, chstr extra = "");

	protected:
		hstr mName;
		grect mRect;
		gvec2 mScale;
		gvec2 mCenter;
		Object* mParent;
		Object* mChildUnderCursor;
		bool mCheckedChildUnderCursor;
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
		bool mRetainAnchorAspect;
		Dataset* mDataset;
		bool mClip;
		bool mUseDisabledAlpha;
		
		void _setName(chstr value) { this->mName = value; }
		void _setDataset(Dataset* value) { this->mDataset = value; }
		void _sortChildren();

		void _updateChildrenHorizontal(float difference);
		void _updateChildrenVertical(float difference);
		
		float _getDerivedAngle(aprilui::Object* overrideRoot = NULL);
		bool _isDerivedClickThrough();
		grect _getDrawRect();
		april::Color _getDrawColor();
		float _getDisabledAlphaFactor();
		void _setParent(Object* value) { this->mParent = value; }

		virtual void OnDraw() { }

	};
	
}

#endif
