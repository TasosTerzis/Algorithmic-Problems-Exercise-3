#include "../../include/utilitiesLsh.h"

// function to find k exact nearest neighbors of a query, by brute force
std::map <double, Image*> bruteForceLsh(DataSet& input_set, Image& query, int k) {
    
    std::map <double, Image*> nearest_neighbors;
    std::vector <Image*> images = input_set.getImages();
    int dimension = input_set.getDimension();
    int num_images = input_set.getNumImages();

    // calculate distance between query and all images
    for(int i=0; i<num_images; i++) {
        double dist = nnDistance(query, *images[i], dimension, 2);
        nearest_neighbors.insert({dist, images[i]});
    }

    // get the k best candidates
    std::map <double, Image*> result;
    int count=0;
    for(std::map <double, Image*>::iterator it = nearest_neighbors.begin(); it != nearest_neighbors.end(); it++) {
        result.insert({it->first, it->second});
        count++;
        if(count == k) break;
    }

    return result;  // return k best neighbors from all hash tables
}

// function to find the approximately k nearest neighbors of a query
std::map <double, Image*> kNearestNeighbour(Hash& input_hash, Image & query, int k) {
    
    int L = input_hash.getL();
    std::map <double, Image*> nearest_neighbors;

    // for all hash tables
    for(int i=0; i<L; i++) {
    
        // get the candidates from current hash table
        std::vector <Image*> candidates;    
        candidates = input_hash.getCandidates(query, false, i);
        std::map <double, Image*> sorted_candidates;
        for(int j=0; j<candidates.size(); j++) {
            double dist = nnDistance(query, *candidates[j], input_hash.getDimension(), 2);
            sorted_candidates.insert({dist, candidates[j]});
        }

        // get k best neighbors from sorted_candidates. add them to nearest_neighbors
        int count=0;
        for(std::map <double, Image*>::iterator it = sorted_candidates.begin(); it != sorted_candidates.end(); it++) {
            nearest_neighbors.insert({it->first, it->second});
            count++;
            if(count == k) break;
        }
        
    }

    // // get the k best candidates from ALL hash tables
    std::map <double, Image*> result;
    int count=0;
    for(std::map <double, Image*>::iterator it = nearest_neighbors.begin(); it != nearest_neighbors.end(); it++) {
        result.insert({it->first, it->second});
        count++;
        if(count == k) break;
    }

    return result;  
}


// function to find neihbours inside a radius (range search)
std::vector <Image*> rangeSearch (Hash & input_hash, Image & query, int radius) {
        
    int L = input_hash.getL();
    std::vector <Image*> nearest_neighbors;

    for(int i=0; i<L; i++) {

        // get the candidates from current hash table
        std::vector <Image*> candidates;
        candidates = input_hash.getCandidates(query, false, i);
        for(int j=0; j<candidates.size(); j++) {
            //check if candidate already exists in nearest_neighbors
            if (std::find(nearest_neighbors.begin(), nearest_neighbors.end(), candidates[j]) != nearest_neighbors.end())
                continue;
            double dist = nnDistance(query, *candidates[j], input_hash.getDimension(), 2);
            if(dist < radius) 
                nearest_neighbors.push_back(candidates[j]);
        }
    }

    // contains all images that are inside the radius, from all hash tables
    return nearest_neighbors;  
}
