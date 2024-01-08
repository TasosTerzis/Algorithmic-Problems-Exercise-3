#ifndef UTILITIESCUBE_H
#define UTILITIESCUBE_H

#include "./common.h"
#include "./dataset.h"
#include "./image.h"
#include "./helpers.h"
#include "./hypercube.h"

// define the window size for the h functions
#define W_CUBE 800

// function to calculate the smallest distance between a query image and a set of images (brute force)
std::multimap <double, Image*> bruteForceCube(DataSet& , Image&, int);

// function to find the k nearest neighbors of a query
std::multimap <double, Image*> kNearestNeighbour(HyperCube &, Image &, int);

// function to find neihbours inside a radius (range search)
std::vector <Image*> rangeSearch (HyperCube &, Image & , int);

#endif 
