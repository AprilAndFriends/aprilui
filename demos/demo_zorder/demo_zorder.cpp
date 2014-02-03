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

#define __APRIL_SINGLE_INSTANCE_NAME "demo_zorder"

#include <april/april.h>
#include <april/main.h>
#include <april/RenderSystem.h>
#include <april/Platform.h>
#include <april/UpdateDelegate.h>
#include <april/Window.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <aprilui/Objects.h>
#include <atres/atres.h>
#include <atres/FontResourceBitmap.h>
#include <atres/Renderer.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>

grect drawRect(0.0f, 0.0f, 800.0f, 600.0f);
grect viewport = drawRect;

aprilui::Dataset* dataset;

class UpdateDelegate : public april::UpdateDelegate
{
public:
	bool onUpdate(float timeSinceLastFrame)
	{
		april::rendersys->clear();
		april::rendersys->setOrthoProjection(viewport);
		aprilui::updateCursorPosition();
		aprilui::processEvents();
		dataset->getObject("obj0" + hstr(hrand(1, 8)))->setZOrder(hrand(100));
		dataset->getObject("root")->draw();
		dataset->update(timeSinceLastFrame);
		return true;
	}

};

static UpdateDelegate* updateDelegate = NULL;

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
	try
	{
		hlog::setLevelDebug(true);
#if defined(_ANDROID) || defined(_IOS) || defined(_WINRT)
		drawRect.setSize(april::getSystemInfo().displayResolution);
#endif
		april::init(april::RS_DEFAULT, april::WS_DEFAULT);
		april::createRenderSystem();
		april::createWindow((int)drawRect.w, (int)drawRect.h, false, "demo_zorder");
		atres::init();
		aprilui::init();
#ifdef _WINRT
		april::window->setParam("cursor_mappings", "101 " RESOURCE_PATH "cursor\n102 " RESOURCE_PATH "simple");
#endif
		april::window->setUpdateDelegate(updateDelegate);
		april::window->setCursorFilename(RESOURCE_PATH "cursor");
		aprilui::setViewport(viewport);
		aprilui::setLocalization("en");
		dataset = new aprilui::Dataset(RESOURCE_PATH "demo_zorder.dts");
		dataset->load();
	}
	catch (aprilui::_GenericException& e)
	{
		printf("%s\n", e.getType().c_str());
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
		printf("%s\n", e.getType().c_str());
	}
	delete updateDelegate;
	updateDelegate = NULL;
}
