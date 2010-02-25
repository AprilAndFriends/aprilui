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
#include "Vector.h"
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
		float getDerivedAlpha(Object* o);
		
		virtual void OnDraw(float offset_x,float offset_y) { };
		virtual void OnUpdate(float k) { };
		void setParent(Object* parent) { mParent=parent; }

		void _moveChildToFront(Object* o);
		void _moveChildToBack(Object* o);
	public:
		Object(std::string type_name,std::string name,int x,int y,int w,int h);
		~Object();

		void addChild(Object* o);
		void removeChild(Object* o);
		Object* getParent() { return mParent; }
		void removeAllChildren();

		Object* getChildUnderPoint(int x,int y);

		int getZOrder() { return mZOrder; }
		void setZOrder(int zorder);
		
		virtual bool isPointInside(float x,float y);
		void registerEvent(std::string event_name,void (*callback)(EventArgs*));
		
		
		Vector getPosition() { return Vector(mX,mY); };
		float getXPosition() { return mX; }
		float getYPosition() { return mY; }
		Vector getSize() { return Vector(mWidth,mHeight); }
		float getWidth() { return mWidth; }
		float getHeight() { return mHeight; }
		void setPosition(float x,float y) { mX=x; mY=y; }
		void setPosition(Vector v) { mX=v.x; mY=v.y; }
		void setXPosition(float x) { mX=x; }
		void setYPosition(float y) { mY=y; }

		void setSize(float w,float h) { mWidth=w; mHeight=h; }

		void setVisible(bool visible) { mVisible=visible; }
		bool isVisible() { return mVisible && mAlpha > 0; }
		void setEnabled(bool enabled) { mEnabled=enabled; }
		bool isEnabled() { return mEnabled; }
		bool getVisibilityFlag() { return mVisible; }
		
		void setAlpha(float alpha) { mAlpha=alpha; }
		float getAlpha() { return mAlpha; }

		void moveToFront();
		void moveToBack();
		
		void _setTypeName(std::string type) { mTypeName=type; }
		std::string getType() { return mTypeName; }
		std::string getName() { return mName; }
		
		// if a childs event returns true, event is not propagated to parents
		virtual bool OnMouseDown(int button,float x,float y);
		virtual bool OnMouseUp(int button,float x,float y);
		virtual void OnMouseMove(float x,float y);
		
		
		virtual void update(float k);
		void draw(float offset_x=0,float offset_y=0);
		

		virtual void notifyEvent(std::string event_name,void* params);
		
		virtual void setProperty(std::string name,std::string value);
		// system call, do not use!
		void _setDataset(Dataset* d) { mDataPtr=d; }
	};

	enum VertFormatting
	{
		TOP,BOTTOM
	};
	/*******************************************************************************/
	class AprilUIExport DummyObject : public Object
	{
	public:
		DummyObject(std::string name,float x,float y,float w,float h);
		void setProperty(std::string name,std::string value);
	};

	class ColoredQuad : public Object
	{
		float mRed,mGreen,mBlue,mAlpha;
	public:
		ColoredQuad(std::string name,float x,float y,float w,float h);
		void setColor(float r,float g,float b,float a);
		
		void OnDraw(float offset_x,float offset_y);
		void setProperty(std::string name,std::string value);

		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
	};
	/*******************************************************************************/
	class AprilUIExport StaticImage : public Object
	{
	protected:
		Image* mImage;
		
		void OnDraw(float offset_x,float offset_y);
	public:
		StaticImage(std::string name,float x,float y,float w,float h);
		
		virtual Image* getImage() { return mImage; };
		virtual void setImage(Image* image);
		void setImageByName(std::string image);
		
		void setProperty(std::string name,std::string value);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
	};
	/*******************************************************************************/
	class AprilUIExport RotationImage : public StaticImage
	{
	protected:
		float mAngle;
		void OnDraw(float offset_x,float offset_y);
	public:
		RotationImage(std::string name,float x,float y,float w,float h);

		virtual void setAngle(float angle) { mAngle=angle; }
		float getAngle() { return mAngle; }
		bool angleEquals(float angle);
		void setProperty(std::string name,std::string value);
	};
	/*******************************************************************************/
	class AprilUIExport RotatableImage : public RotationImage
	{
	protected:
		float mDestAngle;
		float mRotationSpeed;
	public:
		RotatableImage(std::string name,float x,float y,float w,float h);
		
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
	class AprilUIExport Label : public Object
	{
	protected:
		std::string mText;
		std::string mFontName;
		Atres::Alignment mHorzFormatting;
		VertFormatting mVertFormatting;
		Atres::Effect mFontEffect;
		
		April::Color mTextColor;

		void OnDraw(float offset_x,float offset_y);
	public:
		Label(std::string name,float x,float y,float w,float h);
		
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
		
		void setProperty(std::string name,std::string value);

		void setTextColor(April::Color color) { mTextColor=color; }
		April::Color getTextColor() { return mTextColor; }
	};
	/*******************************************************************************/
	class AprilUIExport TextButton : public Label
	{
	protected:
		bool mPushed;
		void OnDraw(float offset_x,float offset_y);
	public:
		TextButton(std::string name,float x,float y,float w,float h);
		
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);

	};

	/*******************************************************************************/
	class AprilUIExport Slider : public StaticImage
	{
		bool mPushed;
		float mValue;
	public:
		Slider(std::string name,float x,float y,float w,float h);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void OnMouseMove(float x,float y);
		void OnDraw(float offset_x,float offset_y);
		void setProperty(std::string name,std::string value);
		float getValue() { return mValue; }
		void setValue(float value) { mValue=value; }
		
	};
	/*******************************************************************************/
	class AprilUIExport Button : public StaticImage
	{
	protected:
		Image *mPushedImage,*mHoverImage,*mNormalImage,*mDisabledImage;
		bool mPushed;
		
		void OnUpdate(float k);
	public:
		
		Button(std::string name,float x,float y,float w,float h);
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
		
		void setProperty(std::string name,std::string value);
	};
	/*******************************************************************************/
	class AprilUIExport ToggleButton : public Button
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
