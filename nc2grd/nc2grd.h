#pragma once;
 // input parameters
extern int iconst, jconst, kconst, tconst;
extern int imin, imax, jmin, jmax, kmin, kmax, tmin, tmax;
extern int logCrd;
extern char *filein, *fileout, *field_name;

// data parameters
extern int data_size;
extern int dimx, dimy, dimz, dimt; // array indices
extern int nx, ny;
extern int grd_x_axis, grd_y_axis;
extern double grd_xmin, grd_xmax, grd_ymin, grd_ymax;
extern float *field;
extern float fillValue;

// command line parser
int parse_args(int argc, char* argv[]);

// Read the nc-file
int read_nc();

// grd data structure + data saver function
struct GRD {
    short nx, ny;
    double x1, x2, y1, y2, z1, z2;
    float *data;    
    int save_grd(char* path);
};