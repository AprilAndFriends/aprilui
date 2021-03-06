/// @file
/// @version 5.1
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
		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, Direction,
		(
			HL_ENUM_DECLARE(Direction, Clockwise270);
			HL_ENUM_DECLARE(Direction, Clockwise180);
			HL_ENUM_DECLARE(Direction, Clockwise);
			HL_ENUM_DECLARE(Direction, Clockwise90);
			HL_ENUM_DECLARE(Direction, ClockwiseMax);
			HL_ENUM_DECLARE(Direction, Counterclockwise270);
			HL_ENUM_DECLARE(Direction, Counterclockwise180);
			HL_ENUM_DECLARE(Direction, Counterclockwise);
			HL_ENUM_DECLARE(Direction, Counterclockwise90);
			HL_ENUM_DECLARE(Direction, CounterclockwiseMax);
			int getAngle() const;
			bool isClockwise() const;
			bool isCounterclockwise() const;
		));

		ProgressCircle(chstr name);
		inline hstr getClassName() const { return "ProgressCircle"; }
		static Object* createInstance(chstr name);
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(Direction, direction, Direction);
		Dataset* getDataset() const;
		BaseImage* getImage() const;
		void setImage(BaseImage* image);
		hstr getImageName() const;
		void setImageByName(chstr name);

		bool trySetImageByName(chstr name);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		Direction direction;
		
		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		harray<BaseImage*> _getUsedImages() const;
		april::Color _makeDrawColor(const april::Color& color) const;

		void _draw();
		harray<april::TexturedVertex> _calcVertices(cgrectf rect, float progress, Direction direction);

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
