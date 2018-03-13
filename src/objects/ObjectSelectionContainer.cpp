/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>
#include <april/Keys.h>
#include <april/Window.h>
#include <hltypes/harray.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Object.h"
#include "ObjectSelectionContainer.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> SelectionContainer::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> SelectionContainer::_getters;
	hmap<hstr, PropertyDescription::Accessor*> SelectionContainer::_setters;

	SelectionContainer::SelectionContainer(chstr name) : Container(name)
	{
		this->selectedIndex = -1;
		this->itemHeight = 32.0f;
		this->pushedColor.set(april::Color::LightGrey, 192);
		this->hoverColor.set(april::Color::White, 192);
		this->selectedHoverColor.set(april::Color::LightAqua, 192);
		this->selectedColor.set(april::Color::Aqua, 192);
		this->selectedPushedColor.set(april::Color::DarkAqua, 192);
		this->allowDrag = false;
	}

	SelectionContainer::SelectionContainer(const SelectionContainer& other) : Container(other)
	{
		this->selectedIndex = -1; // a cloned SelectionContainer has no children
		this->itemHeight = other.itemHeight;
		this->pushedColor = other.pushedColor;
		this->hoverColor = other.hoverColor;
		this->selectedColor = other.selectedColor;
		this->selectedPushedColor = other.selectedPushedColor;
		this->selectedHoverColor = other.selectedHoverColor;
		this->allowDrag = other.allowDrag;
	}

	SelectionContainer::~SelectionContainer()
	{
	}
	
	hmap<hstr, PropertyDescription>& SelectionContainer::getPropertyDescriptions() const
	{
		if (SelectionContainer::_propertyDescriptions.size() == 0)
		{
			SelectionContainer::_propertyDescriptions = Container::getPropertyDescriptions();
			SelectionContainer::_propertyDescriptions["selected_index"] = PropertyDescription("selected_index", PropertyDescription::Type::Int);
			SelectionContainer::_propertyDescriptions["item_height"] = PropertyDescription("item_height", PropertyDescription::Type::Float);
			SelectionContainer::_propertyDescriptions["pushed_color"] = PropertyDescription("pushed_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions["hover_color"] = PropertyDescription("hover_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions["selected_color"] = PropertyDescription("selected_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions["selected_pushed_color"] = PropertyDescription("selected_pushed_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions["selected_hover_color"] = PropertyDescription("selected_hover_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions["allow_drag"] = PropertyDescription("allow_drag", PropertyDescription::Type::Bool);
			SelectionContainer::_propertyDescriptions["item_count"] = PropertyDescription("item_count", PropertyDescription::Type::Int);
		}
		return SelectionContainer::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& SelectionContainer::_getGetters() const
	{
		if (SelectionContainer::_getters.size() == 0)
		{
			SelectionContainer::_getters = Container::_getGetters();
			SelectionContainer::_getters["selected_index"] = new PropertyDescription::Get<SelectionContainer, int>(&SelectionContainer::getSelectedIndex);
			SelectionContainer::_getters["item_height"] = new PropertyDescription::Get<SelectionContainer, float>(&SelectionContainer::getItemHeight);
			SelectionContainer::_getters["pushed_color"] = new PropertyDescription::GetColor<SelectionContainer>(&SelectionContainer::getPushedColor);
			SelectionContainer::_getters["hover_color"] = new PropertyDescription::GetColor<SelectionContainer>(&SelectionContainer::getHoverColor);
			SelectionContainer::_getters["selected_color"] = new PropertyDescription::GetColor<SelectionContainer>(&SelectionContainer::getSelectedColor);
			SelectionContainer::_getters["selected_pushed_color"] = new PropertyDescription::GetColor<SelectionContainer>(&SelectionContainer::getSelectedPushedColor);
			SelectionContainer::_getters["selected_hover_color"] = new PropertyDescription::GetColor<SelectionContainer>(&SelectionContainer::getSelectedHoverColor);
			SelectionContainer::_getters["allow_drag"] = new PropertyDescription::Get<SelectionContainer, bool>(&SelectionContainer::isAllowDrag);
			SelectionContainer::_getters["item_count"] = new PropertyDescription::Get<SelectionContainer, int>(&SelectionContainer::getItemCount);
		}
		return SelectionContainer::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& SelectionContainer::_getSetters() const
	{
		if (SelectionContainer::_setters.size() == 0)
		{
			SelectionContainer::_setters = Container::_getSetters();
			SelectionContainer::_setters["selected_index"] = new PropertyDescription::Set<SelectionContainer, int>(&SelectionContainer::setSelectedIndex);
			SelectionContainer::_setters["item_height"] = new PropertyDescription::Set<SelectionContainer, float>(&SelectionContainer::setItemHeight);
			SelectionContainer::_setters["pushed_color"] = new PropertyDescription::Set<SelectionContainer, hstr>(&SelectionContainer::setPushedSymbolicColor);
			SelectionContainer::_setters["hover_color"] = new PropertyDescription::Set<SelectionContainer, hstr>(&SelectionContainer::setHoverSymbolicColor);
			SelectionContainer::_setters["selected_color"] = new PropertyDescription::Set<SelectionContainer, hstr>(&SelectionContainer::setSelectedSymbolicColor);
			SelectionContainer::_setters["selected_pushed_color"] = new PropertyDescription::Set<SelectionContainer, hstr>(&SelectionContainer::setSelectedPushedSymbolicColor);
			SelectionContainer::_setters["selected_hover_color"] = new PropertyDescription::Set<SelectionContainer, hstr>(&SelectionContainer::setSelectedHoverSymbolicColor);
			SelectionContainer::_setters["allow_drag"] = new PropertyDescription::Set<SelectionContainer, bool>(&SelectionContainer::setAllowDrag);
		}
		return SelectionContainer::_setters;
	}

	void SelectionContainer::setSelectedIndex(const int& value)
	{
		if (this->selectedIndex != value)
		{
			int oldIndex = this->selectedIndex;
			this->selectedIndex = (value < this->getItemCount() ? value : -1);
			if (this->selectedIndex != oldIndex)
			{
				this->_updateItem(oldIndex);
				this->_updateItem(this->selectedIndex);
				this->triggerEvent(Event::SelectedChanged, hstr(this->selectedIndex));
			}
		}
	}

	void SelectionContainer::setItemHeight(const float& value)
	{
		if (this->itemHeight != value)
		{
			this->itemHeight = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setPushedColor(const april::Color& value)
	{
		if (this->pushedColor != value)
		{
			this->pushedColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setPushedSymbolicColor(chstr value)
	{
		this->setPushedColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setHoverColor(const april::Color& value)
	{
		if (this->hoverColor != value)
		{
			this->hoverColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setHoverSymbolicColor(chstr value)
	{
		this->setHoverColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setSelectedColor(const april::Color& value)
	{
		if (this->selectedColor != value)
		{
			this->selectedColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setSelectedSymbolicColor(chstr value)
	{
		this->setSelectedColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setSelectedPushedColor(const april::Color& value)
	{
		if (this->selectedPushedColor != value)
		{
			this->selectedPushedColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setSelectedPushedSymbolicColor(chstr value)
	{
		this->setSelectedPushedColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setSelectedHoverColor(const april::Color& value)
	{
		if (this->selectedHoverColor != value)
		{
			this->selectedHoverColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setSelectedHoverSymbolicColor(chstr value)
	{
		this->setSelectedHoverColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setAllowDrag(const bool& value)
	{
		if (this->allowDrag != value)
		{
			this->allowDrag = value;
			if (this->scrollArea != NULL)
			{
				this->scrollArea->setAllowDrag(value);
			}
		}
	}

	void SelectionContainer::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args);
		if (type == Event::RegisteredInDataset)
		{
			if (this->scrollArea == NULL)
			{
				this->registerChild(new ScrollArea(april::generateName("aprilui::ScrollArea"))); // sets this->scrollArea
				this->scrollArea->setRect(this->rect);
				this->scrollArea->setAnchors(true, true, true, false);
				this->scrollArea->setVisible(this->getItemCount() > 0);
			}
		}
	}

	void SelectionContainer::_update(float timeDelta)
	{
		Container::_update(timeDelta);
		this->_optimizeVisibility();
	}

	void SelectionContainer::_optimizeVisibility()
	{
	}

}
