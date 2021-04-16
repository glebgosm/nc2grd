//====================================================
// Export time-variable from NetCDF to txt file
// The output file format is: 
//      timestep, year, month, day, hour, minute
//      timestep, year, month, day, hour, minute
//      timestep, year, month, day, hour, minute
//      ...
//====================================================
#include <iostream>
#include <netcdf.h>
#include <fstream>
using namespace std;

void err(int eCode);
void assert(int status);

//---------------------------------------------------------------------------
//  main recieves the input .nc-file name as the first argument and
//                the output .txt-file name as the second
//  Returns: 
//       0 - OK
//      -1 - error in command line
//      -2 - can't read the input file or the file has unsupported structure
//      -3 - other error
//---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "\nError: NetCDF file name must be provided.\n";
        err(-1);
    }
    float *timeteps, *years, *months, *days, *hours, *minutes;
    int ncid, time_id, status;
    size_t time_size;

// Open the in-file in read-only mode, get its id
    assert( nc_open(argv[1], NC_NOWRITE, &ncid) ); 

// Read timesteps
#define MAX_TIMESTEPS 100000
    timeteps= new float[MAX_TIMESTEPS]; for (int i=0; i<MAX_TIMESTEPS; i++) timeteps[i] = 0;
    years   = new float[MAX_TIMESTEPS]; for (int i=0; i<MAX_TIMESTEPS; i++) years   [i] = 0;
    months  = new float[MAX_TIMESTEPS]; for (int i=0; i<MAX_TIMESTEPS; i++) months  [i] = 0;
    days    = new float[MAX_TIMESTEPS]; for (int i=0; i<MAX_TIMESTEPS; i++) days    [i] = 0;
    hours   = new float[MAX_TIMESTEPS]; for (int i=0; i<MAX_TIMESTEPS; i++) hours   [i] = 0;
    minutes = new float[MAX_TIMESTEPS]; for (int i=0; i<MAX_TIMESTEPS; i++) minutes [i] = 0;

    // get time-var id (time_id) by name, test several names
                            status = nc_inq_varid(ncid, "time",      &time_id);
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "timestep",  &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "time step", &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "timesteps", &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "time steps",&time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "t",         &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "l",         &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "time_l",    &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "year",      &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "month",     &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "day",       &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "hour",      &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "minute",    &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "second",    &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "years",     &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "months",    &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "days",      &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "hours",     &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "minutes",   &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "seconds",   &time_id); 
    if (status != NC_NOERR) {
        cout << "\nError: Can't find time variable in the NetCDF file.\n";
        err(-2);
    }
    assert( nc_inq_dim(ncid, time_id, NULL, &time_size) );
    assert( nc_get_var_float(ncid, time_id, timeteps) );

// Read years
    // get time-var id (time_id) by name, test several names
                            status = nc_inq_varid(ncid, "year",  &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "years", &time_id); 
    if (status == NC_NOERR) 
        assert( nc_get_var_float(ncid, time_id, years) );

// Read months
    // get time-var id (time_id) by name, test several names
                            status = nc_inq_varid(ncid, "month",  &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "months", &time_id); 
    if (status == NC_NOERR) 
        assert( nc_get_var_float(ncid, time_id, months) );

// Read days
    // get time-var id (time_id) by name, test several names
                            status = nc_inq_varid(ncid, "day",  &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "days", &time_id); 
    if (status == NC_NOERR) 
        assert( nc_get_var_float(ncid, time_id, days) );

// Read hours
    // get time-var id (time_id) by name, test several names
                            status = nc_inq_varid(ncid, "hour",  &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "hours", &time_id); 
    if (status == NC_NOERR)
        assert( nc_get_var_float(ncid, time_id, hours) );

// Read minutes
    // get time-var id (time_id) by name, test several names
                            status = nc_inq_varid(ncid, "minute", &time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "minutes",&time_id); 
    if (status != NC_NOERR) status = nc_inq_varid(ncid, "min",    &time_id); 
    if (status == NC_NOERR)
        assert( nc_get_var_float(ncid, time_id, minutes) );
    
    // close nc file
    nc_close(ncid);
    
// Write timesteps
    fstream f(argv[2], ios::out);
    for (int i = 0; i < time_size; i++) 
        f << timeteps[i] << ", " 
          << years   [i] << ", " 
          << months  [i] << ", " 
          << days    [i] << ", " 
          << hours   [i] << ", "  
          << minutes [i] <<"\n";
    f.close();
    cout << "done!";
    //getchar();
}

// exit on error
void err(int eCode) {
    //cout << "\nError: "<< msg << endl;
    getchar();
    exit(eCode);
}

// Assert that NetCDF operations go well
void assert(int status) {
    if (status == NC_NOERR) return;
    cout << "\nNetCDF error (" << status << "): ";
    switch (status) {
    case NC_ENFILE       : cout << "Too many files open"                         << endl; err(-2);
    case NC_ENOMEM       : cout << "Out of memory."                              << endl; err(-2);
    case NC_EHDFERR      : cout << "HDF5 error. (NetCDF-4 files only.)"          << endl; err(-2);
    case NC_EDIMMETA     : cout << "Error in netCDF-4 dimension metadata. "      << endl; err(-2);
    case NC_EBADID       : cout << "Bad ncid. File not found."                   << endl; err(-2);
    case NC_ENOTVAR      : cout << "Invalid variable name or Variable not found."<< endl; err(-2);
    case NC_EBADDIM      : cout << "Invalid dimension ID or name."               << endl; err(-2);
    case NC_EINVALCOORDS : cout << "Index exceeds dimension bound."              << endl; err(-2);
    case NC_EEDGE        : cout << "Start+count exceeds dimension bound."        << endl; err(-2);
    case NC_ERANGE       : cout << "One or more of the values are out of range." << endl; err(-2);
    default              : cout << "Unknown NetCDF error."                       << endl; err(-2);
    }
}