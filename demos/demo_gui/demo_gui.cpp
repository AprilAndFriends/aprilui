/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifndef _ANDROID
#ifndef _WINRT
#define RESOURCE_PATH "../media/"
#else
#define RESOURCE_PATH "media/"
#endif
#else
#define RESOURCE_PATH "./"
#endif

#include <stdio.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#define __APRIL_SINGLE_INSTANCE_NAME "demo_gui"

#include <april/april.h>
#include <april/KeyboardDelegate.h>
#include <april/main.h>
#include <april/MouseDelegate.h>
#include <april/Platform.h>
#include <april/RenderSystem.h>
#include <april/UpdateDelegate.h>
#include <april/Window.h>
#include <aprilui/Animator.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <aprilui/Objects.h>
#include <aprilui/Texture.h>
#include <atres/atres.h>
#include <atres/FontResourceBitmap.h>
#include <atres/Renderer.h>
#include <gtypes/Vector2.h>

grect drawRect(0.0f, 0.0f, 800.0f, 600.0f);
grect viewport = drawRect;

aprilui::Dataset* dataset;

float _animatorCustomFunction(aprilui::Animator* animator, float time)
{
	float sine = (float)dsin(time * animator->getSpeed() * 360);
	return (sine * sine * animator->getAmplitude() + time * 30); // sin(t)^2 + t*30
}

class UpdateDelegate : public april::UpdateDelegate
{
public:
	bool onUpdate(float timeSinceLastFrame)
	{
		april::rendersys->clear();
		april::rendersys->setOrthoProjection(viewport);
		aprilui::updateCursorPosition();
		aprilui::processEvents();
		aprilui::ProgressBar* progressBar;
		aprilui::ProgressCircle* progressCircle;
		progressBar = dataset->getObject<aprilui::ProgressBar*>("progress_bar_1");
		progressBar->setProgress(hmodf(progressBar->getProgress() + timeSinceLastFrame * 0.2f, 1.25f));
		progressBar = dataset->getObject<aprilui::ProgressBar*>("progress_bar_2");
		progressBar->setProgress(hmodf(progressBar->getProgress() + timeSinceLastFrame * 0.25f, 1.25f));
		progressCircle = dataset->getObject<aprilui::ProgressCircle*>("progress_circle_1");
		progressCircle->setProgress(hmodf(progressCircle->getProgress() + timeSinceLastFrame * 0.2f, 1.25f));
		dataset->getObject<aprilui::Label*>("keyboard_height")->setText(hsprintf("%d%%", (int)(april::window->getVirtualKeyboardHeightRatio() * 100)));
		dataset->update(timeSinceLastFrame);
		dataset->getObject("root")->draw();
		return true;
	}

};

class KeyboardDelegate : public april::KeyboardDelegate
{
	void onKeyDown(april::Key keyCode)
	{
		aprilui::Object* object;
		switch (keyCode)
		{
		case april::AK_MENU:
			dataset->unload();
			dataset->load();
			dataset->getObject<aprilui::Animator*>("custom_animator")->setCustomFunction(&_animatorCustomFunction);
			break;
		case april::AK_INSERT:
			aprilui::setLocalization(aprilui::getLocalization() == "en" ? "de" : "en");
			{
				aprilui::Texture* texture = dataset->getTexture("texture");
				texture->reload(aprilui::getLocalization() == "en" ? RESOURCE_PATH "texture" : RESOURCE_PATH "transparency");
			}
			break;
		case april::AK_N:
			object = dataset->getObject<aprilui::ScrollArea*>("scrolltest_scroll_area");
			if (object->getHeight() > 128)
			{
				object->setHeight(object->getHeight() - 128);
			}
			break;
		case april::AK_M:
			object = dataset->getObject<aprilui::ScrollArea*>("scrolltest_scroll_area");
			object->setHeight(object->getHeight() + 128);
			break;
		case april::AK_D:
			aprilui::setDebugEnabled(!aprilui::isDebugEnabled());
			break;
		}
		aprilui::onKeyDown(keyCode);
	}

	void onKeyUp(april::Key keyCode)
	{
		aprilui::onKeyUp(keyCode);
	}

	void onChar(unsigned int charCode)
	{
		aprilui::onChar(charCode);
	}

};

class MouseDelegate : public april::MouseDelegate
{
	void onMouseDown(april::Key button)
	{
		aprilui::onMouseDown(button);
		aprilui::processEvents();
	}
	
