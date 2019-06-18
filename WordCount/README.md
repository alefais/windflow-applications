# Compile and run WordCount

## Compile
From inside the root directory `WordCount` run `make` in order to compile the tests (at the end of the compilation, the executable file can be found in `WordCount/bin`).

## Run
WordCount application can be run passing some arguments:<ul><li>`--nsource` source parallelism degree</li><li>`--nsplitter` line splitter node parallelism degree</li><li>`--ncounter` word counter node parallelism degree</li><li>`--nsink` sink parallelism degree</li><li>`--rate` source generation rate (-1 to generate at the max possible rate)</li></ul> Instead of specifying the parallelism degree for each single node, it's possible to use `--pardeg` parallelism degree in order to assign the same parallelism degree to all the nodes in the topology.

### Execution examples:
* Run the application specifying explicitly the parallelism degree of all the nodes in the system and the generation rate (expressed in tuple/s), where -1 means the maximum possible rate: <br> `./main --nsource 1 --nsplitter 2 --ncounter 1 --nsink 1 --rate -1`

* Run the application specifying the same parallelism degree for all the nodes in the system and the generation rate (expressed in tuple/s), where -1 means the maximum possible rate: <br> `./main --pardeg 1 --rate -1`

<b>NB:</b> Running the application as `./main --help` visualizes all the parameter options.

<b>NB:</b> It is possible to set `FF_BOUNDED_BUFFER` FastFlow macro in the provided Makefile in order to use bounded queues instead of the default unbounded ones.

<b>NB:</b> The current implementation exploits the chaining of the operators supported by WindFlow. Chaining can be performed only if the operators have the same parallelism degree, in which case the chained operators will be executed within the same thread. In the WordCount topology <strong>word counter</strong> and <strong>sink</strong> are chained together if it is possible (if they all have the same parallelism degree).