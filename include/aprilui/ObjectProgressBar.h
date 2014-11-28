/// @file
/// @version 4.0
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
		APRILUI_CLONEABLE(ProgressBar);
	public:
		enum Direction
		{
			Right = 0,
			Left = 1,
			Down = 2,
			Up = 3
		};

		ProgressBar(chstr name);
		~ProgressBar();
		inline hstr getClassName() const { return "ProgressBar"; }

		static Object* createInstance(chstr name);
		
		HL_DEFINE_ISSET(stretching, Stretching);
		HL_DEFINE_GETSET(Direction, direction, Direction);
		HL_DEFINE_ISSET(interactable, Interactable);
		Dataset* getDataset();
		Image* getImage();
		void setImage(Image* image);
		hstr getImageName();
		void setImageByName(chstr name);
		harray<Image*> getUsedImages();

		harray<PropertyDescription> getPropertyDescriptions();

		bool trySetImageByName(chstr name);

		void update(float timeDelta);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		bool stretching;
		Direction direction;
		bool interactable;
		bool pushed;

		grect _calcRectDirection(grect rect, float progress);
		
		void _draw();

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();

		void _updateInteractablePosition();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
