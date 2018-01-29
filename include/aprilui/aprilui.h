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
/// Defines all functions used in aprilui.

#ifndef APRILUI_H
#define APRILUI_H

#include <april/Keys.h>
#include <april/Texture.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Event.h"

#define APRILUI_REGISTER_OBJECT_TYPE(name) aprilui::registerObjectFactory(name("").getClassName(), &name::createInstance)
#define APRILUI_REGISTER_ANIMATOR_TYPE(name) aprilui::registerAnimatorFactory(name("").getClassName(), &name::createInstance)
#define APRILUI_REGISTER_IMAGE_TYPE(name) aprilui::registerImageFactory(name(NULL, "", grect()).getClassName(), &name::createInstance)

namespace aprilui
{
	class Animator;
	class BaseImage;
	class Dataset;
	class MinimalImage;
	class Object;
	class Texture;

	static const char SeparatorParameter = ',';
	
	apriluiExport extern hstr logTag;

	apriluiFnExport void init();
	apriluiFnExport void destroy();
	
	apriluiFnExport bool isDebugEnabled();
	apriluiFnExport void setDebugEnabled(bool value);
	
	apriluiFnExport grect getViewport();
	apriluiFnExport void setViewport(cgrect value);
	apriluiFnExport bool isLimitCursorToViewport();
	apriluiFnExport void setLimitCursorToViewport(bool value);
	apriluiFnExport bool isHoverEffectEnabled();
	apriluiFnExport void setHoverEffectEnabled(bool value);
	apriluiFnExport hstr getDefaultTextsPath();
	apriluiFnExport void setDefaultTextsPath(chstr value);
	apriluiFnExport hstr getDefaultLocalization();
	apriluiFnExport void setDefaultLocalization(chstr value);
	apriluiFnExport hstr getLocalization();
	apriluiFnExport void setLocalization(chstr value);
	apriluiFnExport harray<hstr> getSupportedLocalizations();
	apriluiFnExport void setSupportedLocalizations(const harray<hstr>& value);
	apriluiFnExport float getTextureIdleUnloadTime();
	apriluiFnExport void setTextureIdleUnloadTime(float value);
	apriluiFnExport bool isDefaultManagedTextures();
	apriluiFnExport void setDefaultManagedTextures(bool value);
	apriluiFnExport april::Texture::LoadMode getDefaultTextureLoadMode();
	apriluiFnExport void setDefaultTextureLoadMode(april::Texture::LoadMode value);
	apriluiFnExport bool isUseKeyboardAutoOffset();
	apriluiFnExport void setUseKeyboardAutoOffset(bool value);
	apriluiFnExport hmap<hstr, Dataset*> getDatasets();
	apriluiFnExport harray<Texture*> getTextures();

	apriluiFnExport void registerObjectFactory(chstr typeName, Object* (*factory)(chstr));
	apriluiFnExport void registerAnimatorFactory(chstr typeName, Animator* (*factory)(chstr));
	apriluiFnExport void registerImageFactory(chstr typeName, MinimalImage* (*factory)(Texture*, chstr, cgrect));
	apriluiFnExport bool hasObjectFactory(chstr typeName);
	apriluiFnExport bool hasAnimatorFactory(chstr typeName);
	apriluiFnExport bool hasImageFactory(chstr typeName);
	apriluiFnExport void unregisterObjectFactory(chstr typeName);
	apriluiFnExport void unregisterAnimatorFactory(chstr typeName);
	apriluiFnExport void unregisterImageFactory(chstr typeName);
	apriluiFnExport const hmap<hstr, Object* (*)(chstr)>& getObjectFactories();
	apriluiFnExport const hmap<hstr, Animator* (*)(chstr)>& getAnimatorFactories();
	apriluiFnExport const hmap<hstr, MinimalImage* (*)(Texture*, chstr, cgrect)>& getImageFactories();

	apriluiFnExport Object* createObject(chstr type, chstr name);
	apriluiFnExport Animator* createAnimator(chstr type, chstr name);
	apriluiFnExport MinimalImage* createImage(chstr type, Texture* texture, chstr name, cgrect source);

	apriluiFnExport gvec2 transformWindowPoint(cgvec2 point);
	apriluiFnExport void updateViewportPosition(cgrect newViewport, bool updateOrthoProjection = true);
	apriluiFnExport void updateCursorPosition();
	apriluiFnExport gvec2 getCursorPosition();
	apriluiFnExport void setCursorPosition(cgvec2 position);
	apriluiFnExport void setCursorImage(BaseImage* image);
	apriluiFnExport void showCursor();
	apriluiFnExport void hideCursor();
	apriluiFnExport void drawCursor();
	/// @brief Allows certain exception types to be disabled.
	/// @param[in] textureFiles Exceptions related to proper loading of textures.
	/// @param[in] childManipulation Exceptions related to proper handling of BaseObject children.
	/// @param[in] creationFactories Exceptions related to proper handling of Object and Animator factories.
	/// @param[in] objectExistence Exceptions related to proper handling when certain dataset, objects, images, textures, styles, etc. exist or not.
	/// @param[in] systemConsistency Exceptions related to other aspects of the system.
	/// @note This is only enabled when compiling as debug and should only be used as debug feature and to prevent crashes when having incomplete data.
	apriluiFnExport void setDebugExceptionsEnabled(bool textureFiles, bool childManipulation, bool creationFactories, bool objectExistence, bool systemConsistency);

	apriluiFnExport Dataset* getDatasetByName(chstr name);
	
	apriluiFnExport void notifyEvent(chstr type, EventArgs* args);
	apriluiFnExport void processEvents();
	apriluiFnExport void update(float timeDelta);
	apriluiFnExport void updateTextures(float timeDelta);
	apriluiFnExport void unloadUnusedResources();
	apriluiFnExport void reloadTextures();

	apriluiFnExport bool setTextureExtensionPrefixes(harray<hstr> prefixes, harray<float> scales);
	apriluiFnExport void setTextureExtensionScales(harray<hstr> extensions, harray<float> scales);
	apriluiFnExport float getTextureExtensionScale(chstr extension);
	apriluiFnExport float findTextureExtensionScale(chstr filename);
	
	apriluiFnExport void onMouseDown(april::Key keyCode);
	apriluiFnExport void onMouseUp(april::Key keyCode);
	apriluiFnExport void onMouseCancel(april::Key keyCode);
	apriluiFnExport void onMouseMove();
	apriluiFnExport void onMouseScroll(float x, float y);
	apriluiFnExport void onKeyDown(april::Key keyCode);
	apriluiFnExport void onKeyUp(april::Key keyCode);
	apriluiFnExport void onChar(unsigned int charCode);
	apriluiFnExport void onTouch(const harray<gvec2>& touches);
	apriluiFnExport void onButtonDown(april::Button buttonCode);
	apriluiFnExport void onButtonUp(april::Button buttonCode);

}
#endif
