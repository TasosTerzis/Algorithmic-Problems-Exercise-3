#ifndef HASHTABLE_H
#define HASHTABLE_H


#include "./common.h"
#include "./image.h"
#include "./dataset.h"
#include "./funcG.h"

// class to represent one single hash table used in hypercube
class HashTable {

    public:
        HashTable(int);
        ~HashTable();
        void push(Image *, int32_t);
        std::vector<Image *> getCandidates(int32_t, int);
        int getTableSize();

    private:
        int table_size;     // size of the hash table
        std::vector<std::vector< Image *>> table;   // hash table. Each bucket is a vector that stores the images.
};

#endif