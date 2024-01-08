#include "../../include/funcG.h"

// Constructor of Gfunc. Creates k HiFunc objects and stores them in a vector
Gfunc::Gfunc(int d, int w, int k, int table_size) {

    this->k = k;
    this->table_size = table_size;
    for (int i = 0; i < k; i++) {
        HiFunc *hi_func = new HiFunc(d, w);
        this->hi_funcs.push_back(hi_func);
    }

    // initialize r_values. They are random, but remain the same for current g function
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<u_int32_t> r_distribution(1, static_cast<u_int32_t>(RAND_MAX));
    for (int i = 0; i < k; i++) {
        u_int32_t r = r_distribution(generator);
        this->r_values.push_back(r);
    }
}

// Returns the id value by analyzing image. It is used by the g func to calculate the g value and store the image in the correct bucket
u_int32_t Gfunc::Idi (Image &p) {

    // calculate h(p) values
    std::vector<u_int32_t> h_values(k);
    for(int i=0; i<k; i++) 
        h_values[i] = this->hi_funcs[i]->hi(p);
    
    u_int32_t M = static_cast<u_int32_t>(pow(2, 32) - 5)/k;
    // multiply h(p) values with r values and sum them to calculate id value
    u_int32_t id_value = 0;
    for (int i = 0; i < k; i++){
        id_value += modulo(this->r_values[i] * h_values[i], M);
    }
    // Apply modulo M to the result
    return id_value;
}

// the actual hash function. Returns the bucket id
u_int32_t Gfunc::Gi(u_int32_t id) {
    u_int32_t temp = modulo(id, this->table_size);
    return temp;
}

// Destructor of Gfunc. Deletes all the HiFunc objects
Gfunc::~Gfunc() {
    for (int i = 0; i < this->k; i++)
        delete this->hi_funcs[i];
}




