# Compile and run Fraud Detection application

## Compile
From inside the root directory `FraudDetection/`

* Run `make test_heavy_source` in order to compile only the "heavy" source version of the application

* Run `make test_light_source` in order to compile only the "light" source version of the application

* Run `make` in order to compile all the tests

## Run
FraudDetection application can be run passing some arguments:<ul><li>`--nsource` source parallelism degree</li><li>`--npredictor` predictor node parallelism degree</li><li>`--nsink` sink parallelism degree</li><li>`--rate` source generation rate (-1 to generate at the max possible rate)</li></ul> Instead of specifying the parallelism degree for each single node, it's possible to use `--pardeg` parallelism degree in order to assign the same parallelism degree to all the nodes in the topology.

### Execution examples:
* Run the version with "heavy" source node specifying explicitly the parallelism degree of all the nodes in the system and the generation rate (expressed in tuple/s): <br> `./main_heavy_source --nsource 1 --npredictor 1 --nsink 1 --rate -1`

* Run the version with "light" source node specifying the same parallelism degree for all the nodes in the system and the generation rate (expressed in tuple/s): <br> `./main_light_source --pardeg 2 --rate 1000000`

<b>NB:</b> Running the application as `./main_heavy_source --help` or as `./main_light_source --help` visualizes all the parameter options.

<b>NB:</b> The path of the dataset input file containing credit card transactions (absolute or relative to `FraudDetection/` directory) and the prediction model parameters are defined and can be modified in [constants.hpp](https://github.com/alefais/windflow-applications/blob/master/FraudDetection/includes/util/constants.hpp).

<b>NB:</b> It is possible to set `FF_BOUNDED_BUFFER` FastFlow macro in the provided Makefile in order to use bounded queues instead of the default unbounded ones.

<b>NB:</b> The current implementation exploits the chaining of the operators supported by WindFlow. Chaining can be performed only if the operators have the same parallelism degree, in which case the chained operators will be executed within the same thread. In the FraudDetection topology <strong>predictor</strong> and <strong>sink</strong> are chained together if it is possible (if they have the same parallelism degree).
