PHAS0100Assignment2
------------------

[![Build Status](https://travis-ci.com/[USERNAME]/PHAS0100Assignment2.svg?branch=master)](https://travis-ci.com/[USERNAME]/PHAS0100Assignment2)
[![Build Status](https://ci.appveyor.com/api/projects/status/[APPVEYOR_ID]/branch/master)](https://ci.appveyor.com/project/[USERNAME]/PHAS0100Assignment2)


Purpose
-------

This project serves as a starting point for the PHAS0100 Assignment 2 Gravitational N-body Simulation coursework. It has a reasonable folder structure for [CMake](https://cmake.org/) based projects,
that use [CTest](https://cmake.org/) to run unit tests via [Catch](https://github.com/catchorg/Catch2). 

Further information on the specific project is left as an exercise for the student.


Credits
-------

This project is maintained by [Dr. Jim Dobson](https://www.ucl.ac.uk/physics-astronomy/people/dr-jim-dobson). It is based on [CMakeCatch2](https://github.com/UCL/CMakeCatch2.git) that was originally developed as a teaching aid for UCL's ["Research Computing with C++"](http://rits.github-pages.ucl.ac.uk/research-computing-with-cpp/)
course developed by [Dr. James Hetherington](http://www.ucl.ac.uk/research-it-services/people/james)
and [Dr. Matt Clarkson](https://iris.ucl.ac.uk/iris/browse/profile?upi=MJCLA42).


Build Instructions
------------------

To build this project.

First, turn on the IDE(like VS code) by Dev Container and open the project as the folder.

Second, open new terminal and input the comand below on terminal to build the document.

```Bash
mkdir build
cd build
cmake ..
make
```
<!-- if you want to toggle on/off all std::cout for the benchmarking, put this link to cmake build type:
```Bash
cmake ..-DCMAKE_CXX_FLAGS_DEBUG=Debug
``` -->
After building document, users could run the project following the run instruction below.

Use instruction
---
### **Command-line app**

There are two command line app in this project which are solarSystemSimulator and randomSystemSimulator.

### *Solar System Simulator*

This is an app to simulate solar system in real world to evolve system and calulate the system energy by specific time length and step-size.

The app will output the positions of plants in solar-system and the Kinetic energy, potential energy, total energy of plant system at the beginning and end of simulation, and the time of code running.


User could input comands below from the build directory to check the help information:

```Bash
./bin/solarSystemSimulator -h
```
or
```Bash
./bin/solarSystemSimulator --help
```

And user could input the step-size (the time of each step) and the time(total time of simulation) to simulate the plants system. This is the instruction:

```Bash
./bin/solarSystemSimulator -s <step-size, unit:year> -t <time, unit:year>
```

This is an example for  timestep of 0.000274 years (this is~0.1 days) and then run the program for 1 year of simulated time:

```Bash
./bin/solarSystemSimulator -s 0.000274 -t 1
```

### *Random system simulator*

This is an app to simulate a random 2000-planet system to evolve system and calulate the system energy by specific time length and step-size.

OpenMP is used to parallelise the simulation in order to improve its performance in this app.

User could input comands below from the build directory to check the help information:

```Bash
./bin/randomSystemSimulator -h
```
or
```Bash
./bin/randomSystemSimulator --help
```

And user could input the step-size (the time of each step) and the time(total time of simulation) and the number of thread to simulate the plants system. This is the instruction:

```Bash
./bin/randomSystemSimulator -s <step-size, unit:year> -t <time, unit:year> -n <number of thread>
```

This is an example for  timestep of 0.00137 years (this is~0.5 days) and then run the program for 1 year of simulated time and set 8threads:

```Bash
./bin/randomSystemSimulator -s 0.00137 -t 1 -n 8
```

### **Unit Tests**
Users can input the command below to run the file "unit_test.cpp" to test.
```Bash
ctest -V
```

### **Test the solarSystemSimulator**

I use a timestep of 0.000274 years (~0.1 days) and 1 year of simulated time as an example to test solarSystemSimulator app.

Summarising the position of the solar system
bodies at the start and end of the simulation:

```Bash
Sun
 original position:
-0.00670894
  0.0060565
 0.00010651
 current position:
-0.00830539
 0.00294565
0.000168637

Mercury
 original position:
  0.230275
 -0.351957
-0.0508834
 current position:
-0.146209
 -1.04648
-0.074043

Venus
 original position:
-0.453178
-0.563815
0.0180571
 current position:
   0.169022
   0.751123
0.000189264

Earth
 original position:
  -0.185741
   0.972886
4.83111e-05
 current position:
  -0.099235
     1.0015
0.000108855

Mars
 original position:
 0.614263
  1.38167
0.0136846
 current position:
 -2.52688
  1.26325
0.0882352

Jupiter
 original position:
   3.0346
 -4.08225
-0.050909
 current position:
   4.32888
  -1.39283
-0.0910674

Saturn
 original position:
    5.4834
  -8.33624
-0.0732546
 current position:
  6.83329
  -6.8811
-0.152439

Uranus
 original position:
  15.3436
  12.4673
-0.152634
 current position:
  14.2433
  13.3617
-0.135071

Neptune
 original position:
  29.4537
 -5.22631
-0.571104
 current position:
  29.5955
 -4.08302
-0.597839
```


Benchmark
---

### **The energy of solar system**

In this part, I run for 100 year’s worth of simulated time and for 10 different timestep sizes(from 1e-7 to 5e-3 year) to summary the results of solar-system energy and benchmark the runtime.


```bash
./bin/solarSystemSimulator -s 1e-7 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.154232
The Potential energy:-0.323735
The Total energy:-0.169503
CPU time used: 2116807.46 ms
Wall clock time passed: 2117319.95 ms


./bin/solarSystemSimulator -s 5e-7 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.154218
The Potential energy:-0.323682
The Total energy:-0.169463
CPU time used: 507366.03 ms
Wall clock time passed: 507377.13 ms


./bin/solarSystemSimulator -s 1e-6 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.154858
The Potential energy:-0.324274
The Total energy:-0.169415
CPU time used: 202197.73 ms
Wall clock time passed: 202197.81 ms


./bin/solarSystemSimulator -s 5e-6 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.154063
The Potential energy:-0.323156
The Total energy:-0.169093
CPU time used: 40619.36 ms
Wall clock time passed: 40622.48 ms


./bin/solarSystemSimulator -s 1e-5 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.152736
The Potential energy:-0.321518
The Total energy:-0.168782
CPU time used: 19541.54 ms
Wall clock time passed: 19542.45 ms


./bin/solarSystemSimulator -s 5e-5 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.148145
The Potential energy:-0.315565
The Total energy:-0.16742
CPU time used: 3978.08 ms
Wall clock time passed: 3978.67 ms


./bin/solarSystemSimulator -s 1e-4 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.143218
The Potential energy:-0.309691
The Total energy:-0.166472
CPU time used: 2133.48 ms
Wall clock time passed: 2133.47 ms


./bin/solarSystemSimulator -s 5e-4 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.122924
The Potential energy:-0.285179
The Total energy:-0.162255
CPU time used: 402.75 ms
Wall clock time passed: 402.77 ms


./bin/solarSystemSimulator -s 1e-3 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.124411
The Potential energy:-0.282653
The Total energy:-0.158243
CPU time used: 208.93 ms
Wall clock time passed: 208.95 ms


./bin/solarSystemSimulator -s 5e-3 -t 100

The energy at the beginning of simulation: 
The Kinetic energy:0.187358
The Potential energy:-0.356872
The Total energy:-0.169514
The total energy at the end of simulation: 
The Kinetic energy:0.102706
The Potential energy:-0.240089
The Total energy:-0.137383
CPU time used: 44.48 ms
Wall clock time passed: 44.52 ms
```

By the result, we could see the original total energy is $-0.169514$ before the simulation. After 100 years time simulation, the total energy by shortest timestep size $1*10^{-7}$ is $-0.169503$ which is the closest to original value. On the other hand, the total energy by longest timestep size $5*10^{-3}$ is $-0.137383$ which has largest difference as original value. In addition, we can observe that the energy is far from the original energy with the decrease of timestep size. Therefore, we could deduce that the accuracy increases as the timestep size decrease.

However, although the result is the most accurate of choosing timestep size $1*10^{-7}$, the runtime is longest which is $2117319.95$ ms(~35minutes 17seconds), and it takes so much time. The result of choosing step size $5*10^{-7}$ is the second most accurate with $-0.169463$ which three significant digits are accaury, and the runtime is $507377.13$ ms (~~8minutes 27seconds) which is reasonable.

Hence, I choose the timestep size $5*10^{-7}$  as the good value of timestep to balance between simulation run time and accuracy. 

### **Parallelise 2000-planet-system simulation by OpenMP**

I choose timestep size 0.00137 year(~0.5days) and total time 1 year to 2000-planet-system.


**Benchmark using OpenMP to Parallelise simulation:**

|**Number of thread**|**CPU time**|**Wall clock time**|
| :----: | :----: | :----: |
|Without OpenMP| 108276 ms |108295 ms|
|2| 121665 ms |61986 ms|
|3|136681 ms |48832 ms|
|4|175326 ms |45654 ms|
|5|148711 ms |45282 ms|
|6|158723 ms |48433 ms|
|7|171935 ms|49765 ms|
|8|170123 ms |49389 ms|


Wall clock time measures how much time has passed while CPU time is how long the CPU is busy. With OpenMP, Wall clock time is smaller than CPU time which shows that the simulation has been executed in parallel. 

In conclusion, without OpenMp, CPU time is almost the same as Wall clock time. CPU time is almost the product of number of threads and Wall clock time when number of threads is less and equal to 4. In addition, before the number of threads reaching the number of cores on the machine(4 in my machine), Wall clock time decrease as increasing number of threads. 

Therefore, using OpenMP to parallelise the relevant parts can improve the performance of the simulation. However, the CPU time grow up as the number of threads increase.

However, after the number of threads reaching the number of cores on the machine, the increasing number of threads lead to increasing Wall clock time and decreasing CPU time. Therefore, when number of threads exceed the the number of cores, more threads can not help improve the performance and could even produce a negative effect.




