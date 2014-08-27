/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base object as a node in a tree-like structure for GUI elements and other objects.

#ifndef APRILUI_BASE_OBJECT_H
#define APRILUI_BASE_OBJECT_H

#include <april/Keys.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "EventReceiver.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Animator;
	class Dataset;
	class Event;
	class EventArgs;
	class Object;

	class apriluiExport BaseObject : public EventReceiver
	{
	public:
		friend class Dataset;
		friend class Object;

		BaseObject(chstr name);
		~BaseObject();
		virtual hstr getClassName() const { return "BaseObject"; }

		HL_DEFINE_GET(hstr, name, Name);
		virtual HL_DEFINE_GET(Dataset*, dataset, Dataset);
		HL_DEFINE_GET(Object*, parent, Parent);
		HL_DEFINE_IS(enabled, Enabled);
		void setEnabled(bool value);
		HL_DEFINE_GET(int, zOrder, ZOrder);
		void setZOrder(int zorder);

		inline harray<Object*>& getObjects() { return this->objects; }
		inline harray<Animator*>& getAnimators() { return this->animators; }
		harray<BaseObject*> getChildren();
		inline hmap<hstr, Event*>& getEvents() { return this->events; }
		virtual inline bool isAnimated() { return false; }
		virtual inline bool isWaitingAnimation() { return false; }

		hstr getFullName();
		harray<Object*> getAncestors();
		harray<BaseObject*> getDescendants();

		bool isDerivedEnabled();

		virtual harray<PropertyDescription> getPropertyDescriptions();
		bool hasProperty(chstr name);

		/// @returns Whether or not a given object is a direct child of this object
		bool isChild(BaseObject* obj);
		/// @returns whether or not a given object is a descendant of this object (child or child of a child etc., recursively)
		bool isDescendant(BaseObject* obj);
		/// @returns whether or not a given object is a direct parent of this object
		bool isParent(BaseObject* obj);
		/// @returns whether or not a given object is an ancestor of a this object (parent or parent of a parent etc., recursively)
		bool isAncestor(BaseObject* obj);
		BaseObject* findChildByName(chstr name);
		BaseObject* findDescendantByName(chstr name);
		void attach(Object* object);
		void detach();

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		virtual void update(float timeDelta);

		void registerEvent(chstr name, void(*callback)(EventArgs*));
		void registerEvent(chstr name, Event* event);
		void unregisterEvent(chstr name);

		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, april::Key keyCode = april::AK_NONE, chstr extra = "");
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, april::Button buttonCode, chstr extra = "");
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, float x, float y, april::Key keyCode = april::AK_NONE, chstr extra = "");

	protected:
		hstr name;
		Dataset* dataset;
		Object* parent; // a BaseObject cannot be a parent
		harray<Object*> objects;
		harray<Animator*> animators;
		bool enabled;
		int zOrder;
		hmap<hstr, Event*> events;

		void _sortChildren();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
	
}

#endif
