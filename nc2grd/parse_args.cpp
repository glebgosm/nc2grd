#include <iostream>
#include "stdlib.h"
#include "nc2grd.h"
#include "utils.h"
using namespace std;

// input parameters
int iconst, jconst, kconst, tconst;
int imin, imax, jmin, jmax, kmin, kmax, tmin, tmax;
int logCrd;
char *filein, *fileout, *field_name;

// Parse command line arguments
int parse_args(int argc, char* argv[]) {
    // files & field names
    if (argc < 4) {
		cout << "\nError: At least input, output filenames and field name must be provided";
		err(-1);
	} else {
		filein  = argv[1];
        field_name = argv[2];
        fileout = argv[argc-1];
	}

	// section & time parameters
    tconst= LONG_MIN; tmin = LONG_MIN; tmax = LONG_MAX;
	iconst= LONG_MIN; imin = LONG_MIN; imax = LONG_MAX;
    jconst= LONG_MIN; jmin = LONG_MIN; jmax = LONG_MAX;
    kconst= LONG_MIN; kmin = LONG_MIN; kmax = LONG_MAX;
    logCrd = 0; // geo-coords by default
	for (int i = 3; i < argc-2; i+=2) {
        if      (!strcmp(argv[i], "i" )) iconst = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "j" )) jconst = atoi(argv[i+1]);
	    else if (!strcmp(argv[i], "k" )) kconst = atoi(argv[i+1]);
	    else if (!strcmp(argv[i], "t" )) tconst = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "i1")) imin   = atoi(argv[i+1]);
	    else if (!strcmp(argv[i], "i2")) imax   = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "j1")) jmin   = atoi(argv[i+1]);
	    else if (!strcmp(argv[i], "j2")) jmax   = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "k1")) kmin   = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "k2")) kmax   = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "t1")) tmin   = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "t2")) tmax   = atoi(argv[i+1]);
        else if (!strcmp(argv[i], "lc")) logCrd = atoi(argv[i+1]);
        else {
            cout << "\nError: Unknown command line argument - " << argv[i];
            err(-1);
        }
    }
    
    // print out the parameters
	cout << "Command line is: " << endl;
    for (int i=0; i<argc; i++)
        cout << argv[i] << " ";
    cout << endl << endl;
    cout << "filein  = " << filein << endl;
    cout << "field   = " << field_name << endl;
    cout << "iconst  = " << iconst << endl;
    cout << "jconst  = " << jconst << endl;
    cout << "kconst  = " << kconst << endl;
    cout << "tconst  = " << tconst << endl;
    cout << "imin    = " << imin << endl;
    cout << "imax    = " << imax << endl;
    cout << "jmin    = " << jmin << endl;
    cout << "jmax    = " << jmax << endl;
    cout << "kmin    = " << kmin << endl;
    cout << "kmax    = " << kmax << endl;
    cout << "tmin    = " << imin << endl;
    cout << "tmax    = " << imax << endl;
    cout << "logCrd  = " << logCrd  << endl;
    cout << "fileout = " << fileout << endl;

    cout << endl;
    return 0;
}
