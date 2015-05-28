/// @file
/// @version 4.05
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an object that is actually displayed visually.

#ifndef APRILUI_OBJECT_H
#define APRILUI_OBJECT_H

#include <april/Color.h>
#include <april/Keys.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "apriluiExport.h"
#include "BaseObject.h"
#include "EventReceiver.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Dataset;
	class BaseImage;
	class Event;
	class EventArgs;
	class Object;
	
	typedef bool (*CustomPointInsideCallback)(Object*, gvec2);

	class apriluiExport Object : public BaseObject
	{
		APRILUI_CLONEABLE(Object);
	public:
		friend class Dataset;

		enum HitTest
		{
			HIT_TEST_ENABLED = 0,
			HIT_TEST_DISABLED = 1,
			HIT_TEST_DISABLED_RECURSIVE = 2
		};

		Object(chstr name);
		~Object();
		inline hstr getClassName() const { return "Object"; }
		Object* cloneTree();

		HL_DEFINE_GET(grect, rect, Rect);
		void setRect(grect value);
		void setRect(gvec2 position, gvec2 size);
		void setRect(gvec2 position, float w, float h);
		void setRect(float x, float y, gvec2 size);
		void setRect(float x, float y, float w, float h);
		HL_DEFINE_GETSET(float, rect.x, X);
		HL_DEFINE_GETSET(float, rect.y, Y);
		HL_DEFINE_GET(float, rect.w, Width);
		void setWidth(float value);
		HL_DEFINE_GET(float, rect.h, Height);
		void setHeight(float value);
		inline gvec2 getPosition() { return this->rect.getPosition(); }
		inline void setPosition(gvec2 value) { this->rect.setPosition(value); }
		inline void setPosition(float x, float y) { this->rect.setPosition(x, y); }
		inline gvec2 getSize() { return this->rect.getSize(); }
		void setSize(gvec2 value);
		void setSize(float w, float h);

		HL_DEFINE_GETSET(gvec2, center, Center);
		inline void setCenter(float x, float y) { this->center.set(x, y); }
		HL_DEFINE_GETSET(float, center.x, CenterX);
		HL_DEFINE_GETSET(float, center.y, CenterY);

		HL_DEFINE_GETSET(april::Color, color, Color);
		HL_DEFINE_GETSET(unsigned char, color.r, Red);
		HL_DEFINE_GETSET(unsigned char, color.g, Green);
		HL_DEFINE_GETSET(unsigned char, color.b, Blue);
		HL_DEFINE_GETSET(unsigned char, color.a, Alpha);

		inline bool isVisible() { return (this->visible && this->color.a > 0); }
		HL_DEFINE_SET(bool, visible, Visible);
		inline bool getVisibilityFlag() { return this->visible; }

		HL_DEFINE_GETSET(gvec2, scaleFactor, Scale);
		inline void setScale(float x, float y) { this->scaleFactor.set(x, y); }
		HL_DEFINE_GETSET(float, scaleFactor.x, ScaleX);
		HL_DEFINE_GETSET(float, scaleFactor.y, ScaleY);

		HL_DEFINE_GETSET(float, angle, Angle);
		HL_DEFINE_ISSET(anchorLeft, AnchorLeft);
		HL_DEFINE_ISSET(anchorRight, AnchorRight);
		HL_DEFINE_ISSET(anchorTop, AnchorTop);
		HL_DEFINE_ISSET(anchorBottom, AnchorBottom);
		void setAnchors(bool left, bool right, bool top, bool bottom);
		HL_DEFINE_ISSET(retainAnchorAspect, RetainAnchorAspect);
		HL_DEFINE_GETSET(HitTest, hitTest, HitTest);
		HL_DEFINE_ISSET(inheritAlpha, InheritAlpha);
		HL_DEFINE_ISSET(useDisabledAlpha, UseDisabledAlpha);
		HL_DEFINE_ISSET(clip, Clip);
		virtual HL_DEFINE_GET(int, focusIndex, FocusIndex);
		HL_DEFINE_SET(int, focusIndex, FocusIndex);
		void setCustomPointInsideCallback(CustomPointInsideCallback callback) { this->customPointInsideCallback = callback; }
		CustomPointInsideCallback getCustomPointInsideCallback() { return this->customPointInsideCallback; }

		harray<PropertyDescription> getPropertyDescriptions();

		virtual bool isFocused();
		virtual void setFocused(bool focused);
		bool isCursorInside();
		Object* getChildUnderCursor();

		virtual harray<BaseImage*> getUsedImages();
		unsigned char getDerivedAlpha(aprilui::Object* overrideRoot = NULL);

		void addChild(BaseObject* object);
		void removeChild(BaseObject* object);
		void registerChild(BaseObject* object);
		void unregisterChild(BaseObject* object);
		void removeChildren(bool recursive = false);
		void destroyChildren();

		Object* getChildUnderPoint(gvec2 pos);
		Object* getChildUnderPoint(float x, float y);
		virtual void clearChildUnderCursor();
		virtual bool isPointInside(gvec2 position);
		bool angleEquals(float angle);

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
		float getDerivedAngle(aprilui::Object* overrideRoot = NULL);
		bool isDerivedVisible();
		virtual bool isAnimated();
		virtual bool isWaitingAnimation();
		bool hasDynamicAnimation();
		
		// if this returns true, the event is processed and is not propagated to parents or other siblings, etc.
		// (these should usually not be overriden)
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

		void draw();
		
		void resetCenter();
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
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
		Animator* changeZOrder(int z, float speed);
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
		
		Animator* moveXF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* moveYF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* scaleXF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* scaleYF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* resizeXF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* resizeYF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* rotateF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* moveCenterXF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* moveCenterYF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* fadeRedF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* fadeGreenF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* fadeBlueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* fadeAlphaF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		Animator* changeZOrderF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods);
		
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
		Animator* changeZOrderQueue(int z, float speed, float delay = 0.0f);
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

		Animator* moveXQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* moveYQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* scaleXQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* scaleYQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* resizeXQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* resizeYQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* rotateQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* moveCenterXQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* moveCenterYQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* fadeRedQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* fadeGreenQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* fadeBlueQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* fadeAlphaQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		Animator* changeZOrderQueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay = 0.0f);
		
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
		void changeZOrderStop();
		void moveStop();
		void scaleStop();
		void resizeStop();
		void moveCenterStop();
		void fadeColorStop();

		void stopAllAnimations();

		DEPRECATED_ATTRIBUTE bool isClickThrough();
		DEPRECATED_ATTRIBUTE inline void setClickThrough(bool value) { this->hitTest = (value ? HIT_TEST_DISABLED_RECURSIVE : HIT_TEST_ENABLED); }

	protected:
		grect rect;
		gvec2 center;
		april::Color color;
		bool visible;
		gvec2 scaleFactor;
		float angle;
		bool anchorLeft;
		bool anchorRight;
		bool anchorTop;
		bool anchorBottom;
		bool retainAnchorAspect;
		HitTest hitTest;
		bool clip;
		bool inheritAlpha;
		bool useDisabledAlpha;
		int focusIndex;
		CustomPointInsideCallback customPointInsideCallback;
		harray<Animator*> dynamicAnimators;
		april::Color debugColor;
		
		void _updateChildrenHorizontal(float difference);
		void _updateChildrenVertical(float difference);
		void _cloneChildren(harray<Object*>& objects, harray<Animator*>& animators);

		float _getDerivedAngle(aprilui::Object* overrideRoot = NULL);
		bool _isDerivedHitTestEnabled();
		grect _getDrawRect();
		april::Color _getDrawColor();
		float _getDisabledAlphaFactor();

		void _update(float timeDelta);
		virtual void _draw();
		virtual void _drawDebug();

		// if this returns true, the event is processed and is not propagated to parents or other siblings, etc.
		virtual bool _mouseDown(april::Key keyCode);
		virtual bool _mouseUp(april::Key keyCode);
		virtual void _mouseCancel(april::Key keyCode);
		virtual bool _mouseMove();
		virtual bool _mouseScroll(float x, float y);
		virtual bool _keyDown(april::Key keyCode);
		virtual bool _keyUp(april::Key keyCode);
		virtual bool _char(unsigned int charCode);
		virtual bool _touch(const harray<gvec2>& touches);
		virtual bool _buttonDown(april::Button buttonCode);
		virtual bool _buttonUp(april::Button buttonCode);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		Object* _childUnderCursor;
		bool _checkedChildUnderCursor;

	};
	
}

#endif
