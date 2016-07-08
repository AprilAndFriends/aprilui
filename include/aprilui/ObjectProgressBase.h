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
#include "PropertyDescription.h"

namespace aprilui
{
	class Dataset;
	class BaseImage;

	class apriluiExport ProgressBase
	{
		APRILUI_CLONEABLE_ABSTRACT(ProgressBase);
	public:
		ProgressBase();
		virtual ~ProgressBase();

		HL_DEFINE_GET(BaseImage*, progressImage, ProgressImage);
		HL_DEFINE_GET(BaseImage*, antiProgressImage, AntiProgressImage);
		HL_DEFINE_GET(BaseImage*, maskImage, MaskImage);
		HL_DEFINE_GET(hstr, progressImageName, ProgressImageName);
		HL_DEFINE_GET(hstr, antiProgressImageName, AntiProgressImageName);
		HL_DEFINE_GET(hstr, maskImageName, MaskImageName);
		HL_DEFINE_GETSET(float, progress, Progress);
		void setProgressImage(BaseImage* image);
		void setAntiProgressImage(BaseImage* image);
		void setMaskImage(BaseImage* image);
		void setProgressImageByName(chstr name);
		void setAntiProgressImageByName(chstr name);
		void setMaskImageByName(chstr name);

		virtual Dataset* getDataset() const = 0;
		virtual BaseImage* getImage() const = 0;
		virtual void setImage(BaseImage* image) = 0;
		virtual hstr getImageName() const = 0;
		virtual void setImageByName(chstr name) = 0;

		virtual harray<PropertyDescription> getPropertyDescriptions() const;

		/// @brief Optimized version.
		bool trySetProgressImageByName(chstr name);
		bool trySetAntiProgressImageByName(chstr name);
		bool trySetMaskImageByName(chstr name);
		virtual bool trySetImageByName(chstr name) = 0;
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		BaseImage* progressImage;
		hstr progressImageName;
		BaseImage* antiProgressImage;
		hstr antiProgressImageName;
		BaseImage* maskImage;
		hstr maskImageName;
		float progress;
		
		virtual harray<BaseImage*> _getUsedImages() const;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
