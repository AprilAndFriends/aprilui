/// @file
/// @version 3.5
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
	public:
		enum Direction
		{
			Clockwise = 0,
			Clockwise90 = 1,
			Clockwise180 = 2,
			Clockwise270 = 3,
			Counterclockwise = 4,
			Counterclockwise90 = 5,
			Counterclockwise180 = 6,
			Counterclockwise270 = 7
		};

		ProgressCircle(chstr name);
		~ProgressCircle();
		inline hstr getClassName() const { return "ProgressCircle"; }

		static Object* createInstance(chstr name);
		
		HL_DEFINE_GETSET(Direction, direction, Direction);
		Dataset* getDataset();
		Image* getImage();
		void setImage(Image* image);
		hstr getImageName();
		void setImageByName(chstr name);

		harray<PropertyDescription> getPropertyDescriptions();

		bool trySetImageByName(chstr name);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		Direction direction;
		
		void _draw();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
