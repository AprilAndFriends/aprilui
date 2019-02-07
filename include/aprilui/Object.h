/// @file
/// @version 5.1
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
	
	typedef bool (*CustomPointInsideCallback)(const Object*, cgvec2f);

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

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(grectf, rect, Rect);
		void setRect(cgrectf value);
		void setRect(cgvec2f position, cgvec2f size);
		void setRect(cgvec2f position, const float& w, const float& h);
		void setRect(const float& x, const float& y, cgvec2f size);
		void setRect(const float& x, const float& y, const float& w, const float& h);
		HL_DEFINE_GET(float, rect.x, X);
		void setX(const float& value);
		HL_DEFINE_GET(float, rect.y, Y);
		void setY(const float& value);
		HL_DEFINE_GET(float, rect.w, Width);
		void setWidth(const float& value);
		HL_DEFINE_GET(float, rect.h, Height);
		void setHeight(const float& value);
		inline gvec2f getPosition() const { return this->rect.getPosition(); }
		void setPosition(cgvec2f value);
		void setPosition(const float& x, const float& y);
		inline gvec2f getSize() const { return this->rect.getSize(); }
		void setSize(cgvec2f value);
		void setSize(const float& w, const float& h);
		HL_DEFINE_GET(float, maxSize.x, MaxWidth);
		void setMaxWidth(const float& value);
		HL_DEFINE_GET(float, maxSize.y, MaxHeight);
		void setMaxHeight(const float& value);
		HL_DEFINE_GET(gvec2f, maxSize, MaxSize);
		void setMaxSize(cgvec2f value);
		void setMaxSize(const float& w, const float& h);
		HL_DEFINE_GET(gvec2f, unlimitedSize, UnlimitedSize);
		HL_DEFINE_GET(float, unlimitedSize.x, UnlimitedWidth);
		HL_DEFINE_GET(float, unlimitedSize.y, UnlimitedHeight);

		HL_DEFINE_GETSET(gvec2f, pivot, Pivot);
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

		HL_DEFINE_GETSET(gvec2f, scaleFactor, Scale);
		inline void setScale(float x, float y) { this->scaleFactor.set(x, y); }
		HL_DEFINE_GETSET(float, scaleFactor.x, ScaleX);
		HL_DEFINE_GETSET(float, scaleFactor.y, ScaleY);

		HL_DEFINE_GETSET(float, angle, Angle);
		HL_DEFINE_ISSET(anchorLeft, AnchorLeft);
		HL_DEFINE_ISSET(anchorRight, AnchorRight);
		HL_DEFINE_ISSET(anchorTop, AnchorTop);
		HL_DEFINE_ISSET(anchorBottom, AnchorBottom);
		HL_DEFINE_IS(retainAnchorAspect, RetainAnchorAspect);
		void setRetainAnchorAspect(const bool& value);
		HL_DEFINE_GETSET(HitTest, hitTest, HitTest);
		HL_DEFINE_ISSET(inheritAlpha, InheritAlpha);
		HL_DEFINE_ISSET(useDisabledAlpha, UseDisabledAlpha);
		HL_DEFINE_ISSET(clip, Clip);
		HL_DEFINE_ISSET(useClipRound, UseClipRound);
		virtual inline int getFocusIndex() const { return this->focusIndex; }
		HL_DEFINE_SET(int, focusIndex, FocusIndex);
		CustomPointInsideCallback getCustomPointInsideCallback() { return this->customPointInsideCallback; }
		void setCustomPointInsideCallback(CustomPointInsideCallback callback) { this->customPointInsideCallback = callback; }
		bool isDerivedVisible() const;
		bool isAnimated() const;
		bool isWaitingAnimation() const;
		bool hasDynamicAnimation() const;
		bool isFocused() const;
		virtual void setFocused(bool focused);
		virtual bool isCursorInside() const;
		harray<BaseImage*> getUsedImages() const;
		Object* getChildUnderCursor();

		void update(float timeDelta);
		void draw();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void setAnchors(bool left, bool right, bool top, bool bottom);
		void resetPivot();
		harray<gvec2f> transformToLocalSpace(const harray<gvec2f>& points, aprilui::Object* overrideRoot = NULL) const;
		gvec2f transformToLocalSpace(cgvec2f point, aprilui::Object* overrideRoot = NULL) const;

		unsigned char getDerivedAlpha(aprilui::Object* overrideRoot = NULL) const;
		harray<gvec2f> getDerivedPoints(const harray<gvec2f>& points, aprilui::Object* overrideRoot = NULL) const;
		gvec2f getDerivedPoint(cgvec2f point, aprilui::Object* overrideRoot = NULL) const;
		grectf getBoundingRect(aprilui::Object* overrideRoot = NULL) const;
		/// @note The points are ordered as top-left, top-right, bottom-left, bottom-right within the local space.
		harray<gvec2f> getDerivedCorners(aprilui::Object* overrideRoot = NULL) const;
		gvec2f getDerivedPosition(aprilui::Object* overrideRoot = NULL) const;
		gvec2f getDerivedSize(aprilui::Object* overrideRoot = NULL) const;
		gvec2f getDerivedPivot(aprilui::Object* overrideRoot = NULL) const;
		gvec2f getDerivedScale(aprilui::Object* overrideRoot = NULL) const;
		float getDerivedAngle(aprilui::Object* overrideRoot = NULL) const;

		virtual bool isPointInside(cgvec2f position) const;
		Object* getChildUnderPoint(cgvec2f point) const;
		Object* getChildUnderPoint(float x, float y) const;
		virtual void clearChildUnderCursor();
		void clearDescendantChildrenUnderCursor();

		void addChild(BaseObject* object);
		void removeChild(BaseObject* object);
		void registerChild(BaseObject* object);
		void unregisterChild(BaseObject* object);
		void removeChildren(bool recursive = false);
		void destroyChildren();

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
		virtual bool onTouch(const harray<gvec2f>& touches);
		virtual bool onButtonDown(april::Button buttonCode);
		virtual bool onButtonUp(april::Button buttonCode);

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
		void move(cgvec2f position, float speed);
		void scale(float x, float y, float speed);
		void scale(cgvec2f scale, float speed);
		void resize(float x, float y, float speed);
		void resize(cgvec2f size, float speed);
		void movePivot(float x, float y, float speed);
		void movePivot(cgvec2f pivot, float speed);
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
		void moveQueue(cgvec2f position, float speed, float delay = 0.0f);
		void scaleQueue(float x, float y, float speed, float delay = 0.0f);
		void scaleQueue(cgvec2f scale, float speed, float delay = 0.0f);
		void resizeQueue(float x, float y, float speed, float delay = 0.0f);
		void resizeQueue(cgvec2f size, float speed, float delay = 0.0f);
		void movePivotQueue(float x, float y, float speed, float delay = 0.0f);
		void movePivotQueue(cgvec2f pivot, float speed, float delay = 0.0f);
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

		HL_DEPRECATED("angleEquals() is deprecated, use your own implementation.")
		bool angleEquals(float angle) const { return (heqf((float)hsin(angle), (float)hsin(angle), (float)HL_E_TOLERANCE) && heqf((float)hcos(angle), (float)hcos(angle), (float)HL_E_TOLERANCE)); }

	protected:
		grectf rect;
		gvec2f maxSize;
		gvec2f unlimitedSize;
		gvec2f pivot;
		april::Color color;
		bool visible;
		gvec2f scaleFactor;
		float angle;
		bool anchorLeft;
		bool anchorRight;
		bool anchorTop;
		bool anchorBottom;
		bool retainAnchorAspect;
		HitTest hitTest;
		bool clip;
		bool useClipRound;
		bool inheritAlpha;
		bool useDisabledAlpha;
		int focusIndex;
		CustomPointInsideCallback customPointInsideCallback;
		harray<Animator*> dynamicAnimators;
		april::Color debugColor;
		
		void _cloneChildren(const harray<Object*>& objects, const harray<Animator*>& animators);

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		bool _isDerivedHitTestEnabled() const;
		virtual harray<BaseImage*> _getUsedImages() const;

		void _update(float timeDelta);
		virtual void _draw();
		virtual void _drawDebug();

		float _getDerivedAngle(aprilui::Object* overrideRoot = NULL) const;

		void _updateChildrenHorizontal(float difference);
		void _updateChildrenVertical(float difference);

		gvec2f _makeCorrectedSize(cgvec2f size);
		float _makeCorrectedWidth(const float& width);
		float _makeCorrectedHeight(const float& height);
		grectf _makeDrawRect() const;
		april::Color _makeDrawColor() const;
		virtual april::Color _makeDrawColor(const april::Color& color) const;

		// if this returns true, the event is processed and is not propagated to parents or other siblings, etc.
		virtual bool _mouseDown(april::Key keyCode);
		virtual bool _mouseUp(april::Key keyCode);
		virtual void _mouseCancel(april::Key keyCode);
		virtual bool _mouseMove();
		virtual bool _mouseScroll(float x, float y);
		virtual bool _keyDown(april::Key keyCode);
		virtual bool _keyUp(april::Key keyCode);
		virtual bool _char(unsigned int charCode);
		virtual bool _touch(const harray<gvec2f>& touches);
		virtual bool _buttonDown(april::Button buttonCode);
		virtual bool _buttonUp(april::Button buttonCode);

	private:
		bool _initialPivotX;
		bool _initialPivotY;
		Object* _childUnderCursor;
		bool _checkedChildUnderCursor;

		// optimizations
		harray<Animator*> _dynamicAnimators;
		gmat4 _drawModelviewMatrix;
		gmat4 _drawProjectionMatrix;
		grecti _drawViewport;
		bool _drawClipped;
		gvec2f _drawViewportOffset;
		grectf _drawOriginalRect;
		gvec2f _drawRatio;
		grectf _drawNewViewport;
		grectf _drawNewRect;
		grectf _drawModelviewMatrixRect;
		grecti _drawNewViewportRect;
		gvec2f _drawPosition;
		grectf _drawRect;
		april::Color _drawFrameColor;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};
	
}
#endif
