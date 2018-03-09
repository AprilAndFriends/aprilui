/// @file
/// @version 5.0
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
	
	typedef bool (*CustomPointInsideCallback)(const Object*, cgvec2);

	class apriluiExport Object : public BaseObject
	{
		APRILUI_CLONEABLE(Object);
	public:
		friend class Dataset;

		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, HitTest,
		(
			HL_ENUM_DECLARE(HitTest, Enabled);
			HL_ENUM_DECLARE(HitTest, Disabled);
			HL_ENUM_DECLARE(HitTest, DisabledRecursive);
		));

		Object(chstr name);
		~Object();
		inline hstr getClassName() const { return "Object"; }
		Object* cloneTree() const;

		harray<PropertyDescription> getPropertyDescriptions() const;

		HL_DEFINE_GET(grect, rect, Rect);
		void setRect(cgrect value);
		void setRect(cgvec2 position, cgvec2 size);
		void setRect(cgvec2 position, float w, float h);
		void setRect(float x, float y, cgvec2 size);
		void setRect(float x, float y, float w, float h);
		HL_DEFINE_GETSET(float, rect.x, X);
		HL_DEFINE_GETSET(float, rect.y, Y);
		HL_DEFINE_GET(float, rect.w, Width);
		void setWidth(const float& value);
		HL_DEFINE_GET(float, rect.h, Height);
		void setHeight(const float& value);
		inline gvec2 getPosition() const { return this->rect.getPosition(); }
		inline void setPosition(cgvec2 value) { this->rect.setPosition(value); }
		inline void setPosition(float x, float y) { this->rect.setPosition(x, y); }
		inline gvec2 getSize() const { return this->rect.getSize(); }
		void setSize(cgvec2 value);
		void setSize(float w, float h);

		HL_DEFINE_GETSET(gvec2, pivot, Pivot);
		inline void setPivot(float x, float y) { this->pivot.set(x, y); }
		HL_DEFINE_GETSET(float, pivot.x, PivotX);
		HL_DEFINE_GETSET(float, pivot.y, PivotY);

		HL_DEFINE_GETSET(april::Color, color, Color);
		void setSymbolicColor(chstr value);
		HL_DEFINE_GETSET(unsigned char, color.r, Red);
		HL_DEFINE_GETSET(unsigned char, color.g, Green);
		HL_DEFINE_GETSET(unsigned char, color.b, Blue);
		HL_DEFINE_GETSET(unsigned char, color.a, Alpha);

		inline bool isVisible() const { return (this->visible && this->color.a > 0); }
		HL_DEFINE_SET(bool, visible, Visible);
		inline bool getVisibilityFlag() const { return this->visible; }

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
		virtual inline int getFocusIndex() const { return this->focusIndex; }
		HL_DEFINE_SET(int, focusIndex, FocusIndex);
		CustomPointInsideCallback getCustomPointInsideCallback() { return this->customPointInsideCallback; }
		void setCustomPointInsideCallback(CustomPointInsideCallback callback) { this->customPointInsideCallback = callback; }

		bool isFocused() const;
		virtual void setFocused(bool focused);
		virtual bool isCursorInside() const;
		Object* getChildUnderCursor();

		harray<BaseImage*> getUsedImages() const;
		unsigned char getDerivedAlpha(aprilui::Object* overrideRoot = NULL) const;

		void addChild(BaseObject* object);
		void removeChild(BaseObject* object);
		void registerChild(BaseObject* object);
		void unregisterChild(BaseObject* object);
		void removeChildren(bool recursive = false);
		void destroyChildren();

		Object* getChildUnderPoint(cgvec2 point) const;
		Object* getChildUnderPoint(float x, float y) const;
		virtual void clearChildUnderCursor();
		void clearDescendantChildrenUnderCursor();
		virtual bool isPointInside(cgvec2 position) const;
		bool angleEquals(float angle) const;

		harray<gvec2> transformToLocalSpace(const harray<gvec2>& points, aprilui::Object* overrideRoot = NULL) const;
		gvec2 transformToLocalSpace(cgvec2 point, aprilui::Object* overrideRoot = NULL) const;

		harray<gvec2> getDerivedPoints(const harray<gvec2>& points, aprilui::Object* overrideRoot = NULL) const;
		gvec2 getDerivedPoint(cgvec2 point, aprilui::Object* overrideRoot = NULL) const;
		grect getBoundingRect(aprilui::Object* overrideRoot = NULL) const;
		/// @note The points are ordered as top-left, top-right, bottom-left, bottom-right within the local space.
		harray<gvec2> getDerivedCorners(aprilui::Object* overrideRoot = NULL) const;

		gvec2 getDerivedPosition(aprilui::Object* overrideRoot = NULL) const;
		gvec2 getDerivedSize(aprilui::Object* overrideRoot = NULL) const;
		gvec2 getDerivedPivot(aprilui::Object* overrideRoot = NULL) const;
		gvec2 getDerivedScale(aprilui::Object* overrideRoot = NULL) const;
		float getDerivedAngle(aprilui::Object* overrideRoot = NULL) const;
		bool isDerivedVisible() const;
		bool isAnimated() const;
		bool isWaitingAnimation() const;
		bool hasDynamicAnimation() const;
		
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
		
		void resetPivot();
		
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
		Animator* movePivotX(float x, float speed);
		Animator* movePivotY(float y, float speed);
		Animator* fadeRed(unsigned char r, float speed);
		Animator* fadeGreen(unsigned char g, float speed);
		Animator* fadeBlue(unsigned char b, float speed);
		Animator* fadeAlpha(unsigned char a, float speed);
		Animator* changeZOrder(int z, float speed);
		void move(float x, float y, float speed);
		void move(cgvec2 position, float speed);
		void scale(float x, float y, float speed);
		void scale(cgvec2 scale, float speed);
		void resize(float x, float y, float speed);
		void resize(cgvec2 size, float speed);
		void movePivot(float x, float y, float speed);
		void movePivot(cgvec2 pivot, float speed);
		void fadeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeColor(const april::Color& color, float speed);

		Animator* moveXQueue(float x, float speed, float delay = 0.0f);
		Animator* moveYQueue(float y, float speed, float delay = 0.0f);
		Animator* movePivotXQueue(float x, float speed, float delay = 0.0f);
		Animator* movePivotYQueue(float y, float speed, float delay = 0.0f);
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
		void moveQueue(cgvec2 position, float speed, float delay = 0.0f);
		void scaleQueue(float x, float y, float speed, float delay = 0.0f);
		void scaleQueue(cgvec2 scale, float speed, float delay = 0.0f);
		void resizeQueue(float x, float y, float speed, float delay = 0.0f);
		void resizeQueue(cgvec2 size, float speed, float delay = 0.0f);
		void movePivotQueue(float x, float y, float speed, float delay = 0.0f);
		void movePivotQueue(cgvec2 pivot, float speed, float delay = 0.0f);
		void fadeColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeColorQueue(const april::Color& color, float speed, float delay = 0.0f);

		Animator* animateX(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateY(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateScaleX(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateScaleY(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateWidth(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateHeight(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateAngle(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animatePivotX(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animatePivotY(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateRed(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateGreen(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateBlue(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateAlpha(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		Animator* animateZOrder(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animatePosition(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animateScale(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animateSize(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animatePivot(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animateColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);

		void animateStopX();
		void animateStopY();
		void animateStopScaleX();
		void animateStopScaleY();
		void animateStopWidth();
		void animateStopHeight();
		void animateStopAngle();
		void animateStopPivotX();
		void animateStopPivotY();
		void animateStopRed();
		void animateStopGreen();
		void animateStopBlue();
		void animateStopAlpha();
		void animateStopZOrder();
		void animateStopMove();
		void animateStopScale();
		void animateStopSize();
		void animateStopPivot();
		void animateStopColor();

		void animateStopAll();

	protected:
		grect rect;
		gvec2 pivot;
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
		
		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _updateChildrenHorizontal(float difference);
		void _updateChildrenVertical(float difference);
		void _cloneChildren(const harray<Object*>& objects, const harray<Animator*>& animators);

		float _getDerivedAngle(aprilui::Object* overrideRoot = NULL) const;
		bool _isDerivedHitTestEnabled() const;
		grect _makeDrawRect() const;
		april::Color _makeDrawColor() const;
		virtual april::Color _makeDrawColor(const april::Color& color) const;
		virtual harray<BaseImage*> _getUsedImages() const;

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
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

		Object* _childUnderCursor;
		bool _checkedChildUnderCursor;

	};
	
}

#endif
