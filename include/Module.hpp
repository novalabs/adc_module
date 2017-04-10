/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/mw/CoreModule.hpp>
#include <ModuleConfiguration.hpp>

#undef ADC // Workaround for ST header files macro

#if CORE_USE_CONFIGURATION_STORAGE
namespace core {
namespace mw {
class CoreConfigurationStorage;
}
}
#endif
namespace core {
namespace ADS1262_driver {
class ADS1262;
}
}

class Module:
   public core::mw::CoreModule
{
public:
   static bool
   initialize();

   using ADC = core::ADS1262_driver::ADS1262;

// --- DEVICES ----------------------------------------------------------------
   static ADC& adc;
// ----------------------------------------------------------------------------

#if CORE_USE_CONFIGURATION_STORAGE
   static core::mw::CoreConfigurationStorage& configurationStorage;
#endif

   Module();
   virtual ~Module() {}
};
