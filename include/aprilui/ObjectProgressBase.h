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
/// Defines a base class for all objects with progress functionality.

#ifndef APRILUI_PROGRESS_BASE_H
#define APRILUI_PROGRESS_BASE_H

#include <april/Color.h>
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

		virtual harray<PropertyDescription> getPropertyDescriptions() const;

		HL_DEFINE_GET(BaseImage*, progressImage, ProgressImage);
		HL_DEFINE_GET(BaseImage*, antiProgressImage, AntiProgressImage);
		HL_DEFINE_GET(BaseImage*, maskImage, MaskImage);
		HL_DEFINE_GET(hstr, progressImageName, ProgressImageName);
		HL_DEFINE_GET(hstr, antiProgressImageName, AntiProgressImageName);
		HL_DEFINE_GET(hstr, maskImageName, MaskImageName);
		HL_DEFINE_GETSET(float, progress, Progress);
		HL_DEFINE_GETSET(april::Color, progressColor, ProgressColor);
		HL_DEFINE_GETSET(april::Color, antiProgressColor, AntiProgressColor);
		HL_DEFINE_GETSET(april::Color, maskColor, MaskColor);
		void setProgressImage(BaseImage* image);
		void setAntiProgressImage(BaseImage* image);
		void setMaskImage(BaseImage* image);
		void setProgressImageByName(chstr name);
		void setAntiProgressImageByName(chstr name);
		void setMaskImageByName(chstr name);
		void setProgressSymbolicColor(chstr value);
		void setAntiProgressSymbolicColor(chstr value);
		void setMaskSymbolicColor(chstr value);

		virtual Dataset* getDataset() const = 0;
		virtual BaseImage* getImage() const = 0;
		virtual void setImage(BaseImage* image) = 0;
		virtual hstr getImageName() const = 0;
		virtual void setImageByName(chstr name) = 0;

		/// @brief Optimized version.
		bool trySetProgressImageByName(chstr name);
		bool trySetAntiProgressImageByName(chstr name);
		bool trySetMaskImageByName(chstr name);
		virtual bool trySetImageByName(chstr name) = 0;
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
		HL_DEPRECATED("Deprecated API. Use setProgressSymbolicColor() instead.") void setSymbolicProgressColor(chstr value) { this->setProgressSymbolicColor(value); }
		HL_DEPRECATED("Deprecated API. Use setAntiProgressSymbolicColor() instead.") void setSymbolicAntiProgressColor(chstr value) { this->setAntiProgressSymbolicColor(value); }
		HL_DEPRECATED("Deprecated API. Use setMaskSymbolicColor() instead.") void setSymbolicMaskColor(chstr value) { this->setMaskSymbolicColor(value); }
#endif

	protected:
		BaseImage* progressImage;
		hstr progressImageName;
		BaseImage* antiProgressImage;
		hstr antiProgressImageName;
		BaseImage* maskImage;
		hstr maskImageName;
		float progress;
		april::Color progressColor;
		april::Color antiProgressColor;
		april::Color maskColor;
		
		virtual april::Color _makeDrawColor(const april::Color& color) const = 0;
		april::Color _makeDrawProgressColor() const;
		april::Color _makeDrawAntiProgressColor() const;
		april::Color _makeDrawMaskColor() const;
		
		virtual harray<BaseImage*> _getUsedImages() const;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
