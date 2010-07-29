/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef OBJECTS_H
#define OBJECTS_H

#include <map>

#include <april/RenderSystem.h>
#include <atres/Atres.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "Exception.h"

struct xml_node;

namespace AprilUI
{
	class Image;
	class Object;
	class Dataset;

	class AprilUIExport EventArgs
	{
	public:
		EventArgs(Object* o,float x_pos=0,float y_pos=0,char* extra_string=0) { x=x_pos; y=y_pos; object=o; extra=extra_string; }
		Object* object;
		float x,y;
		char* extra;
	};

	class AprilUIExport Event
	{
	public:
		virtual void execute(void* params)=0;
	};

	class CallbackEvent : public Event
	{
		void (*mCallback)(EventArgs*);
	public:
		CallbackEvent(void (*callback)(EventArgs*));
		void execute(void* params);
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

	enum VertFormatting
	{
		VERT_TOP,VERT_CENTER,VERT_BOTTOM
	};
	/*******************************************************************************/
	class AprilUIExport DummyObject : public Object
	{
	public:
		DummyObject(chstr name,float x,float y,float w,float h);
		void setProperty(chstr name,chstr value);
	};
	/*******************************************************************************/
	class AprilUIExport CallbackObject : public Object
	{
		void (*mCallback)();
	public:
		CallbackObject(chstr name,float x,float y,float w,float h);
		void setProperty(chstr name,chstr value);
		
		void setCallback(void (*callback)()) { mCallback=callback; }
		void (*getCallback())() { return mCallback; }
		
		void OnDraw(float offset_x=0,float offset_y=0);
	};
	/*******************************************************************************/
	class ColoredQuad : public Object
	{
		April::Color mColor;
	public:
		ColoredQuad(chstr name,float x,float y,float w,float h);
		void setColor(float a,float r,float g,float b);
		
		void OnDraw(float offset_x,float offset_y);
		void setProperty(chstr name,chstr value);

		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
	};
	/*******************************************************************************/
	class AprilUIExport ImageBox : public Object
	{
	protected:
		Image* mImage;
		hstr mImageName;
		
		void OnDraw(float offset_x,float offset_y);
	public:
		ImageBox(chstr name,float x,float y,float w,float h);
		
		virtual Image* getImage() { return mImage; };
		virtual void setImage(Image* image);
		void setImageByName(chstr image);
		hstr getImageName() { return mImageName; }
		void notifyEvent(chstr event_name,void* params);
		
		void resizeToFitImage();
		
		void setProperty(chstr name,chstr value);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
	};
	/*******************************************************************************/
	class AprilUIExport ColoredImageBox : public ImageBox
	{
	protected:
		April::Color mColor;
		
		void OnDraw(float offset_x,float offset_y);
	public:
		ColoredImageBox(chstr name,float x,float y,float w,float h);
		
		void setColor(chstr color);
		void setColor(April::Color color) { mColor=color; } ;
		April::Color getColor() { return mColor; };

		void setProperty(chstr name,chstr value);
	};
	/*******************************************************************************/
	class AprilUIExport RotationImageBox : public ImageBox
	{
	protected:
		float mAngle;
		void OnDraw(float offset_x,float offset_y);
	public:
		RotationImageBox(chstr name,float x,float y,float w,float h);

		virtual void setAngle(float angle) { mAngle=angle; }
		float getAngle() { return mAngle; }
		bool angleEquals(float angle);
		void setProperty(chstr name,chstr value);
	};
	/*******************************************************************************/
	class AprilUIExport RotatableImageBox : public RotationImageBox
	{
	protected:
		float mDestAngle;
		float mRotationSpeed;
	public:
		RotatableImageBox(chstr name,float x,float y,float w,float h);
		
		void setRotationSpeed(float speed) { mRotationSpeed=speed; }
		float getRotationSpeed() { return mRotationSpeed; }
		void setAngle(float angle) { mAngle=mDestAngle=angle; }
		void setDestAngle(float angle) { mDestAngle=angle; }
		float getDestAngle() { return mDestAngle; }
		void rotate(float angle) { mDestAngle+=angle; }
		bool isRotating();

		void update(float k);
	};
	/*******************************************************************************/
	class AprilUIExport LabelBase
	{
	protected:
		hstr mText;
		hstr mFontName;
		Atres::Alignment mHorzFormatting;
		VertFormatting mVertFormatting;
		Atres::Effect mFontEffect;
		
