/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>

#include <april/april.h>
#include <april/Keys.h>
#include <april/Platform.h>
#include <april/RenderSystem.h>
#include <april/Window.h>
#include <atres/atres.h>
#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "Images.h"
#include "Objects.h"
#include "Texture.h"

#define REGISTER_ANIMATOR_TYPE(name) aprilui::registerAnimatorFactory(#name, &Animators::name::createInstance)

namespace aprilui
{
	hstr logTag = "aprilui";

	static hversion version(5, 1, 0);

	bool _datasetRegisterLock = false; // not static, because it is used elsewhere
	static hmap<hstr, Dataset*> datasets;
	static Dataset* colorDataset = NULL;
	static harray<Texture*> textures;
	static hmutex texturesMutex;
	static hmap<hstr, Object* (*)(chstr)> objectFactories;
	static hmap<hstr, Animator* (*)(chstr)> animatorFactories;
	static hmap<hstr, MinimalImage* (*)(Texture*, chstr, cgrectf)> imageFactories;
	static BaseImage* cursorImage = NULL;
	static bool cursorVisible = true;
	static gvec2f cursorPosition;
	static bool limitCursorToViewport = false;
	static bool hoverEffectEnabled = true;
	static grectf viewport;
	static bool debugEnabled = false;
	static hstr defaultTextsPath = "texts";
	static hstr defaultLocalization = "";
	static hstr localization = "";
	static harray<hstr> supportedLocalizations;
	static float textureIdleUnloadTime = 0.0f;
	static bool defaultManagedTextures = false;
	static april::Texture::LoadMode defaultTextureLoadMode = april::Texture::LoadMode::Async;
	static bool useKeyboardAutoOffset = false;
	static hmap<hstr, float> extensionScales;

