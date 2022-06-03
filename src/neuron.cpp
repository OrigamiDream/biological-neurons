//
// Created by Avis Studios on 2022/05/26.
//

#include "neuron.h"

Substance::Substance(double potassiumIon,
                     double sodiumIon):

                     mPotassiumIon(potassiumIon),
                     mSodiumIon(sodiumIon) {
}

double Substance::getPotassiumIon() const {
    return mPotassiumIon;
}

double Substance::setPotassiumIon(double potassiumIon) {
    mPotassiumIon = potassiumIon;
    return mPotassiumIon;
}

double Substance::getSodiumIon() const {
    return mSodiumIon;
}

double Substance::setSodiumIon(double sodiumIon) {
    mSodiumIon = sodiumIon;
    return mSodiumIon;
}

double Substance::getElectricalPotential() const {
    return mPotassiumIon + mSodiumIon;
}

Neuron::Neuron():
        mIntracellular(110, 0),
        mExtracellular(45, 155),
        mSumOfSodiumConcentration(0) {
    mSumOfSodiumConcentration = mIntracellular.getSodiumIon() + mExtracellular.getSodiumIon();
}

bool Neuron::influxPotassium(double num) {
    if(num > 0) {
        if(mExtracellular.getPotassiumIon() < num) {
            return false;
        }
    } else if(num < 0) {
        if(mIntracellular.getPotassiumIon() < -num) {
            return false;
        }
    }

    mExtracellular.setPotassiumIon(mExtracellular.getPotassiumIon() - num);
    mIntracellular.setPotassiumIon(mIntracellular.getPotassiumIon() + num);
    return true;
}

bool Neuron::influxSodium(double num) {
    if(num > 0) {
        if(mExtracellular.getSodiumIon() < num) {
            return false;
        }
    } else if(num < 0) {
        if(mIntracellular.getSodiumIon() < -num) {
            return false;
        }
    }

    mExtracellular.setSodiumIon(mExtracellular.getSodiumIon() - num);
    mIntracellular.setSodiumIon(mIntracellular.getSodiumIon() + num);
    return true;
}

void Neuron::tick() {
    performPotassiumIonLeakageChannel();
    performSodiumIonLeakageChannel();
    performPotassiumSodiumIonPump();
    performVoltageGatedPotassiumIonChannel();
    performVoltageGatedSodiumIonChannel();
}

void Neuron::performPotassiumIonLeakageChannel() {
    double pIn = mIntracellular.getElectricalPotential();
    double pEx = mExtracellular.getElectricalPotential();
    double value;

    value = pEx - pIn;
    value = value + config::equilibrium_potential::potassium_ion;
    value = value / config::leakage_channel::amplifier;
    value = clipReflectiveByValue(value, config::leakage_channel::min_mole, config::leakage_channel::max_mole);
    influxPotassium(value);
}

void Neuron::performSodiumIonLeakageChannel() {
    double pIn = mIntracellular.getElectricalPotential();
    double pEx = mExtracellular.getElectricalPotential();
    double value;

    value = pEx - pIn;
    value = value + config::equilibrium_potential::sodium_ion;
    value = value / config::leakage_channel::amplifier;
    value = clipReflectiveByValue(value, config::leakage_channel::min_mole, config::leakage_channel::max_mole);
    influxSodium(value);
}

void Neuron::performPotassiumSodiumIonPump() {
    double num_pumps = config::potassium_sodium_pump::num_pumps;
    num_pumps = mIntracellular.getSodiumIon() / mSumOfSodiumConcentration * num_pumps;
    num_pumps = std::floor(num_pumps) / 100;

    const double amplifiers[] = {1, 0.1, 0.01, 0.001};
    for(const double amp : amplifiers) {
        double efflux = config::potassium_sodium_pump::num_sodium_ion_pump_out * amp;
        double influx = config::potassium_sodium_pump::num_potassium_ion_pump_in * amp;

        double max_pumps = std::floor(std::min(mIntracellular.getSodiumIon() / efflux, mExtracellular.getPotassiumIon() / influx));
        max_pumps = std::min(num_pumps * amp, max_pumps);
        if(max_pumps <= 0) {
            continue;
        }
        efflux = max_pumps * config::potassium_sodium_pump::num_sodium_ion_pump_out;
        influx = max_pumps * config::potassium_sodium_pump::num_potassium_ion_pump_in;
        if(mIntracellular.getSodiumIon() >= efflux && mExtracellular.getPotassiumIon() >= influx) {
            influxSodium(-1 * efflux);
            influxPotassium(influx);
        }
    }
}

void Neuron::performVoltageGatedPotassiumIonChannel() {
    double pIn = mIntracellular.getElectricalPotential();
    double pEx = mExtracellular.getElectricalPotential();
    double p = pIn - pEx;

    if(mPotassiumVoltageGateElapsed > 0) {
        double value;
        value = pEx - pIn;
        value = value + config::equilibrium_potential::potassium_ion;
        value = (value / config::voltage_gated_potassium_ion_channel::amplifier) * mPotassiumVoltageGateElapsed;
        value = clipReflectiveByValue(value, config::voltage_gated_potassium_ion_channel::min_mole, config::voltage_gated_potassium_ion_channel::max_mole);

        influxPotassium(value);
        mPotassiumVoltageGateElapsed = std::max(mPotassiumVoltageGateElapsed - config::voltage_gated_potassium_ion_channel::closing_speed, (double) 0);
    }

    if(p >= config::threshold_potential) {
        mPotassiumVoltageGateElapsed = std::min(mPotassiumVoltageGateElapsed + config::voltage_gated_potassium_ion_channel::opening_speed, config::voltage_gated_potassium_ion_channel::fully_opened);
    }
}

void Neuron::performVoltageGatedSodiumIonChannel() {
    double pIn = mIntracellular.getElectricalPotential();
    double pEx = mExtracellular.getElectricalPotential();
    double p = getPotential();
    if(p >= config::threshold_potential) {
        if(!mSodiumVoltageGateInactivated) {
            double value;
            value = pEx - pIn;
            value = value + config::equilibrium_potential::sodium_ion;

            influxSodium(clipReflectiveByValue(value, config::voltage_gated_sodium_ion_channel::min_mole, config::voltage_gated_sodium_ion_channel::max_mole));
            if(getPotential() >= config::equilibrium_potential::sodium_ion + config::voltage_gated_sodium_ion_channel::inactivation_threshold_offset_from_peak) {
                mSodiumVoltageGateInactivated = true;
                mSpikes++;
                // TODO: Propagate spikes to other cells
            }
        }
    } else if(mSodiumVoltageGateInactivated) {
        mSodiumVoltageGateInactivated = false;
    }
}