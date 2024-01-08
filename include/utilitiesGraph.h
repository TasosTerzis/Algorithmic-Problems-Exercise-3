#ifndef UTILITIESGNNS_H
#define UTILITIESGNNS_H

#include <map>

#include "./graph.h"
#include "./image.h"
#include "./dataset.h"

#define T 10

std::map<double, Image *> graphNNSearch(Graph &, Image &, int, int, int);
std::map<double, Image *> bruteForce(DataSet&, Image&, int);
std::map<double, Image *> searchOnGraph(Graph &, Image &, int , int );

#endif