	void onMouseUp(april::Key button)
	{
		aprilui::onMouseUp(button);
		aprilui::processEvents();
	}
	
	void onMouseMove()
	{
		aprilui::onMouseMove();
		aprilui::processEvents();
	}

	void onMouseScroll(float x, float y)
	{
		aprilui::onMouseScroll(x, y);
		aprilui::processEvents();
	}

};

static UpdateDelegate* updateDelegate = NULL;
static KeyboardDelegate* keyboardDelegate = NULL;
static MouseDelegate* mouseDelegate = NULL;

void april_init(const harray<hstr>& args)
{
#ifdef __APPLE__
	// On MacOSX, the current working directory is not set by
	// the Finder, since you are expected to use Core Foundation
	// or ObjC APIs to find files. 
	// So, when porting you probably want to set the current working
	// directory to something sane (e.g. .../Resources/ in the app
	// bundle).
	// In this case, we set it to parent of the .app bundle.
	{	// curly braces in order to localize variables 

		CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
		CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		// let's hope chdir() will be happy with utf8 encoding
		const char* cpath = CFStringGetCStringPtr(path, kCFStringEncodingUTF8);
		char* cpath_alloc = NULL;
		if (cpath == NULL)
		{
			// CFStringGetCStringPtr is allowed to return NULL. bummer.
			// we need to use CFStringGetCString instead.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			CFStringGetCString(path, cpath_alloc, CFStringGetLength(path) + 1, kCFStringEncodingUTF8);
		}
		else
		{
			// even though it didn't return NULL, we still want to slice off bundle name.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			strcpy(cpath_alloc, cpath);
		}
		// just in case / is appended to .app path for some reason
		if (cpath_alloc[CFStringGetLength(path) - 1] == '/')
		{
			cpath_alloc[CFStringGetLength(path) - 1] = 0;
		}
		// replace pre-.app / with a null character, thus
		// cutting off .app's name and getting parent of .app.
		strrchr(cpath_alloc, '/')[0] = 0;
		// change current dir using posix api
		chdir(cpath_alloc);
		free(cpath_alloc); // even if null, still ok
		CFRelease(path);
		CFRelease(url);
	}
#endif
	updateDelegate = new UpdateDelegate();
	keyboardDelegate = new KeyboardDelegate();
	mouseDelegate = new MouseDelegate();
	try
	{
		hlog::setLevelDebug(true);
#if defined(_ANDROID) || defined(_IOS) || defined(_WINRT)
		drawRect.setSize(april::getSystemInfo().displayResolution);
#endif
		april::init(april::RS_DEFAULT, april::WS_DEFAULT);
		april::createRenderSystem();
		april::createWindow((int)drawRect.w, (int)drawRect.h, false, "demo_gui");
		atres::init();
		aprilui::init();
#ifdef _WINRT
		april::window->setParam("cursor_mappings", "101 " RESOURCE_PATH "cursor\n102 " RESOURCE_PATH "simple");
#endif
		april::window->setUpdateDelegate(updateDelegate);
		april::window->setKeyboardDelegate(keyboardDelegate);
		april::window->setMouseDelegate(mouseDelegate);
		april::window->setCursorFilename(RESOURCE_PATH "cursor");
		atres::renderer->registerFontResource(new atres::FontResourceBitmap(RESOURCE_PATH "arial.font"));
		aprilui::setViewport(viewport);
		aprilui::setLocalization("en");
		dataset = new aprilui::Dataset(RESOURCE_PATH "demo_gui.dts");
		dataset->load();
		dataset->getObject<aprilui::Animator*>("custom_animator")->setCustomFunction(&_animatorCustomFunction);
#ifdef _DEBUG
		//aprilui::setDebugMode(true);
#endif
	}
	catch (aprilui::_GenericException& e)
	{
		printf("%s\n", e.getErrorText().c_str());
	}
}

void april_destroy()
{
	try
	{
		delete dataset;
		aprilui::destroy();
		atres::destroy();
		april::destroy();
	}
	catch (aprilui::_GenericException& e)
	{
		printf("%s\n", e.getErrorText().c_str());
	}
	delete updateDelegate;
	updateDelegate = NULL;
	delete keyboardDelegate;
	keyboardDelegate = NULL;
	delete mouseDelegate;
	mouseDelegate = NULL;
}
