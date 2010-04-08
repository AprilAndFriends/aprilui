/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <map>
#include <list>
#include "april/RenderSystem.h"
#include "Exception.h"
#include "atres/Atres.h"
#include "gtypes/Vector2.h"
#include "AprilUIExport.h"

struct _xmlNode;

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
		std::string mTypeName,mName;
		int mZOrder;
		float mX,mY,mWidth,mHeight;
		std::list<Object*> mChildren;
		
		std::map<std::string,Event*> mEvents;
		bool mVisible;
		bool mEnabled;
		float mAlpha;
		
		Dataset* mDataPtr;
		
		void sortChildren();
		
		void triggerEvent(std::string name,float x=0,float y=0,char* extra=0);
		float getDerivedAlpha();
		
		virtual void OnDraw(float offset_x,float offset_y) { };
		virtual void OnUpdate(float k) { };
		void setParent(Object* parent) { mParent=parent; }

		void _moveChildToFront(Object* o);
		void _moveChildToBack(Object* o);
	public:
		Object(std::string type_name,std::string name,float x,float y,float w,float h);
		virtual ~Object();

		void addChild(Object* o);
		void removeChild(Object* o);
		Object* getParent() { return mParent; }
		void removeAllChildren();

		Object* getChildUnderPoint(float x,float y);

		int getZOrder() { return mZOrder; }
		void setZOrder(int zorder);
		
		virtual bool isPointInside(float x,float y);
		void registerEvent(std::string event_name,void (*callback)(EventArgs*));

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
		bool isDerivedEnabled();
		bool getVisibilityFlag() { return mVisible; }
		
		void setAlpha(float alpha) { mAlpha=alpha; }
		float getAlpha() { return mAlpha; }

		void moveToFront();
		void moveToBack();
		
		void _setTypeName(std::string type) { mTypeName=type; }
		const std::string& getType() { return mTypeName; }
		const std::string& getName() { return mName; }
		
		// if a childs event returns true, event is not propagated to parents
		virtual bool OnMouseDown(int button,float x,float y);
		virtual bool OnMouseUp(int button,float x,float y);
		virtual void OnMouseMove(float x,float y);
		
		
		virtual void update(float k);
		void draw(float offset_x=0,float offset_y=0);
		

		virtual void notifyEvent(const std::string& event_name,void* params);
		
		virtual void setProperty(const std::string& name,const std::string& value);
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
		DummyObject(std::string name,float x,float y,float w,float h);
		void setProperty(const std::string& name,const std::string& value);
	};

	class ColoredQuad : public Object
	{
		float mRed,mGreen,mBlue,mAlpha;
	public:
		ColoredQuad(std::string name,float x,float y,float w,float h);
		void setColor(float a,float r,float g,float b);
		
		void OnDraw(float offset_x,float offset_y);
		void setProperty(const std::string& name,const std::string& value);

		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
	};
	/*******************************************************************************/
	class AprilUIExport ImageBox : public Object
	{
	protected:
		Image* mImage;
		
		void OnDraw(float offset_x,float offset_y);
	public:
		ImageBox(std::string name,float x,float y,float w,float h);
		
		virtual Image* getImage() { return mImage; };
		virtual void setImage(Image* image);
		void setImageByName(std::string image);
		
		void setProperty(const std::string& name,const std::string& value);
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
		ColoredImageBox(std::string name,float x,float y,float w,float h);
		
		void setColor(std::string color);
		void setColor(April::Color color) { mColor=color; } ;
		April::Color getColor() { return mColor; };

		void setProperty(const std::string& name,const std::string& value);
	};
	/*******************************************************************************/
	class AprilUIExport RotationImageBox : public ImageBox
	{
	protected:
		float mAngle;
		void OnDraw(float offset_x,float offset_y);
	public:
		RotationImageBox(std::string name,float x,float y,float w,float h);

		virtual void setAngle(float angle) { mAngle=angle; }
		float getAngle() { return mAngle; }
		bool angleEquals(float angle);
		void setProperty(const std::string& name,const std::string& value);
	};
	/*******************************************************************************/
	class AprilUIExport RotatableImageBox : public RotationImageBox
	{
	protected:
		float mDestAngle;
		float mRotationSpeed;
	public:
		RotatableImageBox(std::string name,float x,float y,float w,float h);
		
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
	class LabelBase
	{
	protected:
		std::string mText;
		std::string mFontName;
		Atres::Alignment mHorzFormatting;
		VertFormatting mVertFormatting;
		Atres::Effect mFontEffect;
		
		April::Color mTextColor;

		void _drawLabel(float offset_x,float offset_y,float width,float height,float alpha);
	public:
		LabelBase(std::string name);
		
		std::string getText() { return mText; }
		void setText(std::string text) { mText=text; }
		
		std::string getFont() { return mFontName; }
		void setFont(std::string font) { mFontName=font; }
		
		void setHorzFormatting(Atres::Alignment f) { mHorzFormatting=f; }
		Atres::Alignment getHorzFormatting() { return mHorzFormatting; }
		
		void setFontEffect(Atres::Effect f) { mFontEffect=f; }
		Atres::Effect getFontEffect() { return mFontEffect; }
		
		void setVertFormatting(VertFormatting f) { mVertFormatting=f; }
		VertFormatting getVertFormatting() { return mVertFormatting; }
		
		virtual void setProperty(const std::string& name,const std::string& value);

		void setTextColor(April::Color color) { mTextColor=color; }
		void setTextColor(std::string hex);
		April::Color getTextColor() { return mTextColor; }
	};
	/*******************************************************************************/
	class AprilUIExport Label : public Object, public LabelBase
	{
	protected:
		void OnDraw(float offset_x,float offset_y);
	public:
		Label(std::string name,float x,float y,float w,float h);
		
		void setTextKey(std::string key);
		void setProperty(const std::string& name,const std::string& value);

	};
	/*******************************************************************************/
	class AprilUIExport TextButton : public Label
	{
	protected:
		bool mPushed,mBackgroundEnabled;
		void OnDraw(float offset_x,float offset_y);
	public:
		TextButton(std::string name,float x,float y,float w,float h);
		
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void setProperty(const std::string& name,const std::string& value);
	};

	/*******************************************************************************/
	class AprilUIExport Slider : public ImageBox
	{
		bool mPushed;
		float mValue;
	public:
		Slider(std::string name,float x,float y,float w,float h);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void OnMouseMove(float x,float y);
		void OnDraw(float offset_x,float offset_y);
		void setProperty(const std::string& name,const std::string& value);
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
		
		ImageButton(std::string name,float x,float y,float w,float h);
		Image* getPushedImage() { return mPushedImage; }
		Image* getHoverImage() { return mHoverImage; }
		Image* getDisabledImage() { return mDisabledImage; }
		void setPushedImage(Image* image);
		void setHoverImage(Image* image);
		void setDisabledImage(Image* image);
		void setPushedImageByName(std::string image);
		void setHoverImageByName(std::string image);
		void setDisabledImageByName(std::string image);
		void OnDraw(float offset_x,float offset_y);
		Image* getImage() { return mNormalImage; };
		void setImage(Image* image);
		
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		
		void setProperty(const std::string& name,const std::string& value);
	};
	/*******************************************************************************/
	class AprilUIExport TextImageButton : public ImageButton, public LabelBase
	{
	protected:
		void OnDraw(float offset_x,float offset_y);

	public:
		TextImageButton(std::string name,float x,float y,float w,float h);
		
		void setTextKey(std::string key);
		void setProperty(const std::string& name,const std::string& value);
	};
	/*******************************************************************************/
	class AprilUIExport ToggleButton : public ImageButton
	{
	public:
		ToggleButton(std::string name,float x,float y,float w,float h);

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