	void init()
	{
		hlog::write(logTag, "Initializing AprilUI: " + version.toString());
		_datasetRegisterLock = false;
		cursorVisible = true;
		limitCursorToViewport = true;
		hoverEffectEnabled = true;
		debugEnabled = false;
		defaultTextsPath = "texts";
		localization = "";
		textureIdleUnloadTime = 0.0f;
		defaultManagedTextures = false;
		defaultTextureLoadMode = april::Texture::LoadMode::Async;
		viewport.setSize(april::window->getSize());
		ButtonBase::allowedKeys.clear();
		ButtonBase::allowedKeys += april::Key::MouseL;
		ButtonBase::allowedButtons.clear();
		ButtonBase::allowedButtons += april::Button::A;
		colorDataset = new Dataset("", COLOR_DATASET_NAME);
		colorDataset->load();

		APRILUI_REGISTER_OBJECT_TYPE(CallbackObject);
		APRILUI_REGISTER_OBJECT_TYPE(Container);
		APRILUI_REGISTER_OBJECT_TYPE(EditBox);
		APRILUI_REGISTER_OBJECT_TYPE(FilledRect);
		APRILUI_REGISTER_OBJECT_TYPE(GridView);
		APRILUI_REGISTER_OBJECT_TYPE(GridViewCell);
		APRILUI_REGISTER_OBJECT_TYPE(GridViewRow);
		APRILUI_REGISTER_OBJECT_TYPE(GridViewRowTemplate);
		APRILUI_REGISTER_OBJECT_TYPE(ImageBox);
		APRILUI_REGISTER_OBJECT_TYPE(ImageButton);
		APRILUI_REGISTER_OBJECT_TYPE(Label);
		APRILUI_REGISTER_OBJECT_TYPE(ListBox);
		APRILUI_REGISTER_OBJECT_TYPE(ListBoxItem);
		APRILUI_REGISTER_OBJECT_TYPE(OptionButton);
		APRILUI_REGISTER_OBJECT_TYPE(ProgressBar);
		APRILUI_REGISTER_OBJECT_TYPE(ProgressCircle);
		APRILUI_REGISTER_OBJECT_TYPE(Rect);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollArea);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonBackground);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonBackward);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonForward);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarButtonSlider);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarH);
		APRILUI_REGISTER_OBJECT_TYPE(ScrollBarV);
		APRILUI_REGISTER_OBJECT_TYPE(TreeView);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewExpander);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewImage);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewLabel);
		APRILUI_REGISTER_OBJECT_TYPE(TreeViewNode);
		APRILUI_REGISTER_OBJECT_TYPE(TextImageButton);
		APRILUI_REGISTER_OBJECT_TYPE(ToggleButton);

		REGISTER_ANIMATOR_TYPE(AlphaChanger);
		REGISTER_ANIMATOR_TYPE(BlueChanger);
		REGISTER_ANIMATOR_TYPE(FrameAnimation);
		REGISTER_ANIMATOR_TYPE(GreenChanger);
		REGISTER_ANIMATOR_TYPE(MoverX);
		REGISTER_ANIMATOR_TYPE(MoverY);
		REGISTER_ANIMATOR_TYPE(ProgressChanger);
		REGISTER_ANIMATOR_TYPE(RedChanger);
		REGISTER_ANIMATOR_TYPE(ResizerX);
		REGISTER_ANIMATOR_TYPE(ResizerY);
		REGISTER_ANIMATOR_TYPE(Rotator);
		REGISTER_ANIMATOR_TYPE(ScalerX);
		REGISTER_ANIMATOR_TYPE(ScalerY);
		REGISTER_ANIMATOR_TYPE(TileScrollerX);
		REGISTER_ANIMATOR_TYPE(TileScrollerY);
		REGISTER_ANIMATOR_TYPE(ZOrderChanger);

		APRILUI_REGISTER_IMAGE_TYPE(MinimalImage);
		APRILUI_REGISTER_IMAGE_TYPE(Image);
		APRILUI_REGISTER_IMAGE_TYPE(CropImage);
		APRILUI_REGISTER_IMAGE_TYPE(SkinImage);
		APRILUI_REGISTER_IMAGE_TYPE(TileImage);
	}
	
	void destroy()
	{
		hlog::write(logTag, "Destroying AprilUI.");
		_datasetRegisterLock = true;
		foreach_m (Dataset*, it, datasets)
		{
			delete it->second;
		}
		datasets.clear();
		colorDataset = NULL;
		objectFactories.clear();
		animatorFactories.clear();
		imageFactories.clear();
		cursorImage = NULL;
	}
	
	bool isDebugEnabled()
	{
		return debugEnabled;
	}
	
	void setDebugEnabled(bool value)
	{
		debugEnabled = value;
	}
	
	grectf getViewport()
	{
		return viewport;
	}
	
	void setViewport(cgrectf value)
	{
		viewport = value;
	}
	
	bool isLimitCursorToViewport()
	{
		return limitCursorToViewport;
	}
	
	void setLimitCursorToViewport(bool value)
	{
		limitCursorToViewport = value;
	}
	
	bool isHoverEffectEnabled()
	{
		return hoverEffectEnabled;
	}
	
	void setHoverEffectEnabled(bool value)
	{
		hoverEffectEnabled = value;
	}
	
	hstr getDefaultTextsPath()
	{
		return defaultTextsPath;
	}
	
	void setDefaultTextsPath(chstr value)
	{
		defaultTextsPath = value;
	}
	
	hstr getDefaultLocalization()
	{
		return defaultLocalization;
	}
	
	void setDefaultLocalization(chstr value)
	{
		defaultLocalization = value;
	}
	
	hstr getLocalization()
	{
		return localization;
	}
	
	void setLocalization(chstr value)
	{
		hlog::write(logTag, "Setting localization to: " + value);
		hstr previousLocalization = localization;
		if (supportedLocalizations.size() > 0 && !supportedLocalizations.has(value) && value != defaultLocalization)
		{
			hlog::warnf(logTag, "Localization '%s' not supported, defaulting back to '%s'.", value.cStr(), defaultLocalization.cStr());
			localization = defaultLocalization;
		}
		else
		{
			localization = value;
		}
		if (previousLocalization != localization)
		{
			foreach_m (Dataset*, it, datasets)
			{
				if (it->second->isLoaded())
				{
					it->second->reloadTexts();
					it->second->reloadTextures();
				}
			}
			// finished localization change, now call the appropriate event
			foreach_m (Dataset*, it, datasets)
			{
				if (it->second->isLoaded())
				{
					it->second->notifyEvent(Event::LocalizationChanged, NULL);
				}
			}
		}
	}

	harray<hstr> getSupportedLocalizations()
	{
		return supportedLocalizations;
	}
	
	void setSupportedLocalizations(const harray<hstr>& value)
	{
		supportedLocalizations = value;
	}
	
	float getTextureIdleUnloadTime()
	{
		return textureIdleUnloadTime;
	}
	
	void setTextureIdleUnloadTime(float value)
	{
		textureIdleUnloadTime = value;
	}

	bool isDefaultManagedTextures()
	{
		return defaultManagedTextures;
	}

	void setDefaultManagedTextures(bool value)
	{
		defaultManagedTextures = value;
	}

	april::Texture::LoadMode getDefaultTextureLoadMode()
	{
		return defaultTextureLoadMode;
	}

	void setDefaultTextureLoadMode(april::Texture::LoadMode value)
	{
		defaultTextureLoadMode = value;
	}

	bool isUseKeyboardAutoOffset()
	{
		return useKeyboardAutoOffset;
	}

	void setUseKeyboardAutoOffset(bool value)
	{
		useKeyboardAutoOffset = value;
	}

	hmap<hstr, Dataset*> getDatasets()
	{
		return datasets;
	}

	harray<Texture*> getTextures()
	{
		hmutex::ScopeLock lock(&texturesMutex);
		return textures;
	}
	
	void registerObjectFactory(chstr typeName, Object* (*factory)(chstr))
	{
		if (objectFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryExistsException("Object", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		objectFactories[typeName] = factory;
		// create all important property meta data here already to avoid issues with multi-threaded first-time setup
		Object* temp = (*factory)("temp");
		temp->getPropertyDescriptions();
		temp->getProperty("name");
		temp->setProperty("name", "");
		delete temp;
	}
	
	void registerAnimatorFactory(chstr typeName, Animator* (*factory)(chstr))
	{
		if (animatorFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryExistsException("Animator", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		animatorFactories[typeName] = factory;
		// create all important property meta data here already to avoid issues with multi-threaded first-time setup
		Animator* temp = (*factory)("temp");
		temp->getPropertyDescriptions();
		temp->getProperty("name");
		temp->setProperty("name", "");
		delete temp;
	}
	
	void registerImageFactory(chstr typeName, MinimalImage* (*factory)(Texture*, chstr, cgrectf))
	{
		if (imageFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryExistsException("Image", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		imageFactories[typeName] = factory;
		// create all important property meta data here already to avoid issues with multi-threaded first-time setup
		MinimalImage* temp = (*factory)(NULL, "temp", grectf(0.0f, 0.0f, 0.0f, 0.0f));
		temp->getPropertyDescriptions();
		temp->getProperty("name");
		temp->setProperty("name", "");
		delete temp;
	}

	void unregisterObjectFactory(chstr typeName)
	{
		if (!objectFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryNotExistsException("Object", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		objectFactories.removeKey(typeName);
	}
	
	void unregisterAnimatorFactory(chstr typeName)
	{
		if (!animatorFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryNotExistsException("Animator", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		animatorFactories.removeKey(typeName);
	}

	void unregisterImageFactory(chstr typeName)
	{
		if (!imageFactories.hasKey(typeName))
		{
			__THROW_EXCEPTION(ObjectFactoryNotExistsException("Image", typeName), aprilui::creationFactoriesDebugExceptionsEnabled, return);
		}
		imageFactories.removeKey(typeName);
	}

	const hmap<hstr, Object* (*)(chstr)>& getObjectFactories()
	{
		return objectFactories;
	}

	const hmap<hstr, Animator* (*)(chstr)>& getAnimatorFactories()
	{
		return animatorFactories;
	}

	const hmap<hstr, MinimalImage* (*)(Texture*, chstr, cgrectf)>& getImageFactories()
	{
		return imageFactories;
	}

	bool hasObjectFactory(chstr typeName)
	{
		return objectFactories.hasKey(typeName);
	}
	
	bool hasAnimatorFactory(chstr typeName)
	{
		return animatorFactories.hasKey(typeName);
	}
	
	bool hasImageFactory(chstr typeName)
	{
		return imageFactories.hasKey(typeName);
	}

	Object* createObject(chstr typeName, chstr name)
	{
		return (objectFactories.hasKey(typeName) ? (*objectFactories[typeName])(name) : NULL);
	}
	
	Animator* createAnimator(chstr typeName, chstr name)
	{
		return (animatorFactories.hasKey(typeName) ? (*animatorFactories[typeName])(name) : NULL);
	}
	
	MinimalImage* createImage(chstr typeName, Texture* texture, chstr name, cgrectf source)
	{
		return (imageFactories.hasKey(typeName) ? (*imageFactories[typeName])(texture, name, source) : NULL);
	}

	bool hasImage(chstr imageName, Dataset* defaultDataset)
	{
		if (defaultDataset != NULL && defaultDataset->hasImage(imageName))
		{
			return true;
		}
		int dotIndex = imageName.indexOf('.');
		if (dotIndex < 0)
		{
			return false;
		}
		hstr datasetName = imageName(0, dotIndex);
		return (datasets.hasKey(datasetName) && aprilui::getDatasetByName(datasetName)->hasImage(imageName(dotIndex + 1, -1)));
	}

	gvec2f transformWindowPoint(cgvec2f point)
	{
		gvec2f result;
		result.x = (float)(int)(point.x * viewport.w / april::window->getWidth()) - viewport.x;
		result.y = (float)(int)(point.y * viewport.h / april::window->getHeight()) - viewport.y;
		if (limitCursorToViewport)
		{
			result.x = hclamp(result.x, 0.0f, viewport.w - 1);
			result.y = hclamp(result.y, 0.0f, viewport.h - 1);
		}
		return result;
	}

	void updateViewportPosition(cgrectf newViewport, bool updateOrthoProjection)
	{
		viewport = newViewport;
		bool keyboardVisible = (useKeyboardAutoOffset && april::window->isVirtualKeyboardVisible());
		if (!keyboardVisible)
		{
			if (updateOrthoProjection)
			{
				april::rendersys->setOrthoProjection(viewport);
			}
			return;
		}
		float viewportHeight = viewport.h;
		float visibleHeight = (float)(int)(viewportHeight * (1.0f - april::window->getVirtualKeyboardHeightRatio()));
		float keyboardHeight = viewportHeight - visibleHeight;
		aprilui::Object* object = NULL;
		aprilui::EditBox* editBox = NULL;
		grectf rect;
		float h = 0.0f;
		foreach_m (Dataset*, it, datasets)
		{
			object = it->second->getFocusedObject();
			if (object != NULL)
			{
				rect = object->getBoundingRect();
				h = (float)(int)(rect.y + rect.h * 0.5f);
				editBox = dynamic_cast<aprilui::EditBox*>(object);
				if (editBox != NULL)
				{
					h = editBox->getDerivedPoint(editBox->getCaretRect().getPosition()).y;
				}
				viewport.y = hclamp(-h + (float)(int)(visibleHeight * 0.5f), -keyboardHeight, 0.0f);
				break;
			}
		}
		if (updateOrthoProjection)
		{
			april::rendersys->setOrthoProjection(viewport);
		}
	}
	
	void updateCursorPosition()
	{
		cursorPosition = transformWindowPoint(april::window->getCursorPosition());
	}
	
	void setCursorPosition(cgvec2f position)
	{
		cursorPosition = position;
	}
	
	gvec2f getCursorPosition()
	{
		return cursorPosition;
	}
	
	void setCursorImage(BaseImage* image)
	{
		cursorImage = image;
	}
	
	void showCursor()
	{
		cursorVisible = true;
	}
	
	void hideCursor()
	{
		cursorVisible = false;
	}
	
	void drawCursor()
	{
		if (cursorImage != NULL && cursorVisible)
		{
			cursorImage->draw(grectf(getCursorPosition(), cursorImage->getSrcSize()));
		}
	}

	void getNotchOffsets(gvec2i& topLeft, gvec2i& bottomRight)
	{
		aprilui::getNotchOffsets(topLeft, bottomRight, (viewport.getAspect() > 1.0f));
	}

	void getNotchOffsets(gvec2i& topLeft, gvec2i& bottomRight, bool landscape)
	{
		april::getNotchOffsets(topLeft, bottomRight, landscape);
		gvec2f ratio = viewport.getSize() / april::window->getSize();
		topLeft.set(hceil(topLeft.x * ratio.x), hceil(topLeft.y * ratio.y));
		bottomRight.set(hceil(bottomRight.x * ratio.x), hceil(bottomRight.y * ratio.y));
	}

	void setDebugExceptionsEnabled(bool textureFiles, bool childManipulation, bool creationFactories, bool objectExistence, bool systemConsistency)
	{
		hlog::warn(logTag, "Setting debug-exceptions!");
		aprilui::textureFilesDebugExceptionsEnabled = textureFiles;
		aprilui::childManipulationDebugExceptionsEnabled = childManipulation;
		aprilui::creationFactoriesDebugExceptionsEnabled = creationFactories;
		aprilui::objectExistenceDebugExceptionsEnabled = objectExistence;
		aprilui::systemConsistencyDebugExceptionsEnabled = systemConsistency;
	}
	
	Dataset* getDatasetByName(chstr name)
	{
		if (!datasets.hasKey(name))
		{
			__THROW_EXCEPTION(ObjectNotExistsException("Dataset", name, name), aprilui::objectExistenceDebugExceptionsEnabled, return NULL);
		}
		return datasets[name];
	}
	
	void _registerDataset(chstr name, Dataset* dataset)
	{
		if (!_datasetRegisterLock)
		{
			if (datasets.hasKey(name))
			{
				__THROW_EXCEPTION(ObjectExistsException("Dataset", name, name), aprilui::objectExistenceDebugExceptionsEnabled, return);
			}
			datasets[name] = dataset;
		}
	}
	
	void _unregisterDataset(chstr name, Dataset* dataset)
	{
		if (!_datasetRegisterLock)
		{
			datasets.removeKey(name);
		}
	}
	
	void _registerTexture(Texture* texture)
	{
		hmutex::ScopeLock lock(&texturesMutex);
		textures += texture;
	}

	void _unregisterTexture(Texture* texture)
	{
		hmutex::ScopeLock lock(&texturesMutex);
		textures -= texture;
	}

	void notifyEvent(chstr type, EventArgs* args)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->notifyEvent(type, args);
		}
	}

	void processEvents()
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->processEvents();
		}
	}

	void update(float timeDelta)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->update(timeDelta);
		}
	}
	
	void updateTextures(float timeDelta)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->updateTextures(timeDelta);
		}
	}
	
	void clearChildUnderCursor()
	{
		foreach_m(Dataset*, it, datasets)
		{
			it->second->clearChildUnderCursor();
		}
	}

	void unloadUnusedResources()
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->unloadUnusedResources();
		}
	}

	void reloadTextures()
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->reloadTextures();
		}
	}
	
	bool setTextureExtensionPrefixes(harray<hstr> prefixes, harray<float> scales)
	{
		if (prefixes.size() != scales.size())
		{
			hlog::warn(logTag, "setTextureExtensionPrefixes() called with unmatching 'prefixes' and 'scales' sizes.");
			return false;
		}
		harray<hstr> currentExtensions = april::getTextureExtensions();
		// removing all non-default extensions
		HL_LAMBDA_CLASS(_defaultExtensions, bool, ((hstr const& extension) { return (extension.count('.') <= 1); }));
		harray<hstr> defaultExtensions = currentExtensions.findAll(&_defaultExtensions::lambda);
		// adding new extensions and scales
		harray<hstr> newExtensions;
		harray<float> newScales;
		for_iter (i, 0, prefixes.size())
		{
			foreach (hstr, it, defaultExtensions)
			{
				newExtensions += prefixes[i] + (*it);
				newScales += scales[i];
			}
		}
		foreach (hstr, it, defaultExtensions)
		{
			newExtensions += (*it);
			newScales += 1.0f;
		}
		hmap<hstr, float> newExtensionScales;
		harray<hstr> logs;
		for_iter (i, 0, newExtensions.size())
		{
			newExtensionScales[newExtensions[i]] = newScales[i];
			logs += hsprintf("%s:%g", newExtensions[i].cStr(), newScales[i]);
		}
		// if extension scales have not changed
		if (extensionScales != newExtensionScales)
		{
			hlog::write(logTag, "Setting texture extension scales:\n" + logs.joined('\n'));
			extensionScales = newExtensionScales;
			april::setTextureExtensions(newExtensions);
			return true;
		}
		return false;
	}
	
	void setTextureExtensionScales(harray<hstr> extensions, harray<float> scales)
	{
		while (extensions.size() > scales.size())
		{
			scales += 1.0f;
		}
		extensionScales.clear();
		for_iter (i, 0, extensions.size())
		{
			extensionScales[extensions[i]] = scales[i];
		}
		april::setTextureExtensions(extensions);
	}
	
	float getTextureExtensionScale(chstr extension)
	{
		return (extensionScales.hasKey(extension) ? extensionScales[extension] : 1.0f);
	}
	
	float findTextureExtensionScale(chstr filename)
	{
		harray<hstr> extensions = april::getTextureExtensions();
		foreach (hstr, it, extensions)
		{
			if (filename.endsWith(*it))
			{
				return getTextureExtensionScale(*it);
			}
		}
		return 1.0f;
	}

	void onMouseDown(april::Key keyCode)
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onMouseDown(keyCode);
		}
	}
	
	void onMouseUp(april::Key keyCode)
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onMouseUp(keyCode);
		}
	}
	
	void onMouseCancel(april::Key keyCode)
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onMouseCancel(keyCode);
		}
	}
	
	void onMouseMove()
	{
		aprilui::updateCursorPosition();
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onMouseMove();
		}
	}
	
	void onMouseScroll(float x, float y)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onMouseScroll(x, y);
		}
	}
	
	void onKeyDown(april::Key keyCode)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onKeyDown(keyCode);
		}
	}
	
	void onKeyUp(april::Key keyCode)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onKeyUp(keyCode);
		}
	}
	
	void onChar(unsigned int charCode)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onChar(charCode);
		}
	}
	
	void onTouch(const harray<gvec2f>& touches)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onTouch(touches);
		}
	}
	
	void onButtonDown(april::Button buttonCode)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onButtonDown(buttonCode);
		}
	}
	
	void onButtonUp(april::Button buttonCode)
	{
		foreach_m (Dataset*, it, datasets)
		{
			it->second->onButtonUp(buttonCode);
		}
	}
	
}
