Comments on time dimension of the input file

It is assumed that time is measured as an integer: either number of timestep or number of seconds, past from 01-01-1900T00:00.
But this integer can be stored as a float or a double.

It is assumed that the nc-file contains a variable with either of 3 names: time, timestep or timesteps.
This variable is read and the timestep provided in the command line (argument t) is looked for in the time-variable.