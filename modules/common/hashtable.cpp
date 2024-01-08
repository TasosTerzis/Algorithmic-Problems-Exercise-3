#include "../../include/hashtable.h"

// Constructor
HashTable::HashTable(int table_size) {
    this->table_size = table_size;
    this->table.resize(table_size);
}

// Destructor
HashTable::~HashTable() {
    
}

// Function to insert an image to the hash table
void HashTable::push(Image *image, int32_t key) {
    this->table[key].push_back(image);
}

// Function to get the candidates based on the g_key and id. Return maximum M candidates
std::vector<Image *> HashTable::getCandidates(int32_t key, int M) {

    // For all the images in the bucket with key g_key, find the ones with id = id
    std::vector<Image *> candidates;
    int count = 0;
    for (auto it = this->table[key].begin(); it != this->table[key].end(); it++) {
        // Add the image to the candidates
        Image *image = *it;
        candidates.push_back(image);
        count++;
        if(count == M) break;
    }
    // Return the candidates
    return candidates;
}

// Function to get the table size
int HashTable::getTableSize() {
    return this->table_size;
}
