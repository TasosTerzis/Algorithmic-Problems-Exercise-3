#ifndef FUNCH_H
#define FUNCH_H

#include "./common.h"
#include "./image.h"
#include "./helpers.h"

// class to store the h functions that are used in LSH.
class HiFunc {
    
    public:
        HiFunc(int, int);
        ~HiFunc();
        u_int32_t hi(Image &);
    
    private:
        double t;               // t is a random number in [0, w)
        std::vector<double> v;  // v is a vector of random numbers in [0, 1)
        int w;                  // w is the window size
};

#endif 