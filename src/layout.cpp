// layout.cpp
/*
  neogfx C++ GUI Library
  Copyright(C) 2016 Leigh Johnston
  
  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "neogfx.hpp"
#include "layout.hpp"
#include "i_widget.hpp"
#include "i_spacer.hpp"
#include "app.hpp"

namespace neogfx
{
	layout::item::item(i_widget& aWidget) :
		iPointerWrapper(widget_pointer(widget_pointer(), &aWidget))
	{
	}

	layout::item::item(std::shared_ptr<i_widget> aWidget) :
		iPointerWrapper(aWidget)
	{
	}

	layout::item::item(i_layout& aLayout) :
		iPointerWrapper(layout_pointer(layout_pointer(), &aLayout))
	{
	}

	layout::item::item(std::shared_ptr<i_layout> aLayout) :
		iPointerWrapper(aLayout)
	{
	}

	layout::item::item(i_spacer& aSpacer) :
		iPointerWrapper(spacer_pointer(spacer_pointer(), &aSpacer))
	{
	}

	layout::item::item(std::shared_ptr<i_spacer> aSpacer) :
		iPointerWrapper(aSpacer)
	{
	}

	const layout::item::pointer_wrapper& layout::item::get() const
	{
		return iPointerWrapper;
	}

	layout::item::pointer_wrapper& layout::item::get()
	{
		return iPointerWrapper;
	}

	void layout::item::set_owner(i_widget* aOwner)
	{
		iOwner = aOwner;
		if (iPointerWrapper.is<widget_pointer>())
			iOwner->add_widget(static_variant_cast<widget_pointer&>(iPointerWrapper));
	}

	void layout::item::layout(const point& aPosition, const size& aSize)
	{
		if (iPointerWrapper.is<widget_pointer>())
		{
			static_variant_cast<widget_pointer&>(iPointerWrapper)->move(aPosition);
			static_variant_cast<widget_pointer&>(iPointerWrapper)->resize(aSize);
		}
		else if (iPointerWrapper.is<layout_pointer>())
		{
			static_variant_cast<layout_pointer&>(iPointerWrapper)->layout_items(aPosition, aSize);
		}
	}

	bool layout::item::has_minimum_size() const
	{
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->has_minimum_size() :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->has_minimum_size() : 
				static_variant_cast<const spacer_pointer&>(iPointerWrapper)->has_minimum_size();
	}

	size layout::item::minimum_size() const
	{
		if (!visible())
			return size{};
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->minimum_size() :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->minimum_size() : 
				static_variant_cast<const spacer_pointer&>(iPointerWrapper)->minimum_size();
	}

	void layout::item::set_minimum_size(const optional_size& aMinimumSize, bool aUpdateLayout)
	{
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->set_minimum_size(aMinimumSize, aUpdateLayout) :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->set_minimum_size(aMinimumSize, aUpdateLayout) :
				static_variant_cast<const spacer_pointer&>(iPointerWrapper)->set_minimum_size(aMinimumSize, aUpdateLayout);
	}

	bool layout::item::has_maximum_size() const
	{
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->has_maximum_size() :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->has_maximum_size() : 
				static_variant_cast<const spacer_pointer&>(iPointerWrapper)->has_maximum_size();
	}

	size layout::item::maximum_size() const
	{
		if (!visible())
			return size{};
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->maximum_size() :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->maximum_size() : 
				static_variant_cast<const spacer_pointer&>(iPointerWrapper)->maximum_size();
	}

	void layout::item::set_maximum_size(const optional_size& aMaximumSize, bool aUpdateLayout)
	{
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->set_maximum_size(aMaximumSize, aUpdateLayout) :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->set_maximum_size(aMaximumSize, aUpdateLayout) : 
				static_variant_cast<const spacer_pointer&>(iPointerWrapper)->set_maximum_size(aMaximumSize, aUpdateLayout);
	}

	bool layout::item::is_fixed_size() const
	{
		return has_minimum_size() && minimum_size() == maximum_size();
	}

	void layout::item::set_fixed_size(const optional_size& aFixedSize, bool aUpdateLayout)
	{
		set_minimum_size(aFixedSize, aUpdateLayout);
		set_maximum_size(aFixedSize, aUpdateLayout);
	}

	bool layout::item::has_margins() const
	{
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->has_margins() :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->has_margins() : 
				false;
	}

	margins layout::item::margins() const
	{
		return iPointerWrapper.is<widget_pointer>() ?
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->margins() :
			iPointerWrapper.is<layout_pointer>() ?
				static_variant_cast<const layout_pointer&>(iPointerWrapper)->margins() : 
				neogfx::margins{};
	}

	void layout::item::set_margins(const optional_margins& aMargins)
	{
		if (iPointerWrapper.is<widget_pointer>())
			static_variant_cast<const widget_pointer&>(iPointerWrapper)->set_margins(aMargins);
		else if (iPointerWrapper.is<layout_pointer>())
			static_variant_cast<const layout_pointer&>(iPointerWrapper)->set_margins(aMargins);
	}

	bool layout::item::visible() const
	{
		if (iPointerWrapper.is<widget_pointer>())
			return static_variant_cast<const widget_pointer&>(iPointerWrapper)->visible();
		else
			return true;
	}

	layout::device_metrics_forwarder::device_metrics_forwarder(i_layout& aOwner) :
		iOwner(aOwner)
	{
	}

	size layout::device_metrics_forwarder::extents() const
	{
		if (iOwner.owner() == 0)
			throw no_widget();
		return iOwner.owner()->device_metrics().extents();
	}

	dimension layout::device_metrics_forwarder::horizontal_dpi() const
	{
		if (iOwner.owner() == 0)
			throw no_widget();
		return iOwner.owner()->device_metrics().horizontal_dpi();
	}

	dimension layout::device_metrics_forwarder::vertical_dpi() const
	{
		if (iOwner.owner() == 0)
			throw no_widget();
		return iOwner.owner()->device_metrics().vertical_dpi();
	}

	dimension layout::device_metrics_forwarder::em_size() const
	{
		if (iOwner.owner() == 0)
			throw no_widget();
		return iOwner.owner()->device_metrics().em_size();
	}

	layout::layout(i_widget& aParent) :
		iOwner(&aParent),
		iDeviceMetricsForwarder(*this), iUnitsContext(iDeviceMetricsForwarder),
		iSpacing(app::instance().current_style().default_spacing()),
		iEnabled(true),
		iMinimumSize{},
		iMaximumSize{},
		iLayoutStarted(false)
	{
		aParent.set_layout(*this);
	}

	layout::layout(i_layout& aParent) :
		iOwner(aParent.owner()), 
		iDeviceMetricsForwarder(*this), iUnitsContext(iDeviceMetricsForwarder),
		iMargins(neogfx::margins(0)),
		iSpacing(app::instance().current_style().default_spacing()),
		iEnabled(true),
		iMinimumSize{},
		iMaximumSize{},
		iLayoutStarted(false)
	{
		aParent.add_layout(*this);
	}

	i_widget* layout::owner() const
	{
		return iOwner;
	}

	void layout::set_owner(i_widget* aOwner)
	{
		iOwner = aOwner;
	}

	void layout::add_widget(i_widget& aWidget)
	{
		if (aWidget.has_layout() && &aWidget.layout() == this)
			throw widget_already_added();
		iItems.push_back(item(aWidget));
		if (iOwner != 0)
			iItems.back().set_owner(iOwner);
	}

	void layout::add_widget(std::shared_ptr<i_widget> aWidget)
	{
		if (aWidget->has_layout() && &aWidget->layout() == this)
			throw widget_already_added();
		iItems.push_back(item(aWidget));
		if (iOwner != 0)
			iItems.back().set_owner(iOwner);
	}

	void layout::add_layout(i_layout& aLayout)
	{
		iItems.push_back(item(aLayout));
		if (iOwner != 0)
			iItems.back().set_owner(iOwner);
	}

	void layout::add_layout(std::shared_ptr<i_layout> aLayout)
	{
		iItems.push_back(item(aLayout));
		if (iOwner != 0)
			iItems.back().set_owner(iOwner);
	}

	void layout::add_spacer(i_spacer& aSpacer)
	{
		iItems.push_back(item(aSpacer));
		if (iOwner != 0)
			iItems.back().set_owner(iOwner);
		aSpacer.set_parent(*this);
	}

	void layout::add_spacer(uint32_t aPosition, i_spacer& aSpacer)
	{
		auto i = iItems.insert(std::next(iItems.begin(), aPosition), item(aSpacer));
		if (iOwner != 0)
			i->set_owner(iOwner);
		aSpacer.set_parent(*this);
	}

	void layout::add_spacer(std::shared_ptr<i_spacer> aSpacer)
	{
		iItems.push_back(item(aSpacer));
		if (iOwner != 0)
			iItems.back().set_owner(iOwner);
		aSpacer->set_parent(*this);
	}

	void layout::add_spacer(uint32_t aPosition, std::shared_ptr<i_spacer> aSpacer)
	{
		auto i = iItems.insert(std::next(iItems.begin(), aPosition), item(aSpacer));
		if (iOwner != 0)
			i->set_owner(iOwner);
		aSpacer->set_parent(*this);
	}

	void layout::remove_item(std::size_t aIndex)
	{
		iItems.erase(std::next(iItems.begin(), aIndex));
		if (iOwner != 0)
			iOwner->ultimate_ancestor().layout_items(true);
	}

	std::size_t layout::item_count() const
	{
		return iItems.size();
	}

	i_geometry& layout::get_item(std::size_t aIndex)
	{
		auto item = std::next(iItems.begin(), aIndex);
		if (item->get().is<item::widget_pointer>())
			return *static_variant_cast<item::widget_pointer&>(item->get());
		else if (item->get().is<item::layout_pointer>())
			return *static_variant_cast<item::layout_pointer&>(item->get());
		else
			throw wrong_item_type();
	}

	i_widget& layout::get_widget(std::size_t aIndex)
	{
		auto item = std::next(iItems.begin(), aIndex);
		if (item->get().is<item::widget_pointer>())
			return *static_variant_cast<item::widget_pointer&>(item->get());
		else
			throw wrong_item_type();
	}

	i_layout& layout::get_layout(std::size_t aIndex)
	{
		auto item = std::next(iItems.begin(), aIndex);
		if (item->get().is<item::layout_pointer>())
			return *static_variant_cast<item::layout_pointer&>(item->get());
		else
			throw wrong_item_type();
	}

	bool layout::has_margins() const
	{
		return iMargins != boost::none;
	}

	margins layout::margins() const
	{
		return units_converter(*this).from_device_units(has_margins() ? *iMargins : app::instance().current_style().default_margins());
	}

	void layout::set_margins(const optional_margins& aMargins)
	{
		optional_margins oldMargins = iMargins;
		iMargins = (aMargins != boost::none ? units_converter(*this).to_device_units(*aMargins) : optional_margins());
		if (iMargins != oldMargins && iOwner != 0)
			iOwner->ultimate_ancestor().layout_items(true);
	}

	size layout::spacing() const
	{
		return units_converter(*this).from_device_units(iSpacing);
	}

	void layout::set_spacing(dimension aSpacing)
	{
		set_spacing(size(aSpacing, aSpacing));
	}

	void layout::set_spacing(size aSpacing)
	{
		if (iSpacing != aSpacing)
		{
			iSpacing = units_converter(*this).to_device_units(aSpacing);
			if (iOwner != 0)
				iOwner->ultimate_ancestor().layout_items(true);
		}
	}

	void layout::enable()
	{
		if (iEnabled == false)
		{
			iEnabled = true;
			owner()->layout_items();
		}
	}

	void layout::disable()
	{
		if (iEnabled == true)
		{
			iEnabled = false;
		}
	}

	bool layout::enabled() const
	{
		return iEnabled;
	}

	bool layout::has_minimum_size() const
	{
		return iMinimumSize != boost::none;
	}

	size layout::minimum_size() const
	{
		return has_minimum_size() ?
			units_converter(*this).from_device_units(*iMinimumSize) :
			size{};
	}

	void layout::set_minimum_size(const optional_size& aMinimumSize, bool aUpdateLayout)
	{
		if ((iMinimumSize == boost::none && aMinimumSize != boost::none) ||
			(iMinimumSize != boost::none && aMinimumSize == boost::none) ||
			(iMinimumSize != boost::none && *iMinimumSize != units_converter(*this).to_device_units(*aMinimumSize)))
		{
			iMinimumSize = aMinimumSize != boost::none ? units_converter(*this).to_device_units(*aMinimumSize) : optional_size();
			if (iOwner != 0 && aUpdateLayout)
				iOwner->ultimate_ancestor().layout_items(true);
		}
	}

	bool layout::has_maximum_size() const
	{
		return iMaximumSize != boost::none;
	}

	size layout::maximum_size() const
	{
		return has_maximum_size() ?
			units_converter(*this).from_device_units(*iMaximumSize) :
			size(std::numeric_limits<size::dimension_type>::max(), std::numeric_limits<size::dimension_type>::max());
	}

	void layout::set_maximum_size(const optional_size& aMaximumSize, bool aUpdateLayout)
	{
		if ((iMaximumSize == boost::none && aMaximumSize != boost::none) ||
			(iMaximumSize != boost::none && aMaximumSize == boost::none) ||
			(iMaximumSize != boost::none && *iMaximumSize != units_converter(*this).to_device_units(*aMaximumSize)))
		{
			iMaximumSize = aMaximumSize != boost::none ? units_converter(*this).to_device_units(*aMaximumSize) : optional_size();
			if (iOwner != 0 && aUpdateLayout)
				iOwner->ultimate_ancestor().layout_items(true);
		}
	}

	bool layout::is_fixed_size() const
	{
		return has_minimum_size() && minimum_size() == maximum_size();
	}

	void layout::set_fixed_size(const optional_size& aFixedSize, bool aUpdateLayout)
	{
		set_minimum_size(aFixedSize, aUpdateLayout);
		set_maximum_size(aFixedSize, aUpdateLayout);
	}

	const i_device_metrics& layout::device_metrics() const
	{
		return iDeviceMetricsForwarder;
	}

	units_e layout::units() const
	{
		return iUnitsContext.units();
	}

	units_e layout::set_units(units_e aUnits) const
	{
		return iUnitsContext.set_units(aUnits);
	}

	const layout::item_list& layout::items() const
	{
		return iItems;
	}

	layout::item_list& layout::items()
	{
		return iItems;
	}

	uint32_t layout::spacer_count() const
	{
		uint32_t count = 0u;
		for (const auto& i : iItems)
			if (i.get().is<item::spacer_pointer>())
				++count;
		return count;
	}

	uint32_t layout::items_visible(item_type_e aItemType) const
	{
		uint32_t count = 0u;
		for (const auto& i : iItems)
			if (i.visible())
			{
				if ((aItemType & ItemTypeWidget) && i.get().is<item::widget_pointer>())
					++count;
				else if ((aItemType & ItemTypeLayout) && i.get().is<item::layout_pointer>())
					++count;
				else if ((aItemType & ItemTypeSpacer) && i.get().is<item::spacer_pointer>())
					++count;
			}
		return count;
	}
}