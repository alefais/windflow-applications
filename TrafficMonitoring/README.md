# Compile and run Traffic Monitoring application

## Compile
From inside the root directory `TrafficMonitoring/`

* Run `make test_main_map` in order to compile only the version of the application that uses the Map basic operator of WindFlow
<!--
* Run `make test_main_keyfarm` in order to compile only the version of the application that uses the Keyed Farm window-based operator of WindFlow
-->
* Run `make` in order to compile all the tests

## Run
TrafficMonitoring application can be run passing some arguments:<ul><li>`--nsource` source parallelism degree</li><li>`--nmatcher` map matcher node parallelism degree</li><li>`--ncalculator` speed calculator node parallelism degree</li><li>`--nsink` sink parallelism degree</li><li>`--rate` source generation rate (-1 to generate at the max possible rate)</li></ul> Instead of specifying the parallelism degree for each single node, it's possible to use `--pardeg` parallelism degree in order to assign the same parallelism degree to all the nodes in the topology.

Two datasets are currently supported:
* Beijing city taxi-traces
* Dublin city bus-traces

The city can be selected by setting the `_monitored_city` parameter and the dataset file path in in [constants.hpp](https://github.com/alefais/windflow-applications/blob/master/TrafficMonitoring/includes/util/constants.hpp). Default values are `_monitored_city = BEIJING` and `_beijing_input_file = "../data/taxi-traces.csv"`.

### Execution examples:
* Run the version with Map operator specifying explicitly the parallelism degree of all the nodes in the system and the generation rate (expressed in tuple/s): <br> `./main_map --nsource 1 --nmatcher 1 --ncalculator 1 --nsink 1 --rate -1`
<!--
* Run the version with Keyed Farm operator specifying the same parallelism degree for all the nodes in the system and the generation rate (expressed in tuple/s): <br> `./main_keyfarm --file ~/data/app/sd/sensors.dat --pardeg 2 --rate 1000000`
-->
<b>NB:</b> Running the application as `./main_map --help` visualizes all the parameter options.
<!-- 
	or as `./main_keyfarm --help`
--> 

<b>NB:</b> It is possible to set `FF_BOUNDED_BUFFER` FastFlow macro in the provided Makefile in order to use bounded queues instead of the default unbounded ones.

<b>NB:</b> The current implementation exploits the chaining of the operators supported by WindFlow. Chaining can be performed only if the operators have the same parallelism degree, in which case the chained operators will be executed within the same thread. In the SpikeDetection topology <strong>average calculator</strong>, <strong>detector</strong> and <strong>sink</strong> are chained together if it is possible (if they all have the same parallelism degree).
