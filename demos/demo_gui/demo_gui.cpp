/// @file
/// @version 4.02
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _ANDROID
#ifndef _WINRT
#define RESOURCE_PATH "../../demos/media/"
#else
#define RESOURCE_PATH "media/"
#endif
#elif defined(__APPLE__)
#define RESOURCE_PATH "media/"
#else
#define RESOURCE_PATH "./"
#endif

#include <stdio.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#define __APRIL_SINGLE_INSTANCE_NAME "demo_gui"

#include <april/april.h>
#include <april/Cursor.h>
#include <april/KeyboardDelegate.h>
#include <april/main.h>
#include <april/MouseDelegate.h>
#include <april/Platform.h>
#include <april/RenderSystem.h>
#include <april/UpdateDelegate.h>
#include <april/Window.h>
#include <aprilui/Animator.h>
#include <aprilui/aprilui.h>
#include <aprilui/CallbackEvent.h>
#include <aprilui/Dataset.h>
#include <aprilui/Objects.h>
#include <aprilui/Texture.h>
#include <atres/atres.h>
#include <atres/FontBitmap.h>
#include <atres/Renderer.h>
#include <gtypes/Vector2.h>

#define LOG_TAG "demo_gui"

grect drawRect(0.0f, 0.0f, 1024.0f, 768.0f);
grect viewport = drawRect;

april::Cursor* cursor = NULL;
aprilui::Dataset* dataset = NULL;

float _animatorCustomFunction(aprilui::Animator* animator, float time)
{
	float sine = (float)dsin(time * animator->getSpeed() * 360);
	return (sine * sine * animator->getAmplitude() + time * 30); // sin(t)^2 + t*30
}

void _hoverStarted(aprilui::EventArgs* args)
{
	hlog::write(LOG_TAG, "Mouse Hover started: " + args->baseObject->getName());
}

void _hoverFinished(aprilui::EventArgs* args)
{
	hlog::write(LOG_TAG, "Mouse Hover finished: " + args->baseObject->getName());
}

void _listBoxSelectedChanged(aprilui::EventArgs* args)
{
	hlog::writef(LOG_TAG, "ListBox selected: '%s'", args->string.cStr());
}

void _treeViewSelectedChanged(aprilui::EventArgs* args)
{
	hlog::writef(LOG_TAG, "TreeView selected: '%s'", args->string.cStr());
}

void _gridViewSelectedChanged(aprilui::EventArgs* args)
{
	hlog::writef(LOG_TAG, "GridView selected: '%s'", args->string.cStr());
}

class UpdateDelegate : public april::UpdateDelegate
{
public:
	bool onUpdate(float timeDelta)
	{
		april::rendersys->clear();
		april::rendersys->setOrthoProjection(viewport);
		aprilui::updateCursorPosition();
		aprilui::processEvents();
		aprilui::ProgressBase* progressObject = NULL;
		progressObject = dataset->getObject<aprilui::ProgressBase*>("progress_bar_1");
		progressObject->setProgress(hmodf(progressObject->getProgress() + timeDelta * 0.2f, 1.2f));
		progressObject = dataset->getObject<aprilui::ProgressBase*>("progress_circle_1");
		progressObject->setProgress(hmodf(progressObject->getProgress() + timeDelta * 0.2f, 1.2f));
		dataset->getObject<aprilui::Label*>("keyboard_height")->setText(hsprintf("%d%%", (int)(april::window->getVirtualKeyboardHeightRatio() * 100)));
		dataset->update(timeDelta);
		dataset->draw();
		return true;
	}

};

