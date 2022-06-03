#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <iomanip>

#include "src/neuron.h"

void write_to_file(std::ofstream &stream, const std::string& name, std::vector<double> values, bool last_line = false) {
    stream << "  \"" << name << "\": [";
    for(int i = 0; i < values.size(); ++i) {
        stream << std::setprecision(15) << values[i];
        if(i < values.size() - 1) {
            stream << ", ";
        }
    }
    stream << "]";
    if(!last_line) {
        stream << ",\n";
    } else {
        stream << "\n";
    }
}

uint64_t get_current_milliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
    std::random_device dev;
    std::mt19937 engine(dev());
    std::uniform_real_distribution<double> dist(0, 1);

    std::vector<double> potential;
    std::vector<double> sodium;
    std::vector<double> potassium;

    uint64_t start_time = get_current_milliseconds();

    std::vector<Neuron> neurons(1);
    int i = 0;
    while(true) {
        int n = 0;
        for(Neuron &neuron : neurons) {
            if(n == 0) {
                potential.push_back(neuron.getPotential());
                sodium.push_back(neuron.getSodiumIon());
                potassium.push_back(neuron.getPotassiumIon());
            }

            auto mole = (double) (dist(engine) * 5);
            neuron.influxSodium(mole);
            neuron.tick();
            ++n;
        }

        uint64_t stop_time = get_current_milliseconds();
        if(stop_time - start_time >= 1000) {
            std::cout << (double) (stop_time - start_time) / 1000 << "s elapsed" << std::endl;
            break;
        }
        ++i;
    }
    std::ofstream fs("spikes.json");
    if(fs.is_open()) {
        fs << "{\n";
        write_to_file(fs, "$P$", potential);
        write_to_file(fs, "$Na^+_{IN}$", sodium);
        write_to_file(fs, "$K^+_{IN}$", potassium, true);
        fs << "}\n";
    }

    std::cout << "Number of histories: " << potential.size() << std::endl;
    std::cout << "Number of spikes: " << neurons[0].mSpikes << std::endl;
    std::cout << "Membrane potential: " << std::setprecision(15) << neurons[0].getPotential() << std::endl;
    return 0;
}
