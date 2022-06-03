# Biological Neurons

This project contains an implementation of biologically nearly-accurate neuron model. 
The model have imitated from interneurons in Hippocampus and Basal Ganglia.
Currently, the model only have the cell itself. Such things like synapses, dendrites and axons are not yet implemented.

The goal of this project is generalizing the biological cells in a computational form. 


The model implements several biological gates:
- [Positive Potassium Ion Leakage Channel](https://en.wikipedia.org/wiki/Potassium_channel)
- [Positive Sodium Ion Leakage Channel](https://en.wikipedia.org/wiki/Sodium_channel)
- [Potassium-Sodium Ion Pump](https://en.wikipedia.org/wiki/Sodium–potassium_pump)
- [Voltage-gated Potassium Ion Channel](https://en.wikipedia.org/wiki/Voltage-gated_potassium_channel)
- [Voltage-gated Sodium Ion Channel](https://en.wikipedia.org/wiki/Voltage-gated_ion_channel#Sodium_(Na+)_channels)

To simulate scientifically accurate [Resting Potential](https://en.wikipedia.org/wiki/Resting_potential), 
It uses [Equilibrium Potential](https://www.d.umn.edu/~jfitzake/Lectures/DMED/IonChannelPhysiology/MembranePotentials/EquilibriumPotentials.html) that results from [Nernst Equation](https://en.wikipedia.org/wiki/Nernst_equation).

To promote [Action Potential](https://en.wikipedia.org/wiki/Action_potential), such parameters like cell size and number of voltage-gated sodium ion channels are required.
Due to limitations of computational resource, however, I have decided to use hard-coded [Threshold Potential](https://en.wikipedia.org/wiki/Threshold_potential) which is -55mV.


I would appreciate someone who have deep-knowledge in Neuroscience can help this project.


### Building and Running

```bash
# Build
$ git clone https://github.com/OrigamiDream/biological-neurons.git
$ cd biological-neurons
$ cmake .
$ make

# Run and simulate
$ ./biological_neurons
```

This will simulate the randomly potentiated spikes for 1 second. <br>
Find `spikes.json` file to check simulated results. You can plot a graph with the JSON-formatted data.

### Plotting with Python
```python
import matplotlib.pyplot as plt
import json

with open('spikes.json', 'r') as f:
    histories = json.load(f)

plt.plot(figsize=(14, 7), facecolor='white')
for name, p in histories.items():
    plt.plot(p, label=name)
plt.legend(loc='upper right')
plt.show()
```