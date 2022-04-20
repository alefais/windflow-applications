# Benchmarking Applications - WindFlow version

This repository contains a collection of Data Stream Processing applications implemented using the [WindFlow](https://github.com/ParaGroup/WindFlow) library. WindFlow is a C++17 library for parallel stream processing on shared-memory architectures featuring multi-core CPUs and GPU devices. Further information on the usage and the operators provided by the WindFlow API can be found in the [official documentation](https://paragroup.github.io/WindFlow/).

Two more implementations of these applications have been provided using [Apache Storm](http://storm.apache.org/) and [Apache Flink](https://ci.apache.org/projects/flink/flink-docs-release-1.7/) frameworks. The code can be found respectively in the [storm-applications](https://github.com/alefais/storm-applications) repository and in the [flink-applications](https://github.com/alefais/flink-applications) repository.


## Dependencies 
In order to run the applications contained in this project, the following dependencies are needed:
* GCC (GNU Compiler Collection) version >= 7.2
* WindFlow library version 1.0.0 -> see [here](https://github.com/ParaGroup/WindFlow/tags)
* FastFlow library version >= 3.0 -> see [here](https://sourceforge.net/projects/mc-fastflow/)
* ALGLIB library version 3.15.0 (released 2019/02/21) -> it has been already included in this project. More details about the library can be found in the [official site](https://www.alglib.net/). In the context of this project, the functionalities contained in the [Descriptive Statistics](https://www.alglib.net/statistics/descriptive.php) package have been used. 
* [GDAL library](https://staging.www.osgeo.org/projects/gdal/) version 2.4.0 (released 2018/12/14) -> see [here](https://download.osgeo.org/gdal/2.4.0/)

