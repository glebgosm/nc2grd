#pragma once

// min of two
int min(int a, int b);

// string to lower case convertion
char* lowerCase(char* s);

// determine whether a string is a name of abscissa
bool isXDim(char* s);

// determine whether a string is a name of ordinate
bool isYDim(char* s);

// determine whether a string is a name of depth
bool isZDim(char* s);

// determine whether a string is a name of time
bool isTDim(char* s);

// Assert NetCDF operations go well
void assert(int status);

// exit on error
void err(int eCode);
