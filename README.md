# Basic C++ project that embeds Python

## Purpose
The purpose of this code is to document the process of embedding python code within a C++ project. At the time of writing there didn't seem to be a good all inclusive guide as to how to do this so, these are essentialy my notes gathered from various locations on the web and a lot of trial and error.

This was developed under Windows with Visual Studio 2019. I do plan on extending this to other plaforms but for now there are hard coded assumptions in many places the require Windows. I have tested this with Python 3.8.1, although I assume it will work with other recent version of Python.

## Goal
The goal is to get a C++ project that can run an totally isolated instance of python shipped aloneside the application itself. As well as this the python instance must be able to import modules including third party modules (such as requests).

For this example project I have resisted adding other C++ dependencies such as pybind11 or boost::python - however for anything more than a trivial example it would be highly recommended to use one of these helper libraries. Very little about the overall setup would change other than the actual calls to run the python code itself.

## Prerequisites
- The normal C++ compilation requirements (assumed knowledge)
- A full install of Python
- A copy of the embedded python zip file
- CMake

## Setup
### CMake
First configure and generate a makefile through CMake. You may need to adjust the configuration 'Python_ROOT_DIR' to the directory in which python is installed

### Bootstrapping
TODO

## Todo
I plan on extending this example and documentation to Linux (and maybe OSX) in the future.
