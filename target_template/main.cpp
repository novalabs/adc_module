/* ADC Module template file
 *
 * ADCNode.cpp contains a sample node you can start from.
 */

#include <ModuleConfiguration.hpp>
#include <Module.hpp>

// --- BOARD IMPL -------------------------------------------------------------
#include <core/ADS1262_driver/ADS1262.hpp>

// --- MODULE -----------------------------------------------------------------
Module module;

// *** DO NOT MOVE THE CODE ABOVE THIS COMMENT *** //

// --- MESSAGES ---------------------------------------------------------------
#include <core/common_msgs/Led.hpp>

// --- NODES ------------------------------------------------------------------
#include <core/led/Subscriber.hpp>
#include "ADCNode.hpp"

// --- TYPES ------------------------------------------------------------------

// --- CONFIGURATIONS ---------------------------------------------------------
core::led::SubscriberConfiguration led_subscriber_configuration_default;

// --- NODES ------------------------------------------------------------------
core::led::Subscriber led_subscriber("led_sub", core::os::Thread::PriorityEnum::LOWEST);

// --- DEVICE CONFIGURATION ---------------------------------------------------

// --- MAIN -------------------------------------------------------------------
extern "C" {
    int
    main()
    {
        module.initialize();

        // Device configurations

        // Default configuration
        led_subscriber_configuration_default.topic = "led";

        // Add configurable objects to the configuration manager...
        module.configurations().add(led_subscriber, led_subscriber_configuration_default);

        // ... and load the configuration
        module.configurations().loadFrom(module.configurationStorage());

        // Add nodes to the node manager...
        module.nodes().add(led_subscriber);

        // ... and let's play!
        module.nodes().setup();
        module.nodes().run();

        // Create a simple node
        core::os::Thread::create_heap(NULL, 4096, core::os::Thread::PriorityEnum::NORMAL, adcNodeThread, NULL);

        // Is everything going well?
        for (;;) {
            if (!module.nodes().areOk()) {
                module.halt("This must not happen!");
            }

            core::os::Thread::sleep(core::os::Time::ms(500));

            // Remember to feed the (watch)dog!
            module.keepAlive();
        }

        return core::os::Thread::OK;
    } // main
}
