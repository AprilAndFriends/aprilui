/// @file
/// @version 4.02
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef __ANDROID__
	#ifndef _UWP
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
	#import <CoreFoundation/CoreFoundation.h>
	#import <Foundation/Foundation.h>
	#include <hltypes/hdir.h>
#endif

#define __APRIL_SINGLE_INSTANCE_NAME "demo_gui"

#include <april/april.h>
#include <april/Application.h>
#include <april/Cursor.h>
#include <april/KeyDelegate.h>
#include <april/main.h>
#include <april/MouseDelegate.h>
#include <april/Platform.h>
#include <april/RenderSystem.h>
#include <april/SystemDelegate.h>
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

grectf drawRect(0.0f, 0.0f, 1024.0f, 768.0f);
grectf viewport = drawRect;

april::Cursor* cursor = NULL;
aprilui::Dataset* dataset = NULL;

double _animatorCustomFunction(aprilui::Animator* animator, double time)
{
	double sine = hsin(time * animator->getSpeed() * 360);
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
		dataset->getObject<aprilui::Label*>("keyboard_height")->setText(hsprintf("KBH: %d%%", (int)(april::window->getVirtualKeyboardHeightRatio() * 100)));
		dataset->getObject<aprilui::Label*>("fps")->setText("FPS: " + hstr(april::application->getFps()));
		dataset->update(timeDelta);
		dataset->draw();
		return true;
	}

};

class SystemDelegate : public april::SystemDelegate
{
public:
	SystemDelegate() : april::SystemDelegate()
	{
	}

	void onWindowSizeChanged(int width, int height, bool fullScreen)
	{
		//this is called when the window size is changed
		april::rendersys->setViewport(drawRect);
		aprilui::setViewport(drawRect);
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

class KeyDelegate : public april::KeyDelegate
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
		if (keyCode == april::Key::Menu)
		{
			dataset->unload();
			dataset->load();
			dataset->getAnimator("custom_animator")->setCustomFunction(&_animatorCustomFunction);
		}
		else if (keyCode == april::Key::Control)
		{
			aprilui::setLocalization(aprilui::getLocalization() == "en" ? "de" : "en");
			{
				aprilui::Texture* texture = dataset->getTexture("texture");
				texture->reload(aprilui::getLocalization() == "en" ? RESOURCE_PATH "texture" : RESOURCE_PATH "transparency");
			}
		}
		else if (keyCode == april::Key::N)
		{
			object = dataset->getObject<aprilui::ScrollArea*>("scrolltest_scroll_area");
			if (object->getHeight() > 128)
			{
				object->setHeight(object->getHeight() - 128);
			}
		}
		else if (keyCode == april::Key::M)
		{
			object = dataset->getObject<aprilui::ScrollArea*>("scrolltest_scroll_area");
			object->setHeight(object->getHeight() + 128);
		}
		else if (keyCode == april::Key::Insert)
		{
			listBox = dataset->getObject<aprilui::ListBox*>("list_box");
			listBoxItem = listBox->createItem(hrand(listBox->getItemCount() + 1), april::generateName("item "));
			listBoxItem->setText(listBoxItem->getName());
			listBoxItem->setEffect(atres::TextEffect::Border);
		}
		else if (keyCode == april::Key::Delete)
		{
			listBox = dataset->getObject<aprilui::ListBox*>("list_box");
			listBox->deleteItem(hrand(listBox->getItemCount()));
		}
		else if (keyCode == april::Key::Home)
		{
			treeView = dataset->getObject<aprilui::TreeView*>("tree_view");
			indices += treeView->getItemAt(indices)->getNodes().size();
			treeViewNode = treeView->createItem(indices, april::generateName("node "));
			treeViewNode->getLabel()->setText(treeViewNode->getName());
			treeViewNode->getLabel()->setEffect(atres::TextEffect::Border);
			treeViewNode->getLabel()->setHorzFormatting(atres::Horizontal::Left);
		}
		else if (keyCode == april::Key::End)
		{
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
		}
		else if (keyCode == april::Key::Prior)
		{
			gridView = dataset->getObject<aprilui::GridView*>("grid_view");
			gridViewRow = gridView->createRow(hrand(gridView->getRowCount() + 1), april::generateName("row "));
		}
		else if (keyCode == april::Key::Next)
		{
			gridView = dataset->getObject<aprilui::GridView*>("grid_view");
			gridView->deleteRow(hrand(gridView->getRowCount()));
		}
		else if (keyCode == april::Key::F2)
		{
			aprilui::setDebugEnabled(!aprilui::isDebugEnabled());
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

static UpdateDelegate* updateDelegate = NULL;
static SystemDelegate* systemDelegate = NULL;
static MouseDelegate* mouseDelegate = NULL;
static KeyDelegate* keyDelegate = NULL;

#ifdef __APPLE__
static void ObjCUtil_setCWD(const char* override_default_dir)
{
	static bool set = 0;
	if (!set || override_default_dir != NULL)
	{
		if (override_default_dir == NULL)
		{
			NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
			const char* dir = [[[NSBundle mainBundle] resourcePath] UTF8String];
			hdir::chdir(dir);
			[pool release];
		}
		else
		{
			hdir::chdir(override_default_dir);
		}
		set = 1;
	}
}
#endif

void __aprilApplicationInit()
{
#ifdef __APPLE__
	ObjCUtil_setCWD(nil);
#endif
	updateDelegate = new UpdateDelegate();
	systemDelegate = new SystemDelegate();
	mouseDelegate = new MouseDelegate();
	keyDelegate = new KeyDelegate();
	try
	{
		hlog::setLevelDebug(true);
#if defined(__ANDROID__) || defined(_IOS)
		drawRect.setSize(april::getSystemInfo().displayResolution);
#endif
		april::init(april::RenderSystemType::Default, april::WindowType::Default);
		april::createRenderSystem();
		april::Window::Options options;
		options.fpsCounter = true;
		april::createWindow((int)drawRect.w, (int)drawRect.h, false, "demo_gui", options);
		atres::init();
		aprilui::init();
#ifdef _UWP
		april::window->setParam("cursor_mappings", "101 " RESOURCE_PATH "cursor\n102 " RESOURCE_PATH "simple");
#endif
		april::window->setUpdateDelegate(updateDelegate);
		april::window->setSystemDelegate(systemDelegate);
		april::window->setMouseDelegate(mouseDelegate);
		april::window->setKeyDelegate(keyDelegate);
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

void __aprilApplicationDestroy()
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
	delete keyDelegate;
	keyDelegate = NULL;
	delete mouseDelegate;
	mouseDelegate = NULL;
}
