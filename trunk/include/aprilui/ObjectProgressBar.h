/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a progress bar.

#ifndef APRILUI_PROGRESS_BAR_H
#define APRILUI_PROGRESS_BAR_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport ProgressBar : public ImageBox
	{
	public:
		enum Direction
		{
			Right = 0,
			Left = 1,
			Down = 2,
			Up = 3
		};

		ProgressBar(chstr name, grect rect);
		~ProgressBar();
		static Object* createInstance(chstr name, grect rect);
		
		Image* getProgressImage() { return this->mProgressImage; }
		void setProgressImage(Image* image);
		hstr getProgressImageName() { return this->mProgressImageName; }
		void setProgressImageByName(chstr image);
		Image* getMaskImage() { return this->mMaskImage; }
		void setMaskImage(Image* image);
		hstr getMaskImageName() { return this->mProgressImageName; }
		void setMaskImageByName(chstr image);
		float getProgress() { return this->mProgress; }
		void setProgress(float value) { this->mProgress = value; }
		bool isStretching() { return this->mStretching; }
		void setStretching(bool value) { this->mStretching = value; }
		Direction getDirection() { return this->mDirection; }
		void setDirection(Direction value) { this->mDirection = value; }
		
		/// @brief Optimized version.
		bool trySetProgressImageByName(chstr name);
		bool trySetMaskImageByName(chstr name);

		void OnDraw();

		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);

	protected:
		Image* mProgressImage;
		hstr mProgressImageName;
		Image* mMaskImage;
		hstr mMaskImageName;
		float mProgress;
		bool mStretching;
		Direction mDirection;

		grect _calcRectDirection(grect rect, float progress);
		
	};
}

#endif
