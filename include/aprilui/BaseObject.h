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
/// Defines a base object as a node in a tree-like structure for GUI elements and other objects.

#ifndef APRILUI_BASE_OBJECT_H
#define APRILUI_BASE_OBJECT_H

#include <april/Keys.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Event.h"
#include "EventReceiver.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Animator;
	class Dataset;
	class EventArgs;
	class Object;
	class Style;

	class apriluiExport BaseObject : public EventReceiver
	{
		APRILUI_CLONEABLE(BaseObject);
	public:
		friend class Dataset;
		friend class Object;

		BaseObject(chstr name);
		~BaseObject();
		virtual inline hstr getClassName() const { return "BaseObject"; }

		virtual harray<PropertyDescription> getPropertyDescriptions() const;

		HL_DEFINE_GET(hstr, name, Name);
		void setName(chstr value);
		HL_DEFINE_GETSET(hstr, tag, Tag);
		hstr getFullName() const;
		HL_DEFINE_IS(enabled, Enabled);
		void setEnabled(const bool& value);
		HL_DEFINE_IS(awake, Awake);
		void setAwake(const bool& value);
		HL_DEFINE_GET(int, zOrder, ZOrder);
		void setZOrder(const int& zorder);
		virtual inline Dataset* getDataset() const { return this->dataset; }
		virtual inline Object* getParent() const { return this->parent; }

		inline harray<Object*>& getChildrenObjects() { return this->childrenObjects; }
		inline harray<Animator*>& getChildrenAnimators() { return this->childrenAnimators; }
		inline const harray<Object*>& getChildrenObjects() const { return this->childrenObjects; }
		inline const harray<Animator*>& getChildrenAnimators() const { return this->childrenAnimators; }
		harray<BaseObject*> getChildren() const;
		virtual inline bool isAnimated() const { return false; }
		virtual inline bool isWaitingAnimation() const { return false; }

		harray<Object*> getAncestors() const;
		harray<BaseObject*> getDescendants() const;
		harray<Object*> getDescendantObjects() const;

		bool isDerivedEnabled() const;
		bool isDerivedAwake() const;

		bool hasProperty(chstr name);

		/// @returns Whether or not a given object is a direct child of this object
		bool isChild(BaseObject* object);
		/// @returns whether or not a given object is a descendant of this object (child or child of a child etc., recursively)
		bool isDescendant(BaseObject* object);
		/// @returns whether or not a given object is a direct parent of this object
		bool isParent(BaseObject* object);
		/// @returns whether or not a given object is an ancestor of a this object (parent or parent of a parent etc., recursively)
		bool isAncestor(BaseObject* object);
		BaseObject* findChildByName(chstr name);
		BaseObject* findDescendantByName(chstr name);

		void applyStyle(Style* value);
		void applyStyleByName(chstr name);

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		void update(float timeDelta);

	protected:
		hstr name;
		hstr tag;
		Object* parent; // a BaseObject cannot be a parent
		harray<Object*> childrenObjects;
		harray<Animator*> childrenAnimators;
		bool enabled;
		bool awake;
		int zOrder;

		virtual hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		virtual hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		virtual void _update(float timeDelta);
		void _sortChildren();

	private:
		static harray<PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};
	
}

#endif