class KeyboardDelegate : public april::KeyboardDelegate
{
	void onKeyDown(april::Key keyCode)
	{
		aprilui::Object* object = NULL;
		aprilui::ListBox* listBox = NULL;
		aprilui::ListBoxItem* listBoxItem = NULL;
		aprilui::TreeView* treeView = NULL;
		aprilui::TreeViewNode* treeViewNode = NULL;
		aprilui::GridView* gridView = NULL;
		aprilui::GridViewRow* gridViewRow = NULL;
		harray<int> indices = hstr("1,0").split(',').cast<int>();
		switch (keyCode)
		{
		case april::AK_MENU:
			dataset->unload();
			dataset->load();
			dataset->getAnimator("custom_animator")->setCustomFunction(&_animatorCustomFunction);
			break;
		case april::AK_CONTROL:
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
		case april::AK_INSERT:
			listBox = dataset->getObject<aprilui::ListBox*>("list_box");
			listBoxItem = listBox->createItem(hrand(listBox->getItemCount() + 1), april::generateName("item "));
			listBoxItem->setText(listBoxItem->getName());
			listBoxItem->setEffect(atres::TextEffect::Border);
			break;
		case april::AK_DELETE:
			listBox = dataset->getObject<aprilui::ListBox*>("list_box");
			listBox->deleteItem(hrand(listBox->getItemCount()));
			break;
		case april::AK_HOME:
			treeView = dataset->getObject<aprilui::TreeView*>("tree_view");
			indices += treeView->getItemAt(indices)->getNodes().size();
			treeViewNode = treeView->createItem(indices, april::generateName("node "));
			treeViewNode->getLabel()->setText(treeViewNode->getName());
			treeViewNode->getLabel()->setEffect(atres::TextEffect::Border);
			treeViewNode->getLabel()->setHorzFormatting(atres::Horizontal::Left);
			break;
		case april::AK_END:
			treeView = dataset->getObject<aprilui::TreeView*>("tree_view");
			treeViewNode = treeView->getItemAt(indices);
			if (treeViewNode != NULL)
			{
				indices += treeViewNode->getNodes().size() - 1;
				if (indices.last() >= 0)
				{
					treeView->deleteItem(indices);
				}
			}
			break;
		case april::AK_PRIOR:
			gridView = dataset->getObject<aprilui::GridView*>("grid_view");
			gridViewRow = gridView->createRow(hrand(gridView->getRowCount() + 1), april::generateName("row "));
			break;
		case april::AK_NEXT:
			gridView = dataset->getObject<aprilui::GridView*>("grid_view");
			gridView->deleteRow(hrand(gridView->getRowCount()));
			break;
		case april::AK_F2:
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
	
	void onMouseCancel(april::Key button)
	{
		aprilui::onMouseCancel(button);
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
		april::Window::Options options;
		options.fpsCounter = true;
		april::createWindow((int)drawRect.w, (int)drawRect.h, false, "demo_gui", options);
		atres::init();
		aprilui::init();
#ifdef _WINRT
		april::window->setParam("cursor_mappings", "101 " RESOURCE_PATH "cursor\n102 " RESOURCE_PATH "simple");
#endif
		april::window->setUpdateDelegate(updateDelegate);
		april::window->setKeyboardDelegate(keyboardDelegate);
		april::window->setMouseDelegate(mouseDelegate);
		cursor = april::window->createCursorFromResource(RESOURCE_PATH "cursor");
		april::window->setCursor(cursor);
		atres::renderer->registerFont(new atres::FontBitmap(RESOURCE_PATH "arial.font"));
		aprilui::setViewport(viewport);
		aprilui::setLocalization("en");
		aprilui::setUseKeyboardAutoOffset(true);
		dataset = new aprilui::Dataset(RESOURCE_PATH "demo_gui.dts");
		dataset->load();
		dataset->getAnimator("custom_animator")->setCustomFunction(&_animatorCustomFunction);
		aprilui::Object* object = dataset->getObject("hover_image_button");
		object->registerEvent(aprilui::Event::HoverStarted, new aprilui::CallbackEvent(&_hoverStarted));
		object->registerEvent(aprilui::Event::HoverFinished, new aprilui::CallbackEvent(&_hoverFinished));
		dataset->getObject<aprilui::SelectionContainer*>("list_box")->registerEvent(aprilui::Event::SelectedChanged, new aprilui::CallbackEvent(&_listBoxSelectedChanged));
		dataset->getObject<aprilui::SelectionContainer*>("tree_view")->registerEvent(aprilui::Event::SelectedChanged, new aprilui::CallbackEvent(&_treeViewSelectedChanged));
		dataset->getObject<aprilui::SelectionContainer*>("grid_view")->registerEvent(aprilui::Event::SelectedChanged, new aprilui::CallbackEvent(&_gridViewSelectedChanged));
	}
	catch (hexception& e)
	{
		hlog::error(LOG_TAG, e.getMessage().cStr());
	}
}

void april_destroy()
{
	try
	{
		april::window->setCursor(NULL);
		april::window->destroyCursor(cursor);
		delete dataset;
		aprilui::destroy();
		atres::destroy();
		april::destroy();
	}
	catch (hexception& e)
	{
		hlog::error(LOG_TAG, e.getMessage().cStr());
	}
	delete updateDelegate;
	updateDelegate = NULL;
	delete keyboardDelegate;
	keyboardDelegate = NULL;
	delete mouseDelegate;
	mouseDelegate = NULL;
}
