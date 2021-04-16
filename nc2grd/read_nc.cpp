#include <iostream>
#include <netcdf.h>
#include "nc2grd.h"
#include "utils.h"
#include "math.h"
using namespace std;

// resultant vars:
float *field;
int data_size;
int grd_x_axis, grd_y_axis;
double grd_xmin, grd_xmax, grd_ymin, grd_ymax;
int dimx = -1, dimy = -1, dimz = -1, dimt = -1; // array indices
float fillValue;

// Assert NetCDF operations go well
void assert(int status);

// Read the nc-file
int read_nc() {

// 1. Open file & Read common info about the variable
    int ncid, var_id, ndims, dimids[NC_MAX_VAR_DIMS], natts;
    // open the in-file in read-only mode, get its id
    assert( nc_open(filein, NC_NOWRITE, &ncid) );    
    // get id of the field if present in the file
    assert( nc_inq_varid(ncid, field_name, &var_id) );    
    // get info about the field: type id (e.g. 5=float), number of dimensions
    // and attributes, dimension ids
    nc_type type_id;
    assert( nc_inq_var(ncid, var_id, 0, &type_id, &ndims, dimids, &natts) );  
    cout << "NetCDF file: var type id is " << type_id << ", the var has " << ndims << " dimensions" <<endl;
    // inquire fill value
    int no_fill;
    assert( nc_inq_var_fill(ncid, var_id, &no_fill, &fillValue) );
    cout << "NetCDF Fill Value = " << fillValue <<endl<<endl;
    // check ndims
    if (ndims > 4 || ndims < 2) {
        cout << "\nError: Cannot handle arrays of " << ndims << " dimensions.\n";
        err(-2);
    } 
    
// 2. Inquire about dimensions
#define MAX_NDIM 10
#define MAX_DIM_NAME 100
    char dim_name[MAX_NDIM][MAX_DIM_NAME];
    size_t dim_size[MAX_NDIM];
    cout << "Dimensions of nc file are:" <<endl;
    cout << "\tID\tNAME\tSIZE\n";
    for (int i = 0; i < ndims; i++) {
        assert( nc_inq_dim(ncid, dimids[i], dim_name[i], dim_size + i) );
        cout <<"\t"<< dimids[i] << "\t" << dim_name[i] << "\t"<< dim_size[i] << endl;
    }
    cout << endl;
    //   understand which dimension is x, y, z and t
    for (int n = 0; n < ndims; n++) {
        if      (isXDim(dim_name[n])) dimx = n;
        else if (isYDim(dim_name[n])) dimy = n;
        else if (isZDim(dim_name[n])) dimz = n;
        else if (isTDim(dim_name[n])) dimt = n;
    }
    cout << "X dim id = " << dimx << ", Y dim id = " << dimy << ", Z dim id = " << dimz << ", T dim id = " << dimt << endl;
    if (dimx < 0) cout << "\nCan't find x dimension of the variable (see manual for acceptable names).\n";
    if (dimy < 0) cout << "\nCan't find y dimension of the variable (see manual for acceptable names).\n";
    if (dimz < 0) cout << "\nCan't find z dimension of the variable (see manual for acceptable names).\n";
    if (dimt < 0) cout << "\nCan't find t dimension of the variable (see manual for acceptable names).\n";

// 3. Form Hyperslab vectors in order to properly read the data
//    (https://www.unidata.ucar.edu/software/netcdf/docs/programming_notes.html#specify_hyperslab)
    //  3.1. Count fixated dimensions: if less than 2 dims are free => error
    //                                 if more than 2 dims are free => fixate time and/or k
    int n_fixed_dims = 0;
    if (tconst > 0) n_fixed_dims++;
    if (iconst > 0) n_fixed_dims++;
    if (jconst > 0) n_fixed_dims++;
    if (kconst > 0) n_fixed_dims++;
    if (ndims - n_fixed_dims < 2) {
        cout << "\nError: Too many dimensions are fixated: at most " << ndims-2 << 
                " dimensions (i, j, k or t) can be set to const.\n";
        err(-1);
    } else if (ndims - n_fixed_dims == 3) {
        if (tconst < 0 && dimt>=0) tconst = 1;
        else                       kconst = 1;
    } else if (ndims - n_fixed_dims == 4) {
        tconst = 1;
        kconst = 1;
    }
    // 3.2. Set ijkt range of the part of the array to be read
    if (tconst > 0 && tconst > dim_size[dimt]) {
        cout << "\nError: the requested time index exceeds time limits.\n";
        exit(-1);
    }
    if (tconst > 0) { tmin = tconst; tmax = tconst; }
    if (iconst > 0) { imin = iconst; imax = iconst; }
    if (jconst > 0) { jmin = jconst; jmax = jconst; }
    if (kconst > 0) { kmin = kconst; kmax = kconst; }
    if (tmin <= 0) tmin = 1; if (dimt >= 0) tmax = min(dim_size[dimt], tmax); else tmax = LONG_MIN;
    if (imin <= 0) imin = 1; if (dimx >= 0) imax = min(dim_size[dimx], imax); else imax = LONG_MIN;
    if (jmin <= 0) jmin = 1; if (dimy >= 0) jmax = min(dim_size[dimy], jmax); else jmax = LONG_MIN;
    if (kmin <= 0) kmin = 1; if (dimz >= 0) kmax = min(dim_size[dimz], kmax); else kmax = LONG_MIN;
    // 3.3. form hyperslab vectors                   
    size_t *start = new size_t[ndims];
    size_t *count = new size_t[ndims];
    cout << "\nHyperslab: \n";
    cout << "\tDIM\tSTART\tCOUNT\n";
    for (int n = 0; n < ndims; n++)
        if      (n == dimx) {start[n]= imin-1; count[n]= 1+imax-imin; cout<<"\t"<< n <<"\t"<< start[n] <<"\t"<<count[n] <<endl;}
        else if (n == dimy) {start[n]= jmin-1; count[n]= 1+jmax-jmin; cout<<"\t"<< n <<"\t"<< start[n] <<"\t"<<count[n] <<endl;} 
        else if (n == dimz) {start[n]= kmin-1; count[n]= 1+kmax-kmin; cout<<"\t"<< n <<"\t"<< start[n] <<"\t"<<count[n] <<endl;} 
        else if (n == dimt) {
            int ff = start[n]= tmin-1; 
            count[n]= 1+tmax-tmin; 
            cout<<"\t"<< n <<"\t"<< start[n] <<"\t"<<count[n] <<endl;}
        
// 4. Read the field
    // 4.1. Compute data size
    data_size = 1;
    for (int n = 0; n < ndims; n++)
        data_size *= count[n];
    // 4.2. Read the data
    field = new float[data_size];
    assert( nc_get_vara_float(ncid, var_id, start, count, field) );
    //cout << field[0];
    delete[] start; start = NULL;
    delete[] count; count = NULL;  

// 5. Determine which nc-files dimensions correspond to x and y axes of the grd-file
    // x-axis
    if      (tconst < 0 && dimt >= 0) grd_x_axis = dimt;  // Time (if needed) is always along x-axis.
    else if (iconst < 0 && dimx >= 0) grd_x_axis = dimx;  // If no time => x-axis is i or j.
    else                              grd_x_axis = dimy;
    // y-axis
    if      (kconst < 0 && dimz >= 0) grd_y_axis = dimz;  // Depth (if needed) is always along y-axis.
    else if (jconst < 0 && dimy >= 0) grd_y_axis = dimy;  // If no depth => y-axis is j or k.
    else                              grd_y_axis = dimx;  // if grd_y_axis == dimx => tx-diagram
    // test
    if (grd_x_axis == grd_y_axis || grd_x_axis < 0 || grd_y_axis < 0) {
        cout << "\nError: failed to determine which nc-files dimensions correspond to x and y axes of the grd-file: "
             << grd_x_axis << " " << grd_y_axis << "\n";
        err(-3);
    }
    
// 6. Define min and max of the grd-file axes 
    if (logCrd) { // use logical coordinates
        // x axis
        if      (grd_x_axis == dimx) { grd_xmin = imin; grd_xmax = imax; } 
        else if (grd_x_axis == dimy) { grd_xmin = jmin; grd_xmax = jmax; } 
        else if (grd_x_axis == dimt) { grd_xmin = tmin; grd_xmax = tmax; }
        // y axis
        if      (grd_y_axis == dimx) { grd_ymin = imin; grd_ymax = imax; } 
        else if (grd_y_axis == dimy) { grd_ymin = jmin; grd_ymax = jmax; } 
        else if (grd_y_axis == dimz) { grd_ymin =-kmax; grd_ymax =-kmin; }
    } else { // use real (geographical) coordinates
        // read coordinate min and max from nc-file
        int x1_id, x2_id;
        float *x1 = new float[dim_size[grd_x_axis]];
        float *x2 = new float[dim_size[grd_y_axis]];
        assert( nc_inq_varid(ncid, dim_name[grd_x_axis], &x1_id) );
        assert( nc_inq_varid(ncid, dim_name[grd_y_axis], &x2_id) );
        assert( nc_get_var_float(ncid, x1_id, x1) );
        assert( nc_get_var_float(ncid, x2_id, x2) );
        int xmin = -1, ymin = -1, xmax = -1, ymax = -1;
        // x axis
        if      (grd_x_axis == dimx) { xmin = imin; xmax = imax; }
        else if (grd_x_axis == dimy) { xmin = jmin; xmax = jmax; }
        else if (grd_x_axis == dimt) { xmin = tmin; xmax = tmax; }
        // y axis
        if      (grd_y_axis == dimx) { ymin = imin; ymax = imax; }
        else if (grd_y_axis == dimy) { ymin = jmin; ymax = jmax; }
        else if (grd_y_axis == dimz) { ymin = kmax; ymax = kmin; }
        grd_xmin = x1[xmin-1]; grd_xmax = x1[xmax-1];
        grd_ymin = x2[ymin-1]; grd_ymax = x2[ymax-1];
        if (grd_y_axis == dimz) {
            grd_ymin = -grd_ymin;
            grd_ymax = -grd_ymax;
        }
        delete [] x1; x1 = NULL;
        delete [] x2; x2 = NULL;
    }
    
// 7. close the file and clean up
    nc_close(ncid);
    return 0;
}

// Assert that NetCDF operations go well
void assert(int status)
{
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