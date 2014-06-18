/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
#include "ObjectProgressBase.h"

namespace aprilui
{
	class apriluiExport ProgressBar : public ImageBox, public ProgressBase
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
		hstr getClassName() const { return "ProgressBar"; }

		static Object* createInstance(chstr name, grect rect);
		
		HL_DEFINE_ISSET(stretching, Stretching);
		HL_DEFINE_GETSET(Direction, direction, Direction);
		Dataset* getDataset();
		Image* getImage();
		void setImage(Image* image);
		hstr getImageName();
		void setImageByName(chstr name);

		harray<PropertyDescription> getPropertyDescriptions();

		bool trySetImageByName(chstr name);

		void OnDraw();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		bool stretching;
		Direction direction;

		grect _calcRectDirection(grect rect, float progress);
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
