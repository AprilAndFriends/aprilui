/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hplatform.h>
#ifndef _ANDROID
#if !_HL_WINRT
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

#include <april/april.h>
#include <april/main.h>
#include <april/RenderSystem.h>
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
#ifndef _ANDROID
grect viewport = drawRect;
#else
grect viewport(0.0f, 0.0f, 480.0f, 320.0f);
#endif

aprilui::Dataset* dataset;

float _animatorCustomFunction(aprilui::Animator* animator, float time)
{
	float sine = (float)dsin(time * animator->getSpeed() * 360);
	return (sine * sine * animator->getAmplitude()); // sin(x)^2
}

bool update(float k)
{
	april::rendersys->clear();
	april::rendersys->setOrthoProjection(drawRect);
	aprilui::updateCursorPosition();
	dataset->update(k);
	dataset->getObject("root")->draw();
	return true;
}

void onKeyDown(unsigned int keycode)
{
	aprilui::Object* object;
	switch (keycode)
	{
	case april::AK_RETURN:
		dataset->unload();
		dataset->load();
		dataset->getObject<aprilui::Animator*>("custom_animator")->setCustomFunction(&_animatorCustomFunction);
		break;
	case april::AK_BACK:
		aprilui::setLocalization(aprilui::getLocalization() == "en" ? "de" : "en");
		{
			aprilui::Texture* texture = dataset->getTexture("texture");
			texture->reload(aprilui::getLocalization() == "en" ? "../media/texture" : "../media/transparency");
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
	aprilui::onKeyDown(keycode);
}

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
			cpath_alloc = (char*)malloc(CFStringGetLength(path)+1);
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
	try
	{
		april::init(april::RS_DEFAULT, april::WS_DEFAULT);
		april::createRenderSystem();
		april::createWindow((int)viewport.w, (int)viewport.h, false, "demo_gui");
		atres::init();
		aprilui::init();
		april::window->setUpdateCallback(&update);
		april::window->setMouseCallbacks(&aprilui::onMouseDown, &aprilui::onMouseUp, &aprilui::onMouseMove, &aprilui::onMouseScroll);
		april::window->setKeyboardCallbacks(&onKeyDown, &aprilui::onKeyUp, &aprilui::onChar);
		atres::renderer->registerFontResource(new atres::FontResourceBitmap(RESOURCE_PATH "arial.font"));
		aprilui::setLocalization("en");
		dataset = new aprilui::Dataset(RESOURCE_PATH "demo_gui.dts");
		dataset->load();
		dataset->getObject<aprilui::Animator*>("custom_animator")->setCustomFunction(&_animatorCustomFunction);
#ifdef _DEBUG
		//aprilui::setDebugMode(true);
#endif
	}
	catch (aprilui::_GenericException e)
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
	catch (aprilui::_GenericException e)
	{
		printf("%s\n", e.getErrorText().c_str());
	}
}
