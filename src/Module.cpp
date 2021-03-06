/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#include <core/snippets/CortexMxFaultHandlers.h>

#include <core/mw/Middleware.hpp>
#include <core/mw/transport/RTCANTransport.hpp>

#include <core/hw/GPIO.hpp>
#include <core/hw/EXT.hpp>
#include <core/hw/SPI.hpp>
#include <core/hw/IWDG.hpp>
#include <core/os/Thread.hpp>

#include <Module.hpp>

#include <core/ADS126x_driver/ADS126x.hpp>

// LED
using LED_PAD = core::hw::Pad_<core::hw::GPIO_F, GPIOF_LED>;
static LED_PAD _led;

// DEVICES
static core::hw::Pad_<core::hw::GPIO_A, 3>       _adc_reset;
static core::hw::Pad_<core::hw::GPIO_B, 10>      _adc_start;
static core::hw::EXTController_<core::hw::EXT_1> _ext;
static core::hw::EXTChannel_<core::hw::EXT_1, 11, EXT_CH_MODE_FALLING_EDGE | EXT_MODE_GPIOB> _adc_data_ready;
static core::hw::SPIDevice_<core::hw::SPI_1, core::hw::Pad_<core::hw::GPIO_A, 4> > _adc_spi;

// ADC
static core::ADS126x_driver::ADS1262 _ads1262(_adc_spi, _adc_data_ready, _adc_reset, _adc_start);

// MODULE DEVICES
core::ADS126x_driver::ADS1262& Module::adc = _ads1262;

// DEVICE CONFIG
static const SPIConfig _spi_config = {
    NULL, NULL, 0,
    SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_CPHA
};

static EXTConfig _ext_config = {    {
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL},
                                        {EXT_CH_MODE_DISABLED, NULL}
                                    }};

// SYSTEM STUFF
static core::os::Thread::Stack<1024> management_thread_stack;
static core::mw::RTCANTransport      rtcantra(&RTCAND1);

RTCANConfig rtcan_config = {
    1000000, 100, 60
};


Module::Module()
{}

bool
Module::initialize()
{
#ifdef _DEBUG
    FAULT_HANDLERS_ENABLE(true);
#else
    FAULT_HANDLERS_ENABLE(false);
#endif

    static bool initialized = false;

    if (!initialized) {
        core::mw::CoreModule::initialize();

        core::mw::Middleware::instance().initialize(name(), management_thread_stack, management_thread_stack.size(), core::os::Thread::LOWEST);
        rtcantra.initialize(rtcan_config, canID());
        core::mw::Middleware::instance().start();

        _ext.start(_ext_config);
        _adc_spi.start(_spi_config);

        initialized = true;
    }

    return initialized;
} // Board::initialize

// ----------------------------------------------------------------------------
// CoreModule STM32FlashConfigurationStorage
// ----------------------------------------------------------------------------
#include <core/snippets/CoreModuleSTM32FlashConfigurationStorage.hpp>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// CoreModule HW specific implementation
// ----------------------------------------------------------------------------
#include <core/snippets/CoreModuleHWSpecificImplementation.hpp>
// ----------------------------------------------------------------------------
