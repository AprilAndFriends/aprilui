/// @file
/// @version 4.06
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
#include "ObjectImageBox.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	class BaseImage;

	class apriluiExport ProgressBar : public ImageBox, public ProgressBase
	{
		APRILUI_CLONEABLE(ProgressBar);
	public:
		enum Direction
		{
			Down = 2,
			Left = 4,
			Right = 6,
			Up = 8,
			DirectionMax = 10 // used for calculation
		};

		ProgressBar(chstr name);
		~ProgressBar();
		inline hstr getClassName() const { return "ProgressBar"; }

		static Object* createInstance(chstr name);
		
		HL_DEFINE_ISSET(stretching, Stretching);
		HL_DEFINE_GETSET(Direction, direction, Direction);
		HL_DEFINE_ISSET(interactable, Interactable);
		Dataset* getDataset();
		BaseImage* getImage();
		void setImage(BaseImage* image);
		hstr getImageName();
		void setImageByName(chstr name);
		harray<BaseImage*> getUsedImages();

		harray<PropertyDescription> getPropertyDescriptions();

		bool trySetImageByName(chstr name);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		bool stretching;
		Direction direction;
		bool interactable;
		bool pushed;

		grect _calcRectDirection(grect rect, float progress, Direction direction);
		
		void _update(float timeDelta);
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
