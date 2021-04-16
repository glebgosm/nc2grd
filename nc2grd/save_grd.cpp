#include <iostream>
#include <fstream>
#include "nc2grd.h"
#include "utils.h"
using namespace std;

const float grd_blank = 1.70141e38;

// save a field in a Surfer grd-file
int GRD::save_grd(char* path) {
    // put grd_blank values instead of fillValue
    // and find min and max of the data
    z1 =  grd_blank;
    z2 = -grd_blank;
    for (int i=1; i<=nx; i++)
    for (int j=1; j<=ny; j++) {
        int k = (i-1)*ny + j-1;
        if (data[k] == fillValue) 
            data[k] = grd_blank;
        else if (data[k] > z2) 
            z2 = data[k];
        else if (data[k] < z1) 
            z1 = data[k];
    }
    //write the file
    fstream fgrd(path, ios::binary | ios::out);
    if (!fgrd) {
        cout << "\nError: Cannot write file "<< path << endl;
        err(-3);
    }
    fgrd.write("DSBB",4); 
    fgrd.write((char*)&nx,2); 
    fgrd.write((char*)&ny,2);
    fgrd.write((char*)&x1,8);
    fgrd.write((char*)&x2,8);
    fgrd.write((char*)&y1,8);
    fgrd.write((char*)&y2,8);
    fgrd.write((char*)&z1,8);
    fgrd.write((char*)&z2,8);
    fgrd.write((char*)data, sizeof(float)*nx*ny);
    fgrd.close();
    return 0;
}      