		April::Color mTextColor;

		void _drawLabel(float offset_x,float offset_y,float width,float height,float alpha);
	public:
		LabelBase(chstr name);
		
		hstr getText() { return mText; }
		void setText(chstr text) { mText=text; }
		virtual void setTextKey(chstr key) = 0;
		
		hstr getFont() { return mFontName; }
		void setFont(chstr font) { mFontName=font; }
		
		void setHorzFormatting(Atres::Alignment f) { mHorzFormatting=f; }
		Atres::Alignment getHorzFormatting() { return mHorzFormatting; }
		
		void setFontEffect(Atres::Effect f) { mFontEffect=f; }
		Atres::Effect getFontEffect() { return mFontEffect; }
		
		void setVertFormatting(VertFormatting f) { mVertFormatting=f; }
		VertFormatting getVertFormatting() { return mVertFormatting; }
		
		virtual void setProperty(chstr name,chstr value);

		void setTextColor(April::Color color) { mTextColor=color; }
		void setTextColor(chstr hex);
		April::Color getTextColor() { return mTextColor; }
	};
	/*******************************************************************************/
	class AprilUIExport Label : public Object, public LabelBase
	{
	protected:
		hstr mTextKey;
		void OnDraw(float offset_x,float offset_y);
	public:
		Label(chstr name,float x,float y,float w,float h);
		
		void setTextKey(chstr key);
		chstr getTextXey() { return mTextKey; }
		void setProperty(chstr name,chstr value);
		void notifyEvent(chstr event_name,void* params);
	};
	/*******************************************************************************/
	class AprilUIExport TextButton : public Label
	{
	protected:
		bool mPushed,mBackgroundEnabled;
		void OnDraw(float offset_x,float offset_y);
	public:
		TextButton(chstr name,float x,float y,float w,float h);
		
		void setTextKey(chstr key);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void setProperty(chstr name,chstr value);
	};

	/*******************************************************************************/
	class AprilUIExport EditBox : public Label
	{
	protected:
		bool mPushed;
		//int mCaratIndex;
		void OnDraw(float offset_x,float offset_y);
	public:
		EditBox(chstr name,float x,float y,float w,float h);
		
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void setProperty(chstr name,chstr value);
	};

	/*******************************************************************************/
	class AprilUIExport Slider : public ImageBox
	{
		bool mPushed;
		float mValue;
	public:
		Slider(chstr name,float x,float y,float w,float h);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void OnMouseMove(float x,float y);
		void OnDraw(float offset_x,float offset_y);
		void setProperty(chstr name,chstr value);
		float getValue() { return mValue; }
		void setValue(float value) { mValue=value; }
		
	};
	/*******************************************************************************/
	class AprilUIExport ImageButton : public ImageBox
	{
	protected:
		Image *mPushedImage,*mHoverImage,*mNormalImage,*mDisabledImage;
		bool mPushed;
		
		void OnUpdate(float k);
	public:
		
		ImageButton(chstr name,float x,float y,float w,float h);
		Image* getPushedImage() { return mPushedImage; }
		Image* getHoverImage() { return mHoverImage; }
		Image* getDisabledImage() { return mDisabledImage; }
		void setPushedImage(Image* image);
		void setHoverImage(Image* image);
		void setDisabledImage(Image* image);
		void setPushedImageByName(chstr image);
		void setHoverImageByName(chstr image);
		void setDisabledImageByName(chstr image);
		void OnDraw(float offset_x,float offset_y);
		Image* getImage() { return mNormalImage; };
		void setImage(Image* image);
		
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		
		void setProperty(chstr name,chstr value);
	};
	/*******************************************************************************/
	class AprilUIExport TextImageButton : public ImageButton, public LabelBase
	{
	protected:
		void OnDraw(float offset_x,float offset_y);

	public:
		TextImageButton(chstr name,float x,float y,float w,float h);
		
		void setTextKey(chstr key);
		void setProperty(chstr name,chstr value);
	};
	/*******************************************************************************/
	class AprilUIExport ToggleButton : public ImageButton
	{
	public:
		ToggleButton(chstr name,float x,float y,float w,float h);

		void OnDraw(float offset_x,float offset_y);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void untoggle() { mPushed=0; }
		void toggle() { mPushed=!mPushed; }
		void turnOn() { mPushed=1; }
		void turnOff() { mPushed=0; }
		bool isToggled() { return mPushed; }
		
	};
}

#endif
