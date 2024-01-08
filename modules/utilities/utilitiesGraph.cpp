#include "../../include/utilitiesGraph.h"

// function to find k exact nearest neighbors of a query, by brute force
std::map<double, Image *> bruteForce(DataSet &input_set, Image &query, int k){

    std::map<double, Image *> nearest_neighbors;
    std::vector<Image *> images = input_set.getImages();
    int dimension = input_set.getDimension();
    int num_images = input_set.getNumImages();

    // calculate distance between query and all images
    for (int i = 0; i < num_images; i++) {
        double dist = nnDistance(query, *images[i], dimension, METRIC);
        nearest_neighbors.insert({dist, images[i]});
    }

    // get the k best candidates
    std::map<double, Image *> result;
    int count = 0;
    for (std::map<double, Image *>::iterator it = nearest_neighbors.begin(); it != nearest_neighbors.end(); it++) {
        result.insert({it->first, it->second});
        count++;
        if (count == k)
            break;
    }
    return result; // return k best neighbors from all hash tables

}

// function to find k approximate nearest neighbors of a query, using GNNS
std::map<double, Image *> graphNNSearch(Graph &graph, Image &query, int E, int R, int N){

    // S is a map of pairs "distance/image", sorted by distance. Run for R restarts
    std::map <double, Image*> S;
    for (int r = 1; r <= R; r++){
        
        // T is number of greedy steps
        Image* y[T] = {NULL};
        
        //chose a random image from the graph (uniformly)
        int randomIndex = rand() % graph.getSize();
        y[0] = graph.getImage(randomIndex);

        // greedy loop
        for (int t = 1; t < T; t++){
            
            // add the first E neighbors of y[t-1] to y[t] using the n function
            std::vector<Image *> neighbors = graph.nFunc(*y[t-1], E);
            std::multimap<double, Image *> temp_neighbors;
            
            // append the neighbors to S (S = S ∪ N (Yt−1,E ,G))
            for (int i = 0; i < neighbors.size(); i++){

                // S is a map, calculate distances and insert the pairs "distance/image" to S
                double dist = nnDistance(*neighbors[i], query, graph.getDimension(), METRIC);
                S.insert(std::make_pair(dist, neighbors[i]));

                // Insert the pairs "distance/image" to temp_neighbors, in order to sort them by distance
                temp_neighbors.insert(std::make_pair(dist, neighbors[i]));
                
                // min distance is the first element of temp_neighbors
                y[t] = temp_neighbors.begin()->second;
                
                // Calculate the y[t] of the current iteration. You can search either in S or just in "neighbours"
                // We'll search only in neighbors
                // double minDistance = std::numeric_limits<double>::max();
                // for (int j = 0; j<neighbors.size(); j++){
                //     double dist = nnDistance(*neighbors[j], query, graph.getDimension(), METRIC);
                //     if (dist < minDistance){
                //         minDistance = dist;
                //         y[t] = neighbors[j];
                //     }
                // }

                // Alternative: search in S
                // double minDistance = std::numeric_limits<double>::max();
                // for (auto it = S.begin(); it != S.end(); it++){
                //     if (it->first < minDistance){
                //         minDistance = it->first;
                //         y[t] = it->second;
                //     }
                // }
                // min distance is the first element of S
                // y[t] = S.begin()->second;
            }
        }      
    }

    // the map S is sorted by the distance of the images from the query
    // return N points in S with the smallest distance from the query
    std::map<double, Image*> result;
    int count = 0;
    for (auto it = S.begin(); it != S.end(); it++){
        if (count == N)
            break;
        result.insert(std::make_pair(it->first, it->second));
        count++;
    }
    return result;

}

// Generic Search on Graph
std::map<double, Image *> searchOnGraph (Graph &graph, Image &q, int L, int k){

    std::map<Image*, bool> visited;         // map of visited images
    std::map <double, Image*> R;            // map of pairs "distance/image", sorted by distance
    Image* p = graph.getCentroid();         // navigating node. Start from the centroid of the graph

    // insert the pair "distance q from p, image p", to map R
    R.insert(std::make_pair(nnDistance(*p, q, graph.getDimension(), METRIC), p));
    
    int i=1;
    bool running = true;
    while (running) {
        
        // R_temp is a map of pairs "image_index, (distance, image)".
        std::map<int, std::pair<double, Image*>> R_temp;
        
        // get the first element of R which is not visited
        auto it = R.begin();
        while (visited.find(it->second) != visited.end())
            it++;
        if (it == R.end())
            break;
        p = it->second;

        // mark p as visited
        visited.insert(std::make_pair(p, true));

        // get the neighbors of p.
        std::list <Image*> neighbors = graph.getNeighbors(*p);
        
        // insert the neighbors to R. R is a map, so if the key already exists, the new pair is not inserted
        for (auto it = neighbors.begin(); it != neighbors.end(); it++){
            
            // insert, check if we have reached L elements
            R_temp.insert(std::make_pair((*it)->getIndex(), std::make_pair(nnDistance(**it, q, graph.getDimension(), METRIC), *it)));
            if(++i==L){
                running = false;
                break;
            }
        }

        // get the pairs "image_index, image" from map R_temp, sort them by distance and insert them to map "R"
        for (auto it = R_temp.begin(); it != R_temp.end(); it++)
            R.insert(std::make_pair(it->second.first, it->second.second));
        
    }

    // return the first k elements of R
    std::map<double, Image*> result;
    int count = 0;
    for (auto it = R.begin(); it != R.end(); it++){
        if (count == k)
            break;
        result.insert(std::make_pair(it->first, it->second));
        count++;
    }

    return result;

}