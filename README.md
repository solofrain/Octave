# Channel Access for Octave


D. P. Siddons (siddons@bnl.gov)

Brookhaven National Laboratory

Upton, NY 11973

Eric Norum (norume@aps.anl.gov) & Janet Anderson (jba@aps.anl.gov)

Advanced Photon Source

Argonne National Laboratory

9700 South Cass Avenue

Argonne, IL 60439
 
*************************

This package provides simple Channel Access facilities to Octave, an open source Matlab (R) clone. It is based upon the ezca library, and simply calls the C library from a simple C++ wrapper. It does little in the way of error checking yet, and is basically a quick job to get a minimal capability, but someone may find it useful. 

The package is designed as an EPICS extension, and so should be placed in the `extensions/src` directory. "Octave" should be added to the list of subdirs to be made in `extensions/config/CONFIG_EXTENSIONS`.  Before attemping to build this extension you must first install Octave and have the Octave bin directory on your executable search path.

One c++ file (`castuff.cc`) contains wrappers for the following ezca functions:

- caput
- caget
- casetmonitor
- caclrmonitor
- canewmonval
- cagetnelem
- cagetTimeout
- casetTImeout
- cagetRetryCount
- casetRetryCount

Since Octave needs to know where to look for each dynamically loaded command, symbolic links are added to force it to look in the castuff.oct generated file. When built, the .oct files and links are copied to the `extensions/lib/<target>` directory. In order for Octave to find these, the default Octave `LOADPATH` should be modified to add a path to them. This can be done by adding a command such as the the following to a `.octtaverc` file in your home directory:

```
addpath ('/usr/lib/epics/lib/linux-x86_64', '-begin');
```

Then simply invoking any of the above commands from the Octave shell will load it automatically. On some architectures it will be necessary to have the standard EPICS library directores included in the `LD_LIBRARY_PATH` environment variable.

The file 'looping.m' is a simple Octave script to read data from a PV, and illustrates the use of the cagetTimeout, cagetRetryCount, caget, casetmonitor, caclrmonitor and canewmonval commands. The PV referenced by this script is an Octave string variable. It should be set to an existing valid PV name before invoking the script. It was written to reference the compressExample PV which is part of the standard test IOC. As such it uses a 10 x 10 array to hold caget results. The following is a screen dump of a session:

```
octave:1> PVname="peterHost:compressExample"
PVname = peterHost:compressExample
octave:2> looping
geting timeout..
ans = 0.050000
geting retry count..
ans = 599
waiting..
ans =

  5  6  7  8  9  0  1  2  3  4

waiting..
ans =

  6  7  8  9  0  1  2  3  4  5

waiting..
ans =

  7  8  9  0  1  2  3  4  5  6

waiting..
ans =

  8  9  0  1  2  3  4  5  6  7

waiting..
ans =

  9  0  1  2  3  4  5  6  7  8

waiting..
ans =

  0  1  2  3  4  5  6  7  8  9

waiting..
ans =

  1  2  3  4  5  6  7  8  9  0

waiting..
ans =

  2  3  4  5  6  7  8  9  0  1

waiting..
ans =

  3  4  5  6  7  8  9  0  1  2

waiting..
ans =

  4  5  6  7  8  9  0  1  2  3

a =

  5  6  7  8  9  0  1  2  3  4
  6  7  8  9  0  1  2  3  4  5
  7  8  9  0  1  2  3  4  5  6
  8  9  0  1  2  3  4  5  6  7
  9  0  1  2  3  4  5  6  7  8
  0  1  2  3  4  5  6  7  8  9
  1  2  3  4  5  6  7  8  9  0
  2  3  4  5  6  7  8  9  0  1
  3  4  5  6  7  8  9  0  1  2
  4  5  6  7  8  9  0  1  2  3

octave:3>    
```

If a scalar PV is referenced instead of the expected vector, Octave will fill each row of the array with the scalar value received.

Adding further commands is quite straightforward, as the source code shows. A good reference for extending Octave is:

http:/octave.sourceforge.net/coda/

This site explores in some detail the writing of `.oct` files, and the author is indebted to it.  
