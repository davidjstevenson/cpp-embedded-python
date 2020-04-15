# Basic C++ project that embeds Python

## Purpose
The purpose of this code is to document the process of embedding python code within a C++ project. At the time of writing there didn't seem to be a good all inclusive guide as to how to do this so, these are essentialy my notes gathered from various locations on the web and a lot of trial and error.

This was developed under Windows with Visual Studio 2019. I do plan on extending this to other plaforms but for now there are hard coded assumptions in many places the require Windows. I have tested this with Python 3.8.1, although I assume it will work with other recent version of Python.

Note: This file outlines a way to distribute python modules with an application. Note thought has been given to the legality and license requirements of this process - *that is on you!* Only distribute that which you have permission to distribute and in a way that complies with the individual licenses.

## Goal
The goal is to get a C++ project that can run an totally isolated instance of python shipped aloneside the application itself. As well as this the python instance must be able to import modules including third party modules (such as requests).

For this example project I have resisted adding other C++ dependencies such as `pybind11` or `boost::python` - however for anything more than a trivial example it would be highly recommended to use one of these helper libraries. Very little about the overall setup would change other than the actual calls to run the python code itself. The work around in the header file `wrap_python.h` is taken care of by `boost::python` with CMake options.

## Disclaimer
I have had very little experience with the python C-API and embedding python. As such I don't know if this is the best way of doing it. Any suggestions, critques and pull requests welcome!

## Prerequisites
- The normal C++ compilation requirements (assumed knowledge)
- A full install of Python (for example one of these)
    - https://www.python.org/ftp/python/3.8.1/python-3.8.1-amd64.exe
    - https://www.python.org/ftp/python/3.8.1/python-3.8.1-amd64-webinstall.exe
- A copy of the embedded python zip file (for example) this:
    - https://www.python.org/ftp/python/3.8.1/python-3.8.1-embed-amd64.zip
- CMake

## Setup
### CMake
First configure and generate a makefile through CMake. You may need to adjust the configuration 'Python_ROOT_DIR' to the directory in which python is installed

### Bootstrapping
There is a python script `bootstrap.py` which documents the process of turning the embedded python zip file into a set of files that can be distributed with the compiled application.

`bootstrap.py` is not intenteded to be a rebust production ready script - rather to illustrate the step required. Like the C++ code I have avoided any external dependencies such as `click` for arg parsing (the `main()` function is not good practice but its done like this for simplicity and demonstration purposes). These steps are as follows

1. Extract the zip file
2. Install pip
3. Modify `python38._pth` (or equiv) to include the `import site`
4. Install any pip modules that are required (not part of `bootstrap.py`)
5. Create a zipfile of the installed modules
6. Copy of required files to a distributable folder

Note that all `python .\bootstrap.py ...` commands can be run from the normal installed python instance.

#### 1. Extract the zip file
This is self explanitory. bootstrap.py will extract if for you, just place the zip file into the folder `<REPO>/embedded-python`
```
> python .\bootstrap.py extract
```

#### 2. Install pip
As noted by the embedded zip distribution, pip is not supported in this environment. However if installed it does appear to work. Your milage may vary.

Pip can be installed through a script downloaded from https://bootstrap.pypa.io/get-pip.py. See the documentation here: https://pip.pypa.io/en/stable/installing/.

Note that any script downloaded from the internet should be inspected before run, especially one that contains binary data such as this! Please don't take my word for it and check this script yourself.

`bootstrap.py` will run this file for you, just download it to the folder `<REPO>/embedded-python`. If running this manually, make sure to run it with the embedded python executable from the extracted embedded zip file.
```
> python .\bootstrap.py pip
```

The result of this is the addition of two folder in the extracted python folder `<REPO>/embedded-python/python-3.8.1-embed-amd64/Lib` and (`Scripts`), however the Scripts folder is not useful in this context.

#### 3. Modify python38._pth
In order for the installed pip to be useable, the embedded python environment must know to load modules from the `Libs/site-packages` directory. This can be down by uncommenting the line `#import site` in the `python38._pth` (or equiv for you python version) file.

Note that the `python .\bootstrap.py pip` command from step 2 does this for you.

#### 4. Install any pip modules that are required
To use pip, you can run the pip executables from the `Scripts` folder or load the pip module through the embedded python executable. For instance
```
> embedded-python\python-3.8.1-embed-amd64\python.exe -m pip install requests
```

#### 5. Create a zipfile of the installed modules
It would be possible (and may in some cases be required) to just distribute the whole `site-packages` folder with your application and add its path to the `sys.path` within the embedded python instance. However, its much cleaner to distribute these modules as a zipfile in the same way as the embedded python instance ships its standard library (python38.zip). My understanding of this is that the contents of this zip file are not compressed (think a uncompressed tar archive of the unix world) with a TOC such that files can be read directly from byte offsets make it extremely fast to access.

In order to make such a file there is a class `PyZipFile` in the python `zipfile` module. Docs here: https://docs.python.org/3/library/zipfile.html

Using this `PyZipFile` will bundle (and compile where required) *.pyc files. Any other files required by a module must be included by hand (and known about). This is where it could get tricky for some modules. One such case is shown in `bootstrap.py` as a dependency of the `requests` module. `certifi` requires a file called `cacert.pem` which is manually included into the resulting `lib.zip`.

```
> python .\bootstrap.py make_zip
```

Like when installing `pip`, for python on pick up the bundled modules in `lib.zip` this file must either be manually added to `sys.path` or can be mentioned in the `python38._pth` (or equiv) file (this is where the standard library is included)

#### 6. Copy of required files to a distributable folder
This step is not strictly required but makes the CMake scripts a lot easier to write. This just copies all the files from the extracted folder into a seperate folder excluding the folders that the pip install created (we don't need these as the installed modules are now part of `lib.zip`)

Many of these files may in fact not be required (such as `python.exe`) and depending on what is indended to function within python some of the dlls may be omitted as well.

```
> python .\bootstrap.py make_python
```

## Building
This project should build in a similar way to any CMake project.

## Todo
I plan on extending this example and documentation to Linux (and maybe OSX) in the future.
