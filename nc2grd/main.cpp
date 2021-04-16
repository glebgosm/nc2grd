//////////////////////////////////////////////////////////////////////////////////////////////
// Convert a field from NetCDF into a Surfer 15 ".grd"-file
//
//   Command line example:
//   nc2grd  D:\f.nc  t_cp  k 1 imin 50 imax 150 jmin 100 jmax 200  t 123456  D:\ATLAS\temp.grd
//   where 
//      * D:\f.nc - input nc file
//      * t_cp    - name of the field to convert
//      * i, j, k - only one of these may be present: section position of a 3D-array
//      * imin, imax, jmin, jmax, kmin, kmax - cut a fragment (imin:imax,jmin:jmax,kmin:kmax) from 3D-array if present
//      * t - int user_time if necessary
//      * D:\ATLAS\temp.grd - output grd file
//
//  Returns: 
//       0 - OK
//      -1 - error in command line
//      -2 - can't read the input file or the file has unsupported structure
//      -3 - other error
//////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <netcdf.h>
#include "nc2grd.h"
#include "utils.h"
using namespace std;

int main(int argc, char* argv[]) {
    
    for (int i=1; i<=50; i++) cout << "-"; cout << endl;
    cout << "Converting NectCDF into Surfer grd format" << endl;
    for (int i=1; i<=50; i++) cout << "-";
    cout << endl << endl;

// 1. Parse command line arguments
    parse_args(argc,argv);
    
// 2. Read the nc-file
    read_nc();
    
// 3. Save the field to grd file
    GRD grd;
    // set grd metadata
    grd.x1 = grd_xmin; grd.x2 = grd_xmax;
    grd.y1 = grd_ymin; grd.y2 = grd_ymax;
    if      (grd_x_axis == dimx) { grd.nx = imax-imin+1; }
    else if (grd_x_axis == dimy) { grd.nx = jmax-jmin+1; }
    else if (grd_x_axis == dimt) { grd.nx = tmax-tmin+1; }
    if      (grd_y_axis == dimx) { grd.ny = imax-imin+1; }
    else if (grd_y_axis == dimy) { grd.ny = jmax-jmin+1; }
    else if (grd_y_axis == dimz) { grd.ny = kmax-kmin+1; }
    // copy data
    grd.data = new float[data_size];
    if (grd_y_axis < grd_x_axis) { // copy data
        for (int i = 0; i < data_size; i++)
            grd.data[i] = field[i];
    } else { // swap dimensions and copy data
        for (int j = 0; j < grd.ny; j++)
            for (int i = 0; i < grd.nx; i++)
                grd.data[i+j*grd.nx] = field[j+i*grd.ny];
    }
    // reverse depth dimension
    if (grd_y_axis == dimz)
        for (int j = 0; j < grd.ny / 2; j++)
        for (int i = 0; i < grd.nx    ; i++) {
            int n1 = i + j * grd.nx;
            int n2 = i + (grd.ny-j-1) * grd.nx;
            float temp = grd.data[n1];
                         grd.data[n1] = grd.data[n2];
                                        grd.data[n2] = temp;
        }
    // save the grd file
    grd.save_grd(fileout);

// finalize
    delete[] field; field = NULL; grd.data = NULL;
    delete[] grd.data; grd.data = NULL;
    cout << endl << "DONE!" << endl << endl;
    //system("\npause");
	return 0;
}
