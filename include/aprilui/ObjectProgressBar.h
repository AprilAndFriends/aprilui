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
/// Defines a progress bar.

#ifndef APRILUI_PROGRESS_BAR_H
#define APRILUI_PROGRESS_BAR_H

#include <april/Keys.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageBox.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	class BaseImage;

	class apriluiExport ProgressBar : public ImageBox, public ProgressBase
	{
	public:
		enum Direction
		{
			Down = 2,
			Left = 4,
			Right = 6,
			Up = 8,
			DirectionMax = 10 // used for calculation
		};

		ProgressBar(chstr name, grect rect);
		~ProgressBar();

		HL_DEFINE_ISSET(stretching, Stretching);
		HL_DEFINE_GETSET(Direction, direction, Direction);
		Dataset* getDataset();
		Image* getImage();
		void setImage(Image* image);
		hstr getImageName();
		void setImageByName(chstr name);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void OnDraw(gvec2 offset);

	protected:
		bool stretching;
		Direction direction;

		grect _calcRectDirection(grect rect, float progress, Direction direction);

	};
}

#endif
