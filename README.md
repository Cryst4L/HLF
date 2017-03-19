# HLF: A Machine Learning Playground

## What is this?

HLF stands for Hand Learning Framework. It is a C++ tool for prototyping and testing supervised Machine Learning models, using a specific vision problem: infering an articulated hand pose from a single 2D snapshot of it. It is for the most a fun tool, so don't get too hyped ! :)

<p align="center">
  <img src="https://github.com/Cryst4L/HLF/blob/master/Framework/Assets/hlf-config.gif"/>
</p>

#### Project Architecture

The project is built around a dataset generation tool, which render the snapshots of the hand model in random poses. The hand pose itself is expressed as a 10 degrees of freedom vector.

In addtion, it integrates a benchmarking pipeline to evaluate the user's model. This pipeline first calls the data generation tool, then run the user's model training routine, and finally tests it's performances.

HLF also comes with its own display tools (based on SFML) for configuration and visualization. Hence the user only needs to code his Machine Learning model before he calls the pipeline, which runs in a (minimal) GUI environement.

#### The Learning Task

From a ML point of view, this problem is very similar to (and in fact was inspired by) the MNIST challenge, which is already a gold standard for protyping ML models. But with two main differencies:

- Data are synthetic :

Compared to the MNIST dataset, the samples are mechanically generated. So there is no human involved in the process, and thus you never feel cheated by the data when a predictor fails on some samples. It also means that you can arbitrary set the number of training samples, so you can push your model to its limits (the 50K sample of the MNSIT can be limitative sometimes).

<p align="center">
<img src="https://github.com/Cryst4L/HLF/blob/master/Framework/Assets/hlf-data.png"/>
</p>

- It's a regression problem :

In HLF the task is a regression: You must infer the hand pose as a vector of 10 variables (degrees of freedom) taking their ranges in 0-1. Hence a straight scoring of a predictor is the average L2 norm of the errors. By this mean we avoid a limitation of the MNIST, which occurs when a predictor reaches very high scores, and the discretisation of the error measure (the number of miss) can become problematic.


#### An Eigen3 Environement

Eigen3 is a very efficient and convenient C++ library for dealing with linear algebra, and HLF makes an extensive use of it, both to render the hand samples, and to represent the data (samples are Eigen3 matrices, targets are Eigen3 vectors). Hence the user of HLF is requested to implement his model using the Eigen3 library (eventhough its not mandatory).

<p align="center">
  <img src="https://github.com/Cryst4L/HLF/blob/master/Framework/Assets/hlf-bench.gif"/>
</p>


## How to use?

Its quite simple actually !

All you need to do is to implement a ML blackbox inheriting from HLF::Model, which requires you to fill two virtual methods as you can see it here :

```cpp
class MyModel : public HLF::Model
{
  private:
	// Training
    void train(std::vector <MatrixXd> &data, std::vector <VectorXd> &targets) {
		// Do stuff ...
	}
	// Inference
    VectorXd predict(MatrixXd &sample) {
		VectorXd prediction;
		// Do stuff ...
		return prediction;
	}
};
```
For convenience the models are implemented in a header-only style. Once this has been done, you then go to the _main.cpp_ file to call of the pipeline, as shown below :
```cpp
#include "DefaultPipeline.h"
#include "Examples/MyModel.h"
int main(void) {
	HLF::DefaultPipeline <MyModel> ();
    return 0;
}
```
<p align="center">
  <img src="https://github.com/Cryst4L/HLF/blob/master/Framework/Assets/hlf-bench.gif"/>
</p>

Then you compile the project in your _Build_ directory:
```sh
cmake .. # first time only
make
```
And that's all ! Some basic examples are already stored int the _"Examples"_ folder, so you might want to have a look at it ;)

## Dependencies

As explained earlier, HLF makes an extensive use of:

- The Simple and Fast Multimedia Library: **sudo apt-get install libsfml-dev**
- The Eigen3 linear algebra library: **sudo apt-get install libeigen3-dev**

Not to mention this project has been developed and tested only in a Linux (Xubuntu) environement. So having a recent Linux machine is _infine_ required :)

## Example Results

Here are some results of the benchmarked models included in the _"Examples"_ folder.
The dataset configurations used are the default one (size=32; number=25K; seed=1234).

<p align="center">

| Model                                      | L2 Cost (Train) | L2 Cost (Test)  |
|--------------------------------------------|----------------:|----------------:|
| Nearest Neighbour (vanilla)                |              (0)|             0.83|
| MultiLinear Regression (with bias)         |             0.75|             0.76|
| PCA features (50) + Quadratic Classifier   |             0.56|         **0.59**|
| BP Neural Net (3 Hidden Layer x 50 Units)  |             0.61|             0.64|

</p>

## Temporal Consistency and Inference

I realized while developing this project that it would be a quite nice environnement for testing temporal ML models,
such as HMMs or RNNs. Nevertheless it was initially thought to evaluate feed-forward models on a Machine Vision task, so it does not allow such things for now.

Nevertheless, all is needed to implement to make it happen is an alternate dataset generator, which build samples according to a continous random walk (similar to what is displayed in the "Configuration Panel"). So I may add this feature at some point.

## Portability, XCB and MultiThreading

First of all, this project works like a charm on it's dev machine, a 64bit desktop geared with a Intel (Skylake) CPU and a NVidia GPU, running under Ubuntu 16.04 LTS

Nevertheless it is known that fully threading multiple instances of the SFML simultaneously can limits the portability of one's project, because of the way it interacts with XCB and GLX. Hence, on certain Linux platforms you may encounter issues (such as random XCB crashs). If so, a workaround might be to upgrade your XCB using the following [script](https://gist.github.com/slimsag/a26d838ccc4480ce21bc#file-gistfile1-sh).

By the way if you manage to use this project in a different environement, feel free to give feedbacks, and propose modifications to both the sources and the CMakeLists ;)

This project is released under MIT license.
