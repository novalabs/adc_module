#include <Module.hpp>
#include <core/ADS1262_driver/ADS1262.hpp>

#include <core/mw/Node.hpp>
#include <core/mw/Publisher.hpp>
#include <core/common_msgs/Float32.hpp>

#include "ADCNode.hpp"

void
adcNodeThread(
    void* args
)
{
    using ADC = Module::ADC;

    Module::adc.init();
    Module::adc.probe();
    Module::adc.configure();

    Module::adc.setReferenceMux(ADC::ReferenceMuxPositive::INTERNAL_2V5, ADC::ReferenceMuxNegative::INTERNAL_AVSS);
    Module::adc.setReferencePolarityReversal(false);

    Module::adc.setInputMux(ADC::InputMuxPositive::AIN_0, ADC::InputMuxNegative::AIN_1);

    Module::adc.setPGA(false, ADC::Gain::GAIN_1);

    Module::adc.setDataRate(ADC::DataRate::DATARATE_50);
    Module::adc.setFilter(ADC::Filter::SINC_3);
    Module::adc.setDelay(ADC::Delay::_278us);

    Module::adc.start();
    Module::adc.calibrateOffset();
    Module::adc.stop();

    int64_t _filteredValue = 0;
    int32_t _cnt = 0;

    core::mw::Node node("adc_thread");
    core::mw::Publisher<core::common_msgs::Float32> publisher;

    node.advertise(publisher, "adc0"); // This can be made configurable

    Module::adc.start();

    while (1) {
        Module::adc.wait(); // Wait for a new conversion
        Module::adc.update();

        auto status = Module::adc.getStatus();

        status.pgal_alm = 0; // Override alarms

        if (status & 0b01000000) {
            // We have a new sample...
            int32_t x = Module::adc.getRaw();

            _filteredValue += x; // Accumulate the new sample

            _cnt++;

            if (_cnt == 50) {
                _cnt = 0; // Reset the sample counter
            }

            if (_cnt == 0) {
                // A whole perdiod has been accumulated
                core::common_msgs::Float32* msgp;

                float tmp = (float)(_filteredValue / 50) * (2.5f / 0x7FFFFFFF);  // Take the average
                _filteredValue = 0; // Reset the accumulator

                if (publisher.alloc(msgp)) {
                    msgp->value = tmp;

                    if (!publisher.publish(*msgp)) {}
                }
            }
        }
    }
} // adcThread
