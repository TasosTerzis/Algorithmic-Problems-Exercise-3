#ifndef HELPERS_H
#define HELPERS_H

#include "./common.h"
#include "./image.h"

// Functions that are generally used in the program, considered as helpers

// Function to find the distance between two images. Receives the two images, their dimension and the metric to be used (2 for euclidean, 1 for manhattan etc..)
double nnDistance(Image &, Image &, int, int);

// Function to swap the byte order of a 32-bit integer
int swapEndian(int);

// Function to calculate the modulo of two numbers
u_int32_t modulo(u_int32_t, u_int32_t);


// Function to calculate all binary numbers with a given Hamming distance
std::vector<int32_t> hammingNumbers(int32_t, int, int); 

// struct to store the configuration info used in clustering
struct ConfInfo {
    int k_means;
    int L;
    int M;
    int probes;
    int k_lsh;
    int k_cube;
};
typedef struct ConfInfo ConfInfo;

// Function to get the configuration info from the config file
void getConfInfo (ConfInfo &, std::string);

#endif 