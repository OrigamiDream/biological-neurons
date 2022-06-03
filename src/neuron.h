//
// Created by Avis Studios on 2022/05/26.
//

#ifndef BIOLOGICAL_NEURONS_NEURON_H
#define BIOLOGICAL_NEURONS_NEURON_H

#include <algorithm>
#include <cmath>

#include "config.h"

class Substance {

public:
    Substance(double potassiumIon, double sodiumIon);

    ~Substance() = default;

    [[nodiscard]] double getPotassiumIon() const;
    double setPotassiumIon(double potassiumIon);

    [[nodiscard]] double getSodiumIon() const;
    double setSodiumIon(double sodiumIon);

    [[nodiscard]] double getElectricalPotential() const;

private:
    double mPotassiumIon;
    double mSodiumIon;

};

class Neuron {

public:
    Neuron();

    ~Neuron() = default;

    static double clipReflectiveByValue(double value, double from_value, double to_value) {
        if(value > 0) {
            return std::min(std::max(value, from_value), to_value);
        } else if(value < 0) {
            return std::min(std::max(value, to_value * -1), from_value * -1);
        }
        return value;
    }

    [[nodiscard]] double getPotential() const {
        return mIntracellular.getElectricalPotential() - mExtracellular.getElectricalPotential();
    }

    [[nodiscard]] double getSodiumIon() const {
        return mIntracellular.getSodiumIon();
    }

    [[nodiscard]] double getPotassiumIon() const {
        return mIntracellular.getPotassiumIon();
    }

    bool influxPotassium(double num);
    bool influxSodium(double num);

    void tick();

    void performPotassiumIonLeakageChannel();
    void performSodiumIonLeakageChannel();
    void performPotassiumSodiumIonPump();
    void performVoltageGatedSodiumIonChannel();
    void performVoltageGatedPotassiumIonChannel();

    int mSpikes = 0;

private:
    Substance mIntracellular;
    Substance mExtracellular;

    double mSumOfSodiumConcentration;

    double mPotassiumVoltageGateElapsed = 0;
    bool mSodiumVoltageGateInactivated = false;

};


#endif //BIOLOGICAL_NEURONS_NEURON_H
