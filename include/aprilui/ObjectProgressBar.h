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
		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, Direction,
		(
			HL_ENUM_DECLARE(Direction, Down);
			HL_ENUM_DECLARE(Direction, Left);
			HL_ENUM_DECLARE(Direction, Right);
			HL_ENUM_DECLARE(Direction, Up);
			HL_ENUM_DECLARE(Direction, Max);
		));

		ProgressBar(chstr name);
		~ProgressBar();
		inline hstr getClassName() const { return "ProgressBar"; }

		static Object* createInstance(chstr name);
		
		HL_DEFINE_ISSET(stretching, Stretching);
		HL_DEFINE_GETSET(Direction, direction, Direction);
		HL_DEFINE_ISSET(interactable, Interactable);
		Dataset* getDataset() const;
		BaseImage* getImage() const;
		void setImage(BaseImage* image);
		hstr getImageName() const;
		void setImageByName(chstr name);

		harray<PropertyDescription> getPropertyDescriptions() const;

		bool trySetImageByName(chstr name);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		bool stretching;
		Direction direction;
		bool interactable;
		bool pushed;

		harray<BaseImage*> _getUsedImages() const;

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
