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
/// Defines an image button.

#ifndef APRILUI_IMAGE_BUTTON_H
#define APRILUI_IMAGE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"
#include "ObjectButtonBase.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport ImageButton : public ImageBox, public ButtonBase
	{
		APRILUI_CLONEABLE(ImageButton);
	public:
		ImageButton(chstr name);
		inline hstr getClassName() const { return "ImageButton"; }
		static Object* createInstance(chstr name);
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(BaseImage*, hoverImage, HoverImage);
		HL_DEFINE_GET(BaseImage*, pushedImage, PushedImage);
		HL_DEFINE_GET(BaseImage*, disabledImage, DisabledImage);
		HL_DEFINE_GET(hstr, hoverImageName, HoverImageName);
		HL_DEFINE_GET(hstr, pushedImageName, PushedImageName);
		HL_DEFINE_GET(hstr, disabledImageName, DisabledImageName);
		HL_DEFINE_GET(BaseImage*, normalImage, Image);
		HL_DEFINE_GET(float, hoverAddFactor, HoverAddFactor);
		void setHoverAddFactor(const float& value);
		HL_DEFINE_GET(float, pushedMultiplyFactor, PushedMultiplyFactor);
		void setPushedMultiplyFactor(const float& value);
		HL_DEFINE_GET(float, hoverAddFadeSpeed, HoverAddFadeSpeed);
		void setHoverAddFadeSpeed(const float& value);
		HL_DEFINE_GET(float, pushedMultiplyFadeSpeed, PushedMultiplyFadeSpeed);
		void setPushedMultiplyFadeSpeed(const float& value);
		void setImage(BaseImage* value);
		hstr getName() const;
		int getFocusIndex() const;
		Object* getParent() const;
		Dataset* getDataset() const;
		bool isCursorInside() const;
		void setHoverImage(BaseImage* image);
		void setPushedImage(BaseImage* image);
		void setDisabledImage(BaseImage* name);
		void setHoverImageByName(chstr name);
		void setPushedImageByName(chstr name);
		void setDisabledImageByName(chstr name);
		
		/// @brief Optimized version.
		bool trySetHoverImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetPushedImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetDisabledImageByName(chstr name);

		bool isPointInside(cgvec2f position) const;

		void notifyEvent(chstr type, EventArgs* args);
		
		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		BaseImage* normalImage;
		BaseImage* hoverImage;
		BaseImage* pushedImage;
		BaseImage* disabledImage;
		hstr normalImageName;
		hstr hoverImageName;
		hstr pushedImageName;
		hstr disabledImageName;
		float hoverAddFactor;
		float pushedMultiplyFactor;
		float hoverAddFadeSpeed;
		float pushedMultiplyFadeSpeed;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		harray<BaseImage*> _getUsedImages() const;

		void _update(float timeDelta);
		void _updateImage(int* hoveredDirection = NULL, int* pushedDirection = NULL);
		void _draw();
		
		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _touchDown(int index);
		bool _touchUp(int index);
		void _touchCancel(int index);
		bool _touchMove(int index);
		bool _buttonDown(april::Button buttonCode);
		bool _buttonUp(april::Button buttonCode);
		
	private:
		float _hoverAddTime;
		float _pushedMultiplyTime;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
