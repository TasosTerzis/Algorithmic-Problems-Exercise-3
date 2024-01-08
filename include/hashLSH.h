#ifndef HASHLSH_H
#define HASHLSH_H

#include "./common.h"
#include "./image.h"
#include "./dataset.h"
#include "./funcG.h"

#define TABLE_DIVISOR 16    // divisor for the table size. Decides the number of buckets, thus the distribution of the images in the buckets

// class to represent one single hash table used in lsh
class HashTableLSH {

    public:
        HashTableLSH(int, int, int, int);
        ~HashTableLSH();
        void push(Image *, u_int32_t, u_int32_t);
        std::vector<Image *> getCandidates(u_int32_t, u_int32_t, bool);
        Gfunc* getG();
    
    private:
        int table_size;     // size of the hash table
        Gfunc* g;           // g function used to hash the images
        std::vector<std::multimap<u_int32_t, Image *>> table;   // hash table. Each bucket is a multimap that stores the (id, image).
                                                                // The id is the result of the Idi function, in order to utilize the quering trick
};

// class to represent the L hash tables used in lsh
class Hash{

    public:
        Hash(DataSet &dataset, int L, int w, int k);
        ~Hash();
        std::vector<Image *> getCandidates(Image &query_image, bool id_flag, int table_id);
        int getL();
        int getW();
        int getK();
        int getDimension();
    
    private:
        std::vector<HashTableLSH*> tables;  // vector of hash tables
        int hash_table_size;        // size of each hash table
        int dimension;              // dimension of the images
        int L, w, k;                // L is the number of hash tables, w is the window size, k is the number of h functions
};

#endif