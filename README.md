# Project 1: Voltage and Current Across a Capacitor

**Name:** Radhesh Patel
**Course:** ECE 231L - Programming Lab, University of New Mexico
**Due Date:** 10/17/2026 at 11:59 PM
**Language:** C++

## Description
This program solves for the voltage and current across a capacitor as a function of time. It does this for two different charging circuits:

1. A constant current source connected directly to the capacitor
2. A constant voltage source connected to the capacitor through a series resistor

The voltage and current are found with a finite-difference method, stepping forward in time. This project also reviews pointers, structures, and dynamic memory allocation.

## File
- `project1.cpp` - the source code
- `README.md` - this file

## Equations Used

### Constant current source
```
V(t+1) = V(t) + I(t) * dt * (1/C)
```
- V(0) = 0 V
- I(t) = I0 for all time

### Constant voltage source (with series resistor)
```
I(t+1) = I(t) - (I(t) / (R*C)) * dt
V(t+1) = V(t) + I(t) * dt * (1/C)
```
- V(0) = 0 V
- I(0) = V0 / R

## Values Used
| Variable | Value |
|----------|-------|
| dt (time step) | 1e-10 s |
| Final time | 5e-6 s |
| Number of timesteps | 50000 |
| R | 1 kOhm |
| C | 100e-12 F |
| I (constant current supply) | 1e-2 A |
| V0 (constant voltage supply) | 10.0 V |

## How the Program Works
- A `Capacitor` structure holds the time, voltage, and current arrays and the capacitance value.
- The three arrays are dynamically allocated with `malloc`. Each array has 50001 spots, so index 0 holds the initial conditions and index 50000 is the final time.
- Timestep 0 is filled with the initial conditions. The loop then starts at timestep 1 and goes to the last timestep.
- The program checks that `malloc` worked before using the arrays.
- The results are printed to the screen every 200 timesteps (251 rows for each circuit).
- The memory is freed with `free` after each circuit, so there are no memory leaks.

## How to Compile and Run
```bash
g++ -Wall -o project1 project1.cpp
./project1
```
The program compiles with no warnings.

## Results

### Constant current source
- The current stays at 1.0e-02 A the whole time.
- The voltage increases linearly, 2 V for every 200 timesteps.
- At 5e-6 s (step 50000) the voltage is 500 V.
- This matches the expected slope of I/C = 1e8 V/s.

```
    Step        Time (s)     Voltage (V)     Current (A)
       0    0.000000e+00    0.000000e+00    1.000000e-02
     200    2.000000e-08    2.000000e+00    1.000000e-02
     400    4.000000e-08    4.000000e+00    1.000000e-02
     ...
   50000    5.000000e-06    5.000000e+02    1.000000e-02
```
### Constant voltage source
- The current starts at 10 mA (V0 / R) and decays exponentially toward 0.
- The voltage rises from 0 V and levels off at 10 V.
- The time constant is RC = 100 ns. At step 1000 (t = 100 ns) the current is 3.68e-03 A, which is about 10 mA * e^-1.

```
    Step        Time (s)     Voltage (V)     Current (A)
       0    0.000000e+00    0.000000e+00    1.000000e-02
     200    2.000000e-08    1.813512e+00    8.186488e-03
     400    4.000000e-08    3.298141e+00    6.701859e-03
     ...
   50000    5.000000e-06    1.000000e+01    1.881097e-24
```
## Notes
- The number of timesteps is calculated from the final time and dt: 5e-6 / 1e-10 = 50000.
- The time at each step is found with `i * dt`.
- The full output is 507 lines when the program is run.
