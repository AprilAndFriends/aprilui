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
/// Defines a progress circle.

#ifndef APRILUI_PROGRESS_CIRCLE_H
#define APRILUI_PROGRESS_CIRCLE_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"
#include "ObjectImageBox.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	class apriluiExport ProgressCircle : public ImageBox, public ProgressBase
	{
		APRILUI_CLONEABLE(ProgressCircle);
	public:
		enum Direction
		{
			Clockwise270 = 2,
			Clockwise180 = 4,
			Clockwise = 6,
			Clockwise90 = 8,
			Counterclockwise270 = 20,
			Counterclockwise180 = 40,
			Counterclockwise = 60,
			Counterclockwise90 = 80,
			DirectionLimit = 10, // used for calculation
		};

		ProgressCircle(chstr name);
		~ProgressCircle();
		inline hstr getClassName() const { return "ProgressCircle"; }

		static Object* createInstance(chstr name);
		
		HL_DEFINE_GETSET(Direction, direction, Direction);
		Dataset* getDataset() const;
		BaseImage* getImage() const;
		void setImage(BaseImage* image);
		hstr getImageName() const;
		void setImageByName(chstr name);

		harray<PropertyDescription> getPropertyDescriptions();

		bool trySetImageByName(chstr name);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		Direction direction;
		
		harray<BaseImage*> _getUsedImages() const;

		void _draw();
		harray<april::TexturedVertex> _calcVertices(grect rect, float progress, Direction direction);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
