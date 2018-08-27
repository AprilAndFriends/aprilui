/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	ImageButton::ImageButton(chstr name, grectf rect) : ImageBox(name, rect)
	{
		_setTypeName("ImageButton");
		mPushed = false;
		mNormalImage = NULL;
		mPushedImage = NULL;
		mHoverImage = NULL;
		mDisabledImage = NULL;
	}

	ImageButton::ImageButton(chstr name) : ImageBox(name, grectf())// aprilui trunk compatibility
	{
		_setTypeName("ImageButton");
		mPushed = false;
		mNormalImage = NULL;
		mPushedImage = NULL;
		mHoverImage = NULL;
		mDisabledImage = NULL;
	}

	void ImageButton::OnDraw(gvec2f offset)
	{
		grectf rect = _getDrawRect() + offset;
		bool enabled = this->isDerivedEnabled();
		if (!enabled && mDisabledImage != NULL)
		{
			mDisabledImage->draw(rect);
			return;
		}
		bool hover = isCursorInside();
		if (mPushed && mPushedImage == NULL && hover)
		{
			april::Color color;
			color *= 0.7f;
			color.a = (unsigned char)(getDerivedAlpha() * 255);
			mImage->draw(rect, color);
			return;
		}
		ImageBox::OnDraw(offset);
		// the same thing for a hover image fallback solution
		if (enabled && hover && !mPushed && mHoverImage == NULL)
		{
			if (mImage != NULL)
			{
				Image* blendableImage = dynamic_cast<Image*>(mImage);
				if (blendableImage != NULL)
				{
					april::Color drawColor;
					drawColor.a = (unsigned char)(drawColor.a * 0.5f);
					april::BlendMode blendMode = blendableImage->getBlendMode();
					blendableImage->setBlendMode(april::BlendMode::Add);
					blendableImage->draw(rect, drawColor);
					blendableImage->setBlendMode(blendMode);
				}
			}
		}
	}

	void ImageButton::update(float k)
	{
		mImage = mNormalImage;
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		if (!isDerivedEnabled())
		{
			if (mDisabledImage != NULL)
			{
				mImage = mDisabledImage;
			}
		}
		else if (isCursorInside())
		{
			if (mPushed)
			{
				if (mPushedImage != NULL)
				{
					mImage = mPushedImage;
				}
			}
			else if (mHoverImage != NULL)
			{
				mImage = mHoverImage;
			}
		}
		ImageBox::update(k);
	}

	void ImageButton::setPushedImageByName(chstr image)
	{
		setPushedImage(mDataset->getImage(image));
	}

	void ImageButton::setHoverImageByName(chstr image)
	{
		setHoverImage(mDataset->getImage(image));
	}

	void ImageButton::setDisabledImageByName(chstr image)
	{
		setDisabledImage(mDataset->getImage(image));
	}

	void ImageButton::setImage(Image* image)
	{
		ImageBox::setImage(image);
		mNormalImage = image;
	}
	
	bool ImageButton::OnMouseDown(float x, float y, int button)
	{
		if (isCursorInside())
		{
			mPushed = true;
			triggerEvent("MouseDown", x, y, 0);
			return true;
		}
		if (ImageBox::OnMouseDown(x, y, button))
		{
			return true;
		}
		return false;
	}

	bool ImageButton::OnMouseUp(float x, float y, int button)
	{
		if (mPushed && isCursorInside())
		{
			mPushed = false;
			triggerEvent("Click", x, y, 0);
			return true;
		}
		mPushed = false;
		if (ImageBox::OnMouseUp(x, y, button))
		{
			return true;
		}
		return false;
	}

	bool ImageButton::OnMouseMove(float x, float y)
	{
		return ImageBox::OnMouseMove(x, y);
	}

	bool ImageButton::setProperty(chstr name, chstr value)
	{
		ImageBox::setProperty(name, value);
		if		(name == "pushed_image")	setPushedImage(mDataset->getImage(value));
		else if (name == "hover_image")		setHoverImage(mDataset->getImage(value));
		else if (name == "disabled_image")	setDisabledImage(mDataset->getImage(value));
		return 1;
	}
	
}
