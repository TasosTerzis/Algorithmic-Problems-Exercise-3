#include "../../include/utilitiesCube.h"

// function to find k exact nearest neighbors of a query, by brute force
std::multimap <double, Image*> bruteForceCube(DataSet& input_set, Image& query, int k) {
    
    std::multimap <double, Image*> nearest_neighbors;
    std::vector <Image*> images = input_set.getImages();
    int dimension = input_set.getDimension();
    int num_images = input_set.getNumImages();

    // calculate distance between query and all images
    for(int i=0; i<num_images; i++) {
        double dist = nnDistance(query, *images[i], dimension, 2);
        nearest_neighbors.insert({dist, images[i]});
    }

    // get the k best candidates
    std::multimap <double, Image*> result;
    int count=0;
    for(std::multimap <double, Image*>::iterator it = nearest_neighbors.begin(); it != nearest_neighbors.end(); it++) {
        result.insert({it->first, it->second});
        count++;
        if(count == k) break;
    }
    return result;  
}

// function to find the approximately k nearest neighbors of a query
std::multimap <double, Image*> kNearestNeighbour(HyperCube& input_cube, Image & query, int k) {

    // get the key of the query
    int32_t key = input_cube.hashFunction(query);
    
    // Initialize the nearest neighbors
    std::multimap <double, Image*> nearest_neighbors;

    // closestVertices returns all candidates
    std::vector<Image *> candidates = input_cube.closestVertices(key);
    
    // For each candidate
    for (int i = 0; i < candidates.size(); i++) {
        // Get the image
        Image *image = candidates[i];
        // Calculate the distance
        double dist = nnDistance(query, *image, input_cube.getDimension(), 2);
        // Insert the pair (distance, image) to the nearest neighbors
        nearest_neighbors.insert({dist, image});
    }

    // Return the k nearest neighbors
    std::multimap <double, Image*> result;
    int count=0;
    for(std::multimap <double, Image*>::iterator it = nearest_neighbors.begin(); it != nearest_neighbors.end(); it++) {
        result.insert({it->first, it->second});
        count++;
        if(count == k) break;
    }

    return result; 


}
// function to find neihgbors inside a radius (range search)
std::vector <Image*> rangeSearch (HyperCube & input_cube, Image & query , int radius) {

    // get the key of the query
    int32_t key = input_cube.hashFunction(query);
    
    // Initialize the nearest neighbors
    std::vector <Image*> nearest_neighbors;

    // closestVertices returns all candidates
    std::vector<Image *> candidates = input_cube.closestVertices(key);
    
    // return all images that are inside the radius
    for (int i = 0; i < candidates.size(); i++) {
        //check for duplicates (even though it is not possible)
        if (std::find(nearest_neighbors.begin(), nearest_neighbors.end(), candidates[i]) != nearest_neighbors.end())
            continue;
        Image *image = candidates[i];
        double dist = nnDistance(query, *image, input_cube.getDimension(), 2);
        if (dist < radius)
            nearest_neighbors.push_back(image);
    }

    return nearest_neighbors;
}


