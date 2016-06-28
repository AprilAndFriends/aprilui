/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for all objects with progress functionality.

#ifndef APRILUI_PROGRESS_BASE_H
#define APRILUI_PROGRESS_BASE_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	class Image;

	class apriluiExport ProgressBase
	{
	public:
		ProgressBase();
		virtual ~ProgressBase();

		HL_DEFINE_GET(Image*, progressImage, ProgressImage);
		HL_DEFINE_GET(Image*, antiProgressImage, AntiProgressImage);
		HL_DEFINE_GET(Image*, maskImage, MaskImage);
		HL_DEFINE_GET(hstr, progressImageName, ProgressImageName);
		HL_DEFINE_GET(hstr, antiProgressImageName, AntiProgressImageName);
		HL_DEFINE_GET(hstr, maskImageName, MaskImageName);
		HL_DEFINE_GETSET(float, progress, Progress);
		void setProgressImage(Image* image);
		void setAntiProgressImage(Image* image);
		void setMaskImage(Image* image);
		void setProgressImageByName(chstr name);
		void setAntiProgressImageByName(chstr name);
		void setMaskImageByName(chstr name);

		virtual Dataset* getDataset() = 0;
		virtual Image* getImage() = 0;
		virtual void setImage(Image* image) = 0;
		virtual hstr getImageName() = 0;
		virtual void setImageByName(chstr name) = 0;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		Image* progressImage;
		hstr progressImageName;
		Image* antiProgressImage;
		hstr antiProgressImageName;
		Image* maskImage;
		hstr maskImageName;
		float progress;
		
	};
}

#endif
