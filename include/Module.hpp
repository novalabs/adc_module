/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/mw/CoreModule.hpp>
#include <ModuleConfiguration.hpp>

#undef ADC // Workaround for ST header files macro

namespace core {
namespace ADS1262_driver {
class ADS1262;
}
}

#if CORE_USE_CONFIGURATION_STORAGE
namespace core {
namespace mw {
class CoreConfigurationStorage;
}
}
#endif

class Module:
    public core::mw::CoreModule
{
public:
    using ADC = core::ADS1262_driver::ADS1262;

// --- DEVICES ----------------------------------------------------------------
    static ADC& adc;
// ----------------------------------------------------------------------------

    static bool
    initialize();


#if CORE_USE_CONFIGURATION_STORAGE
    static core::mw::CoreConfigurationStorage& configurationStorage;
#endif
    Module();
    virtual ~Module() {}
};
