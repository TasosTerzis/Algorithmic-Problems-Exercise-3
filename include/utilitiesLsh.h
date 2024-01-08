#ifndef UTILITIESLSH_H
#define UTILITIESLSH_H

#include "./common.h"
#include "./dataset.h"
#include "./image.h"
#include "./helpers.h"
#include "./hashLSH.h"

// define the window size for the h functions
#define W_LSH 800

// function to calculate the smallest distance between a query image and a set of images (brute force)
std::map <double, Image*> bruteForceLsh(DataSet& , Image&, int);

// function to find the k nearests neighbors of a query
std::map <double, Image*> kNearestNeighbour(Hash &, Image &, int);

// function to find neihbours inside a radius (range search)
std::vector <Image*> rangeSearch (Hash &, Image & , int);

#endif 
