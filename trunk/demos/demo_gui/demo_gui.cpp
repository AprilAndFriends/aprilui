/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic, Ivan Vucica                           *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <stdio.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <april/RenderSystem.h>
#include <april/Window.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <aprilui/Objects.h>
#include <atres/atres.h>
#include <gtypes/Vector2.h>

grect screen(0, 0, 800, 600);

aprilui::Dataset* dataset;

bool update(float k)
{
	april::rendersys->clear();
	april::rendersys->setOrthoProjection(screen);
	aprilui::updateCursorPosition();
	dataset->update(k);
	dataset->getObject("root")->draw();
	return true;
}

void OnKeyDown(unsigned int keycode)
{
	if (keycode == april::AK_RETURN)
	{
		dataset->unload();
		dataset->load();
	}
	aprilui::OnKeyDown(keycode);
}

int main()
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
		const char* cpath=CFStringGetCStringPtr(path, kCFStringEncodingUTF8);
		char* cpath_alloc=0;
		if(!cpath)
		{
			// CFStringGetCStringPtr is allowed to return NULL. bummer.
			// we need to use CFStringGetCString instead.
			cpath_alloc = (char*)malloc(CFStringGetLength(path)+1);
			CFStringGetCString(path, cpath_alloc, CFStringGetLength(path)+1, kCFStringEncodingUTF8);
		}
		else {
			// even though it didn't return NULL, we still want to slice off bundle name.
			cpath_alloc = (char*)malloc(CFStringGetLength(path)+1);
			strcpy(cpath_alloc, cpath);
		}
		// just in case / is appended to .app path for some reason
		if(cpath_alloc[CFStringGetLength(path)-1]=='/')
			cpath_alloc[CFStringGetLength(path)-1] = 0;
		
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
		april::init("GUI", (int)screen.w, (int)screen.h, false, "demo_gui");
		atres::init();
		aprilui::init();
		april::rendersys->getWindow()->setUpdateCallback(&update);
		april::rendersys->getWindow()->setMouseCallbacks(&aprilui::OnMouseDown, &aprilui::OnMouseUp, &aprilui::OnMouseMove);
		april::rendersys->getWindow()->setKeyboardCallbacks(&OnKeyDown, &aprilui::OnKeyUp, &aprilui::OnChar);
		atres::loadFont("../media/arial.font");
		dataset = new aprilui::Dataset("../media/demo_gui.datadef");
		dataset->load();
#ifdef _DEBUG
		//aprilui::setDebugMode(true);
#endif
		april::rendersys->getWindow()->enterMainLoop();
		delete dataset;
		atres::destroy();
		aprilui::destroy();
		april::destroy();
	}
	catch (aprilui::_GenericException e)
	{
		printf("%s\n", e.getType().c_str());
	}
	return 0;
}
