// Name: Radhesh Patel
// Course: ECE 231L - Programming Lab
// Project 1: Voltage and current across a capacitor as a function of time
// Due: 10/17/2026
//
// This program charges a capacitor two different ways:
//   1) a constant current source
//   2) a constant voltage source with a series resistor
// Finite differences are used to step the voltage and current forward in time.
// The results are printed to the screen every 200 timesteps.

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

// structure that holds the data for the capacitor
typedef struct _capacitor
{
  double *time;       // time array
  double *voltage;    // voltage array
  double *current;    // current array
  double C;           // capacitance value
} Capacitor;

// This function dynamically allocates the three arrays.
// I use steps + 1 spots because index 0 is the initial condition
// and index 50000 is the last timestep.
int allocateCapacitor(Capacitor *cap, int steps, double C)
{
  cap->C = C;
  cap->time = (double *)malloc((steps + 1) * sizeof(double));
  cap->voltage = (double *)malloc((steps + 1) * sizeof(double));
  cap->current = (double *)malloc((steps + 1) * sizeof(double));

  // check that malloc worked, if not return 0 so main can quit
  if (cap->time == NULL || cap->voltage == NULL || cap->current == NULL)
  {
    cout << "Memory allocation failed" << endl;
    return 0;
  }
  return 1;
}

// free the memory so there are no memory leaks
void freeCapacitor(Capacitor *cap)
{
  free(cap->time);
  free(cap->voltage);
  free(cap->current);
  cap->time = NULL;
  cap->voltage = NULL;
  cap->current = NULL;
}

// Constant current case: current stays at I0 the whole time
// V(t+1) = V(t) + I(t) * dt * (1/C)
void constantCurrent(Capacitor *cap, int steps, double dt, double I0)
{
  // initial conditions at timestep 0
  cap->time[0] = 0.0;
  cap->voltage[0] = 0.0;
  cap->current[0] = I0;

  // loop starts at 1 since 0 is already filled in
  for (int i = 1; i <= steps; i++)
  {
    cap->time[i] = i * dt;
    cap->current[i] = I0;
    cap->voltage[i] = cap->voltage[i - 1] + cap->current[i - 1] * dt * (1.0 / cap->C);
  }
}

// Constant voltage case: resistor R in series with the capacitor
// I(t+1) = I(t) - I(t)/(RC) * dt
// the voltage across the capacitor uses the same equation as above
void constantVoltage(Capacitor *cap, int steps, double dt, double V0, double R)
{
  // initial conditions at timestep 0, I(0) = V0 / R
  cap->time[0] = 0.0;
  cap->voltage[0] = 0.0;
  cap->current[0] = V0 / R;

  for (int i = 1; i <= steps; i++)
  {
    cap->time[i] = i * dt;
    cap->current[i] = cap->current[i - 1] - (cap->current[i - 1] / (R * cap->C)) * dt;
    cap->voltage[i] = cap->voltage[i - 1] + cap->current[i - 1] * dt * (1.0 / cap->C);
  }
}

// print the table, only every "skip" timesteps (200 for this project)
void printResults(Capacitor *cap, int steps, int skip)
{
  printf("%8s %15s %15s %15s\n", "Step", "Time (s)", "Voltage (V)", "Current (A)");
  for (int i = 0; i <= steps; i += skip)
  {
    printf("%8d %15.6e %15.6e %15.6e\n", i, cap->time[i], cap->voltage[i], cap->current[i]);
  }
}

int main()
{
  // values given in the project description
  double dt = 1e-10;           // time step in seconds
  double finalTime = 5e-6;     // final time in seconds
  int steps = (int)(finalTime / dt + 0.5);   // 5e-6 / 1e-10 = 50000 timesteps
  double R = 1000.0;           // 1 kOhm
  double C = 100e-12;          // 100 pF
  double I0 = 1e-2;            // 10 mA, constant current supply
  double V0 = 10.0;            // 10 V, constant voltage supply
  int skip = 200;              // print every 200 timesteps

  Capacitor cap;

  // ---------- constant current source ----------
  if (!allocateCapacitor(&cap, steps, C))
  {
    return 1;
  }
  cout << "Constant Current Source" << endl;
  constantCurrent(&cap, steps, dt, I0);
  printResults(&cap, steps, skip);
  freeCapacitor(&cap);

  // ---------- constant voltage source ----------
  if (!allocateCapacitor(&cap, steps, C))
  {
    return 1;
  }
  cout << endl << "Constant Voltage Source" << endl;
  constantVoltage(&cap, steps, dt, V0, R);
  printResults(&cap, steps, skip);
  freeCapacitor(&cap);

  return 0;
}
