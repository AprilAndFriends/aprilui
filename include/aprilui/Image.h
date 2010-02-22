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
#ifndef IMAGE_H
#define IMAGE_H
#include "april/RenderSystem.h"

#include <string>

namespace AprilUI
{
	class Image
	{
	protected:
		April::TexturedVertex mVertices[4];
		April::Texture* mTexture;
		std::string mName;
		std::string mImageName;
		float mSourceX, mSourceY, mSourceW, mSourceH;
		April::BlendMode mBlendMode;
	public:
		
		Image(April::Texture* tex,std::string name,float sx,float sy,float sw,float sh,bool vertical=false);
		virtual void draw(float dx,float dy,float dw=-1,float dh=-1,float r=1,float g=1,float b=1,float a=1);
		virtual void draw(float centerx,float centery,float dw,float dh,float angle);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);

		April::Texture* getTexture();
		std::string getName() const { return mName; }
		std::string getImageName() const { return mImageName; }
		float getSourceX() const { return mSourceX; }
		float getSourceY() const { return mSourceY; }
		float getSourceW() const { return mSourceW; }
		float getSourceH() const { return mSourceH; }
		April::BlendMode getBlendMode() { return mBlendMode; }
		void setBlendMode(April::BlendMode mode) { mBlendMode=mode; }
	};

	class ColoredImage : public Image
	{
		float mRed,mGreen,mBlue,mAlpha;
	public:
		ColoredImage(April::Texture* tex,std::string name,float sx,float sy,float sw,float sh,bool vertical=false,unsigned int color=0xFFFFFF);
		void draw(float dx,float dy,float dw=-1,float dh=-1,float r=1,float g=1,float b=1,float a=1);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);

		float getAlpha() { return mAlpha; }
		void setAlpha(float alpha) { mAlpha=alpha; }
		void setColor(float a,float r,float g,float b);
	};

	class ColorImage : public Image
	{
		float mRed,mGreen,mBlue,mAlpha;
	public:
		ColorImage(std::string name);
		void draw(float dx,float dy,float dw,float dh,float r=1,float g=1,float b=1,float a=1);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
	};


	class NullImage : public Image
	{
	public:
		NullImage();
		void draw(float dx,float dy,float dw,float dh,float r=1,float g=1,float b=1,float a=1) {}
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a) {}
	};

}
#endif