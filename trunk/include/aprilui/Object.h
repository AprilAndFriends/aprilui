/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
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
#include <april/Keys.h>
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
		enum AnimationFunction
		{
			Linear,
			Sine,
			Square,
			Saw,
			Triangle,
			Random,
			Hover,
			Custom
		};
	
		friend class Dataset;

		Object(chstr name, grect rect);
		~Object();

		HL_DEFINE_GET(Object*, parent, Parent);
		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(int, zOrder, ZOrder);
		void setZOrder(int zorder);

		HL_DEFINE_GET(grect, rect, Rect);
		void setRect(grect value);
		float getX() { return this->rect.x; }
		void setX(float value) { this->rect.x = value; }
		float getY() { return this->rect.y; }
		void setY(float value) { this->rect.y = value; }
		gvec2 getPosition() { return this->rect.getPosition(); }
		void setPosition(gvec2 value) { this->rect.setPosition(value); }
		void setPosition(float x, float y) { this->rect.setPosition(x, y); }
		float getWidth() { return this->rect.w; }
		void setWidth(float value);
		float getHeight() { return this->rect.h; }
		void setHeight(float value);
		gvec2 getSize() { return this->rect.getSize(); }
		void setSize(gvec2 value);
		void setSize(float w, float h);

		HL_DEFINE_GET(gvec2, scale, Scale);
		void setScale(float x, float y) { this->scale.set(x, y); }
		float getScaleX() { return this->scale.x; }
		void setScaleX(float value) { this->scale.x = value; }
		float getScaleY() { return this->scale.y; }
		void setScaleY(float value) { this->scale.y = value; }

		HL_DEFINE_GET(gvec2, center, Center);
		void setCenter(float x, float y) { this->center.set(x, y); }
		float getCenterX() { return this->center.x; }
		void setCenterX(float value) { this->center.x = value; }
		float getCenterY() { return this->center.y; }
		void setCenterY(float value) { this->center.y = value; }

		HL_DEFINE_GETSET(april::Color, color, Color);
		unsigned char getRed() { return this->color.r; }
		void setRed(unsigned char value) { this->color.r = value; }
		unsigned char getGreen() { return this->color.g; }
		void setGreen(unsigned char value) { this->color.g = value; }
		unsigned char getBlue() { return this->color.b; }
		void setBlue(unsigned char value) { this->color.b = value; }
		unsigned char getAlpha() { return this->color.a; }
		void setAlpha(unsigned char value);

		HL_DEFINE_IS(enabled, Enabled);
		void setEnabled(bool value);
		HL_DEFINE_GETSET(float, angle, Angle);
		HL_DEFINE_ISSET(anchorLeft, AnchorLeft);
		HL_DEFINE_ISSET(anchorRight, AnchorRight);
		HL_DEFINE_ISSET(anchorTop, AnchorTop);
		HL_DEFINE_ISSET(anchorBottom, AnchorBottom);
		void setAnchors(bool left, bool right, bool top, bool bottom);
		HL_DEFINE_ISSET(retainAnchorAspect, RetainAnchorAspect);
		HL_DEFINE_ISSET(clickThrough, ClickThrough);
		HL_DEFINE_ISSET(inheritAlpha, InheritAlpha);
		bool isVisible() { return (this->visible && this->color.a > 0); }
		HL_DEFINE_SET(bool, visible, Visible);
		bool getVisibilityFlag() { return this->visible; }
		HL_DEFINE_ISSET(clip, Clip);
		HL_DEFINE_ISSET(useDisabledAlpha, UseDisabledAlpha);
		virtual HL_DEFINE_GET(int, focusIndex, FocusIndex);
		HL_DEFINE_SET(int, focusIndex, FocusIndex);
		virtual HL_DEFINE_GET(Dataset*, dataset, Dataset);
		
		harray<Object*>& getChildren() { return this->children; }
		hmap<hstr, Event*>& getEvents() { return this->events; }
		virtual bool isFocused();
		virtual void setFocused(bool focused);
		hstr getFullName();
		bool isCursorInside();
		Object* getChildUnderCursor();
		harray<Object*> getAncestors();
		harray<Object*> getDescendants();

		unsigned char getDerivedAlpha(aprilui::Object* overrideRoot = NULL);

		void addChild(Object* object);
		void removeChild(Object* object);
		void registerChild(Object* object);
		void unregisterChild(Object* object);
		void attach(Object* object);
		void detach();
		void removeChildren(bool recursive = false);
		void destroyChildren();
		Object* getChildUnderPoint(gvec2 pos);
		Object* getChildUnderPoint(float x, float y);
		virtual void clearChildUnderCursor();
		/// @returns Whether or not a given object is a direct child of this object
		bool isChild(Object* obj);
		/// @returns whether or not a given object is a descendant of this object (child or child of a child etc recursively)
		bool isDescendant(Object* obj);
		/// @returns whether or not a given object is a direct parent of this object
		bool isParent(Object* obj);
		/// @returns whether or not a given object is an ancestor of a this object
		bool isAncestor(Object* obj);
		virtual bool isPointInside(gvec2 position);
		bool angleEquals(float angle);
		void unregisterEvent(chstr name);
		void registerEvent(chstr name, void (*callback)(EventArgs*));
		void registerEvent(chstr name, Event* e);
		
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
		virtual bool onMouseDown(april::Key keyCode);
		virtual bool onMouseUp(april::Key keyCode);
		virtual bool onMouseCancel(april::Key keyCode);
		virtual bool onMouseMove();
		virtual bool onMouseScroll(float x, float y);
		virtual bool onKeyDown(april::Key keyCode);
		virtual bool onKeyUp(april::Key keyCode);
		virtual bool onChar(unsigned int charCode);
		virtual bool onTouch(const harray<gvec2>& touches);
		virtual bool onButtonDown(april::Button buttonCode);
		virtual bool onButtonUp(april::Button buttonCode);
		virtual void mouseCancel();

		virtual void update(float k);
		void draw();
		
		Object* findChildByName(chstr name);
		Object* findDescendantByName(chstr name);
		
		void resetCenter();
		
		virtual hstr getProperty(chstr name, bool* propertyExists = NULL);
		virtual bool setProperty(chstr name, chstr value);
		
		// dynamic animators
		
		Animator* moveX(float x, float speed);
		Animator* moveY(float y, float speed);
		Animator* rescaleX(float x, float speed);
		Animator* rescaleY(float y, float speed);
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
		void rescale(float x, float y, float speed);
		void rescale(gvec2 scale, float speed);
		void resize(float x, float y, float speed);
		void resize(gvec2 size, float speed);
		void moveCenter(float x, float y, float speed);
		void moveCenter(gvec2 center, float speed);
		void fadeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeColor(april::Color color, float speed);
		
		Animator* moveYF(float offset, float amplitude, float speed, AnimationFunction function, float periodStart, float periodLength);
		
		Animator* moveXQueue(float x, float speed, float delay = 0.0f);
		Animator* moveYQueue(float y, float speed, float delay = 0.0f);
		Animator* moveCenterXQueue(float x, float speed, float delay = 0.0f);
		Animator* moveCenterYQueue(float y, float speed, float delay = 0.0f);
		Animator* rescaleXQueue(float x, float speed, float delay = 0.0f);
		Animator* rescaleYQueue(float y, float speed, float delay = 0.0f);
		Animator* resizeXQueue(float x, float speed, float delay = 0.0f);
		Animator* resizeYQueue(float y, float speed, float delay = 0.0f);
		Animator* rotateQueue(float angle, float speed, float delay = 0.0f);
		Animator* fadeRedQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeGreenQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeBlueQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeAlphaQueue(unsigned char a, float speed, float delay = 0.0f);
		void moveQueue(float x, float y, float speed, float delay = 0.0f);
		void moveQueue(gvec2 position, float speed, float delay = 0.0f);
		void rescaleQueue(float x, float y, float speed, float delay = 0.0f);
		void rescaleQueue(gvec2 scale, float speed, float delay = 0.0f);
		void resizeQueue(float x, float y, float speed, float delay = 0.0f);
		void resizeQueue(gvec2 size, float speed, float delay = 0.0f);
		void moveCenterQueue(float x, float y, float speed, float delay = 0.0f);
		void moveCenterQueue(gvec2 center, float speed, float delay = 0.0f);
		void fadeColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeColorQueue(april::Color color, float speed, float delay = 0.0f);

		void moveXStop();
		void moveYStop();
		void rescaleXStop();
		void rescaleYStop();
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
		void rescaleStop();
		void resizeStop();
		void moveCenterStop();
		void fadeColorStop();
		
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, april::Key keyCode = april::AK_NONE, chstr extra = "");
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, april::Button buttonCode, chstr extra = "");
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, float x, float y, april::Key keyCode = april::AK_NONE, chstr extra = "");

		DEPRECATED_ATTRIBUTE Object* getChildByName(chstr name, bool recursive) { return (!recursive ? this->findChildByName(name) : this->findDescendantByName(name)); }

	protected:
		hstr name;
		grect rect;
		gvec2 scale;
		gvec2 center;
		Object* parent;
		Object* childUnderCursor;
		bool checkedChildUnderCursor;
		harray<Object*> children;
		harray<Animator*> dynamicAnimators;
		hmap<hstr, Event*> events;
		int zOrder;
		bool enabled;
		bool visible;
		float angle;
		bool clickThrough;
		bool inheritAlpha;
		april::Color color;
		bool anchorLeft;
		bool anchorRight;
		bool anchorTop;
		bool anchorBottom;
		bool retainAnchorAspect;
		bool clip;
		bool useDisabledAlpha;
		int focusIndex;
		Dataset* dataset;
		
		void _sortChildren();

		void _updateChildrenHorizontal(float difference);
		void _updateChildrenVertical(float difference);
		
		float _getDerivedAngle(aprilui::Object* overrideRoot = NULL);
		bool _isDerivedClickThrough();
		grect _getDrawRect();
		april::Color _getDrawColor();
		float _getDisabledAlphaFactor();

		virtual void OnDraw();
		virtual void OnDrawDebug();

	};
	
}

#endif
