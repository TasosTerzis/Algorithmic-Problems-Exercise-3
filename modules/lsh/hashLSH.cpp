#include "../../include/hashLSH.h"

/* ----------------------------- HashTableLsh ------------------------------- */

// Constructor
HashTableLSH::HashTableLSH(int d, int w, int k, int table_size) {    
    this->table_size = table_size;
    this->table.resize(table_size);
    this->g = new Gfunc(d, w, k, table_size);   // each hash table has its own g function
}

// Destructor
HashTableLSH::~HashTableLSH() {
    delete this->g;
}

// Function to insert an image to the hash table. Key is the bucket id, id is the value used for the quering trick
void HashTableLSH::push(Image *image, u_int32_t key, u_int32_t id) {
    this->table[key].insert(std::pair<u_int32_t, Image *>(id, image));
}

// Function to get the candidates based on the g_key and id
std::vector<Image *> HashTableLSH::getCandidates(u_int32_t key, u_int32_t id, bool id_flag) {
    
    // For all the images in the bucket with key = g_key
    std::vector<Image *> candidates;
    for (auto it = this->table[key].begin(); it != this->table[key].end(); it++) {
        
        // check if the trick is used
        if (id_flag) {
            
            // If so, Get the id of the image
            u_int32_t check_id = it->first;
            // If the id of the image is the same as the id of the query, add the image to the candidates
            if (check_id == id) {
                // Add the image to the candidates
                Image *image = it->second;
                candidates.push_back(image);
            }
        }

        // If the trick is not used, add the image to the candidates, since it is found in the same bucket
        else {
            Image *image = it->second;
            candidates.push_back(image);
        }
    }

    // Return the candidates
    return candidates;
}

// Function to get the g function of the hash table
Gfunc* HashTableLSH::getG() {
    return this->g;
}


/* ----------------------------- Hash ------------------------------- */


// Constructor of Hash. Creates L hash tables
Hash::Hash(DataSet &dataset, int L, int w, int k) {
    
    this->hash_table_size = dataset.getNumImages() / TABLE_DIVISOR;
    this->L = L;
    this->dimension = dataset.getDimension();
    this->w = w;
    this->k = k;
    
    
    // Create L hash tables of size dataset.getNumImages() / TABLE_DIVISOR
    for (int i = 0; i < L; ++i) {
        HashTableLSH *table = new HashTableLSH(this->dimension, w, k, this->hash_table_size);
        this->tables.push_back(table);
    }

    // For each image in the dataset push it to the hash tables (using the idFunc and gFunc). All images are pushed to all hash tables 
    for (int i = 0; i < dataset.getNumImages(); i++) {
        
        Image *image = dataset.getImages()[i];
        // For each hash table
        for (int j = 0; j < this->L; j++) {
            // Get the id
            u_int32_t id = this->tables[j]->getG()->Idi(*image);
            // get g key
            u_int32_t g_key = this->tables[j]->getG()->Gi(id);
            // Push the image to the hash table
            this->tables[j]->push(image, g_key, id);
        }
    }
}

// Destructor, deletes all the hash tables
Hash::~Hash() {
    for (int i = 0; i < tables.size(); ++i) 
        delete tables[i];
}

// Function to get the candidates based on the g_key and id.
std::vector<Image *> Hash::getCandidates(Image &query_image, bool id_flag, int table_id) {
    // Create an empty vector of candidates
    std::vector<Image *> candidates;
    // Get the id
    u_int32_t id = this->tables[table_id]->getG()->Idi(query_image);
    // Get the g_key
    u_int32_t g_key = this->tables[table_id]->getG()->Gi(id);
    // Push back the candidates to the candidates vector
    std::vector<Image *> temp_candidates = this->tables[table_id]->getCandidates(g_key, id, id_flag);
    for (int j = 0; j < temp_candidates.size(); ++j)
        candidates.push_back(temp_candidates[j]);
    
    return candidates;
}

// Function to get total number of hash tables
int Hash::getL() {
    return this->L;
}

// function to get the W value
int Hash::getW() {
    return this->w;
}

// Function to get the k value
int Hash::getK() {
    return this->k;
}

// Function to get the dimension of the images stored in the hash tables
int Hash::getDimension() {
    return this->dimension;
}


