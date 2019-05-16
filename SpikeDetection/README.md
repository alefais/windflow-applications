# Compile and run SpikeDetection

## Compile
From inside the root directory `SpikeDetection/`

* Run `make test_main_map` in order to compile only the version of the application that uses the Map basic operator of WindFlow

* Run `make test_main_keyfarm` in order to compile only the version of the application that uses the Keyed Farm window-based operator of WindFlow

* Run `make` in order to compile all the tests

## Run
SpikeDetection application can be run passing some arguments:<ul><li>`--file` path of the dataset input file containing sensors' measurements (absolute or relative to `SpikeDetection/` directory)</li><li>`--nsource` source parallelism degree</li><li>`--naverage` average calculator node parallelism degree</li><li>`--ndetector` detector node parallelism degree</li><li>`--nsink` sink parallelism degree</li><li>`--rate` source generation rate (-1 to generate at the max possible rate)</li></ul> Instead of specifying the parallelism degree for each single node, it's possible to use `--pardeg` parallelism degree in order to assign the same parallelism degree to all the nodes in the topology.

### Execution examples:
* Run the version with Map operator specifying explicitly the parallelism degree of all the nodes in the system and the generation rate (expressed in tuple/s): <br> `./main_map --file ~/data/app/sd/sensors.dat --nsource 1 --naverage 1 --ndetector 1 --nsink 1 --rate -1`

* Run the version with Keyed Farm operator specifying the same parallelism degree for all the nodes in the system and the generation rate (expressed in tuple/s): <br> `./main_keyfarm --file ~/data/app/sd/sensors.dat --pardeg 2 --rate 1000000`

<b>NB:</b> Running the application as `./main_map --help` or as `./main_keyfarm --help` visualizes all the parameter options.

<b>NB:</b> It is possible to set `FF_BOUNDED_BUFFER` macro in the provided Makefile in order to use bounded queues instead of the default unbounded ones.
