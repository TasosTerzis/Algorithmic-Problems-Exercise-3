#ifndef FUNCG_H
#define FUNCG_H

#include "./common.h"
#include "./image.h"
#include "./helpers.h"
#include "./funcH.h"

// class to store the g functions that are used in LSH.
class Gfunc {

    public:
        Gfunc(int, int, int, int);
        ~Gfunc();
        u_int32_t Idi (Image &);
        u_int32_t Gi(u_int32_t);
    
    private:
        std::vector <HiFunc*> hi_funcs;     // vector of k h_functions
        std::vector <u_int32_t> r_values;   // stores the r values for each h_function
        int k, table_size;                  // k is the number of h_functions, table_size is the size of the hash table

};

#endif 