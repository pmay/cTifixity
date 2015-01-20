# Tifixity

TIFF Image Data Checksummer

### What does X do?

Checksums the image data portion of a TIFF file, as opposed to the entire TIFF file. Useful 
for ensuring that the pixel data remains the same before and after some process.

## Features and roadmap

### Version 0.0.1

* Initial version

## How to install and use

### Requirements

To install you need:

* Cygwin (for Windows)
* Git client
* Cmake

### Build instructions

Note: Only tested on windows so far.
To download and build follow these steps:

```bash
$ git clone https://github.com/pmay/Tifixity.git
$ cd Tifixity
$ mkdir build
$ cd build
$ cmake -G "Unix Makefiles" ..
$ make
```

The cmake command will generate a Makefile in the build directory, which make uses to build the tifixity executable.
After successful completion, the tifixity executable will be in the build directory. 

### Use

To use the tool, you can try the following command (from the build directory):
```bash
$ ./tifixity ../test/rgbstrips.tiff
5478865efdfc945d291b584402d34a33
```

### Setup IDE

Tifixity was developed with Eclipse + CDT. To setup the Eclipse IDE with this project:

1. Start Eclipse
2. Select "File > Import". Then, select "C/C++ > Existing Code as Makefile Projects" and click "Next"
3. Give it an Eclipse Project Name, such as Tifixity
4. Provide your local repo directory as the Existing Code Location
5. Select the Cygwin GCC as the toolchain location (for Windows)
6. Click "Finish" to create the project.

To build within Eclipse, you'll need to set up a Make Target to perform the cmake command, as well as set the Project's default builder settings:

#### Make Target: cmake

1. Select "Window > Show View > Make Target" to view the Make Target side bar
2. Create a New Make Target
3. Give it a name, such as "Run CMake"
4. Ensure "Same as the target name" is unchecked and "Make target:" is blank
5. Under "Build Command", uncheck "Use builder settings" and set "Build command:" to cmake -E chdir build/ cmake -G "Unix Makefiles" ../
6. Click "OK" to complete

Running this Make Target will execute cmake to rebuild the Makefile

#### Project Builder Settings

1. Right click the project and select "Properties"
2. Select "C/C++ Build" and uncheck the "Use default build command"
3. Set the "Build command:" to make -C ${ProjDirPath}/build VERBOSE=1 -j

Now when you build the project, make will execute using the Makefile in the build directory

### Troubleshooting

**Getting "TIFFOpen: (null): Bad address."**

Error handling isn't what it should be in this prototype. You've forgotten a TIFF file to check

## More information

### Licence

Tifixity is released under [Apache version 2.0 license](LICENSE.txt).

### Contribute

1. [Fork the GitHub project](https://help.github.com/articles/fork-a-repo)
2. Change the code and push into the forked project
3. [Submit a pull request](https://help.github.com/articles/using-pull-requests)
