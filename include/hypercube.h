#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "./common.h"
#include "./funcH.h"
#include "./hashtable.h"
#include "./dataset.h"
#include "./helpers.h"
#include "./image.h"

// class to represent the hypercube
class HyperCube{

    public:
        HyperCube(DataSet &, int, int, int, int, int);
        ~HyperCube();
        int getM();
        int getProbes();
        int getK();
        int getDimension();
        std::vector<Image *> closestVertices(int32_t);
        int32_t hashFunction(Image&);
        std::vector <Image*> getCandidates(int32_t, int);
        
    private:
        int dimension, k, M, probes;            // dimension of the images, number of h functions, number of vertices, number of probes
        std::vector <HiFunc*> hi_funcs;         // vector of k h_functions
        std::map <int32_t, int16_t> f_values;   // stores the f values for each h_function
        HashTable* h_cube;  // the hypercube is practically a hash table
        int16_t coinToss(); 
};

#endif