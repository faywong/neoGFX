/*
  plugin.cpp

  Copyright (c) 2020 Leigh Johnston.  All Rights Reserved.

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

#include <neogfx/neogfx.hpp>
#include <neolib/app/i_application.hpp>
#include <neolib/plugin/i_plugin.hpp>
#include <neogfx/tools/DesignStudio/element_library_plugin.hpp>
#include "library.hpp"

API void entry_point(neolib::i_application& aApplication, const neolib::i_string& aPluginFolder, neolib::i_ref_ptr<neolib::i_plugin>& aPluginInstance)
{
    neolib::set_service_provider(aApplication.service_provider());
    neolib::async_event_queue::instance().register_with_task(neolib::service<neolib::i_async_task>());
    using instance_t = neogfx::DesignStudio::element_library_plugin<neogfx::DesignStudio::neos_element_library>;
    aPluginInstance = neolib::make_ref<instance_t>(aApplication);
}

