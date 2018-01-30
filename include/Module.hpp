/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/mw/CoreModule.hpp>
#include <ModuleConfiguration.hpp>

#undef ADC // Workaround for ST header files macro

namespace core {
namespace ADS126x_driver {
class ADS1262;
}
}

class Module:
    public core::mw::CoreModule
{
public:
// --- DEVICES ----------------------------------------------------------------
    static core::ADS126x_driver::ADS1262& adc;
// ----------------------------------------------------------------------------

    static bool
    initialize();


    Module();
    virtual ~Module() {}
};
