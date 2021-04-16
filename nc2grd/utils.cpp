#include <iostream>
#include <netcdf.h>
using namespace std;

// min of two
int min(int a, int b) {
    return (a<b) ? a : b;
}

// string to lower case convertion
char* lowerCase(char* s) {
    for (int i=0; s[i]!=0; i++)
        if ('A' <= s[i] && s[i] <= 'Z') 
            s[i] = s[i] - 'A' + 'a';
    return s;
}

// determine whether a string is a name of abscissa
bool isXDim(char* s) {
    s = lowerCase(s);
    return !strcmp(s,"lon") || 
           !strcmp(s,"longitude") ||
           !strcmp(s,"x") ||
           !strcmp(s,"i") ||
           !strcmp(s,"x1") ||
           !strcmp(s,"x1_t") ||
           !strcmp(s,"x1_u") ||
           !strcmp(s,"x1_v") ||
           !strcmp(s,"x1_x");
}

// determine whether a string is a name of ordinate
bool isYDim(char* s) {
    s = lowerCase(s);
    return !strcmp(s,"lat") || 
           !strcmp(s,"latitude") ||
           !strcmp(s,"y") ||
           !strcmp(s,"j") ||
           !strcmp(s,"x2") ||
           !strcmp(s,"x2_t") ||
           !strcmp(s,"x2_u") ||
           !strcmp(s,"x2_v") ||
           !strcmp(s,"x2_x");
}

// determine whether a string is a name of depth
bool isZDim(char* s) {
    s = lowerCase(s);
    return !strcmp(s,"d") || 
           !strcmp(s,"depth") || 
           !strcmp(s,"h") || 
           !strcmp(s,"height") || 
           !strcmp(s,"horizon") || 
           !strcmp(s,"z") ||
           !strcmp(s,"k") ||
           !strcmp(s,"x3") ||
           !strcmp(s,"x3_t") ||
           !strcmp(s,"x3_u") ||
           !strcmp(s,"x3_v") ||
           !strcmp(s,"x3_x");
}

// determine whether a string is a name of time
bool isTDim(char* s) {
    s = lowerCase(s);
    return !strcmp(s,"t")          || 
           !strcmp(s,"time")       || 
           !strcmp(s,"l")          || 
           !strcmp(s,"time_l")     ||
           !strcmp(s,"timestep")   ||
           !strcmp(s,"time step")  ||
           !strcmp(s,"timesteps")  ||
           !strcmp(s,"time steps") ||
           !strcmp(s,"year")       ||
           !strcmp(s,"month")      ||
           !strcmp(s,"day")        ||
           !strcmp(s,"hour")       ||
           !strcmp(s,"minute")     ||
           !strcmp(s,"second");
}

// exit on error
void err(int eCode) {
    //cout << "\nError: "<< msg << endl;
    getchar();
    exit(eCode);
}
