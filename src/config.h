//
// Created by Avis Studios on 2022/06/03.
//

#ifndef BIOLOGICAL_NEURONS_CONFIG_H
#define BIOLOGICAL_NEURONS_CONFIG_H

namespace config {
    namespace equilibrium_potential {
        const double potassium_ion = -90;
        const double sodium_ion = 65;
    }
    namespace leakage_channel {
        const int amplifier = 5;
        const double min_mole = 0.001;
        const double max_mole = 2;
    }
    namespace potassium_sodium_pump {
        const int num_sodium_ion_pump_out = 3;
        const int num_potassium_ion_pump_in = 2;
        const int num_pumps = 1000;
    }
    namespace voltage_gated_sodium_ion_channel {
        const int inactivation_threshold_offset_from_peak = -20;
        const double min_mole = 0.001;
        const double max_mole = 50;
    }
    namespace voltage_gated_potassium_ion_channel {
        const int amplifier = 15;
        const double min_mole = 0.001;
        const double max_mole = 50;
        const double opening_speed = 1;
        const double closing_speed = 0.5;
        const double fully_opened = 5;
    }
    const double threshold_potential = -55;
}

#endif //BIOLOGICAL_NEURONS_CONFIG_H
