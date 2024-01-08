#include "../../include/graph.h"

// set the values used in LSH
#define L 5
#define W 30
#define K 4

// 1st constructor: Graph to be used for GNNS
Graph::Graph(DataSet &dataset, int k) {

    this->k = k;
    this->dimension = dataset.getDimension();
    this->centroid = NULL;  // not used in this constructor

    // create a hash object in order to find the k nearest neighbors of each image via LSH
    Hash *hash = new Hash(dataset, L, W, K);

    // Run loop for every image in the dataset
    for (int i = 0; i < dataset.getNumImages(); i++) {

        // retrieve the image and call kNearestNeighbour
        Image *image = dataset.getImage(i);
        std::map<double, Image *> neighbors = kNearestNeighbour(*hash, *image, k+1);

        // add the neighbors to the graph
        std::list<Image *> image_list;

        // append the image itself to the start of the list
        image_list.push_back(image);

        // if the first image of neighbors is the same as the image, do not add it to the list
        if (neighbors.begin()->second->getIndex() != image->getIndex())
            image_list.push_back(neighbors.begin()->second);

        // add the rest of the neighbors to the list
        for (auto it = ++neighbors.begin(); it != neighbors.end(); it++)
            image_list.push_back(it->second);

        // check if count is k+1, if it is, remove the last element
        if (image_list.size() > k+1)
            image_list.pop_back();

        // add the list to the vector
        this->graph.push_back(image_list);

    }

    // delete the hash object
    delete hash;
}

// 2nd constructor: Graph to be used for MRNG
Graph::Graph(DataSet &dataset, int k, int I){
    
    this->k = k;
    this->dimension = dataset.getDimension();
    this->centroid = NULL;  // initially set to NULL, will be calculated later

    // create a hash object in order to find the k nearest neighbors of each image via LSH. 
    Hash *hash = new Hash(dataset, L, W, K);

    for (int i = 0; i < dataset.getNumImages(); i++) {

        // retrieve the image and call kNearestNeighbour.
        // Rp theoretically contains all the images of the dataset except p, and is sorted by distance from p
        // However, for efficiency reasons, we get just a subset of Rp, by using kNearestNeighbour of LSH. set k to 1% of the dataset size 
        Image *p = dataset.getImage(i);
        std::map <double, Image *> Rp = kNearestNeighbour(*hash, *p, PERCENTAGE * dataset.getNumImages());

        // Remove the first image from Rp in case it is p itself
        if (Rp.begin()->second->getIndex() == p->getIndex())
            Rp.erase(Rp.begin());

        // Lp is the set of images that will be added to the graph as neighbors of p
        // add first element of rp to lp
        std::multimap<double, Image *> Lp = std::multimap<double, Image *>();
        Lp.insert(std::make_pair(Rp.begin()->first, Rp.begin()->second));

        // Rp_Lp is the set of images that belong to Rp but not to Lp
        // calculate difference of Rp - Lp
        std::multimap<double, Image *> Rp_Lp;
        std::set_difference(Rp.begin(), Rp.end(), Lp.begin(), Lp.end(), std::inserter(Rp_Lp, Rp_Lp.begin()), Rp_Lp.value_comp());

        // time to add images-neighbors to Lp
        // for any element r that belongs to Rp_Lp, and every t that belongs to Lp:
        for (auto r = Rp_Lp.begin(); r != Rp_Lp.end(); r++) {

            // for a specific r, check the condition
            bool condition = true;
            for (auto t = Lp.begin(); t != Lp.end(); t++) {

                // calculate the 3 sides of the triangle (prt)
                double pr = r->first;
                double pt = t->first;
                double rt = nnDistance(*r->second, *t->second, this->dimension, METRIC);

                // if pr is the longest side, then condition is not satisfied
                if ((pr > pt) && (pr > rt)) {
                    condition = false;
                    break;
                }

            }

            // Add r to Lp
            if (condition)
                Lp.insert(std::make_pair(r->first, r->second));
        
        }
        
        // Add the Lp neighbours-images, to the actual graph
        // sart by adding the currrent image to the start of the list
        std::list<Image *> image_list;
        image_list.push_back(p);
        for (auto it = Lp.begin(); it != Lp.end(); it++)
            image_list.push_back(it->second);
        this->graph.push_back(image_list);
    
    }

    // find a virtual centroid by averaging all the images of the graph
    std::vector<unsigned char> centroid(this->dimension, 0);
    for (int i = 0; i < this->graph.size(); i++)
        for (int j = 0; j < this->dimension; j++)
            centroid[j] += this->graph[i].front()->getPixel(j);

    // average the values
    for (int i = 0; i < this->dimension; i++)
        centroid[i] /= this->graph.size();

    // now find the real image, that is closest to this virtual centroid. Use brute force
    Image *centroid_image = this->graph[0].front();
    Image *virtual_centroid = new Image(centroid, -1);
    double min_distance = nnDistance(*centroid_image, *virtual_centroid, this->dimension, METRIC);
    for (int i = 1; i < this->graph.size(); i++) {
        // classic comparisons 
        double distance = nnDistance(*this->graph[i].front(), *virtual_centroid, this->dimension, METRIC);
        if (distance < min_distance) {
            min_distance = distance;
            centroid_image = this->graph[i].front();
        }
    }

    // delete the virtual centroid! It is not needed anymore
    delete virtual_centroid;
    this->centroid = centroid_image;

    // no need for the hash object anymore
    delete hash;

}

// get the dimension of the images in the graph
int Graph::getDimension(){
    return this->dimension;
}

// Destructor
Graph::~Graph(){
    for (int i = 0; i < this->graph.size(); i++)
        this->graph[i].clear();
    this->graph.clear();
}

// get the size of the graph
int Graph::getSize(){
    return this->graph.size();
}

// get the image at the given index
Image *Graph::getImage(int index){
    return this->graph[index].front();
}

// get the first E ≤ k neighbors of Y. Used for GNNS
std::vector<Image *> Graph::nFunc(Image &Y, int E){
    
    // check for errors
    if (E > this->k) {
        std::cout << "Error: E > k" << std::endl;
        exit(1);
    }

    // return the first E neighbors of Y
    std::vector<Image *> neighbors;
    for (auto it = ++this->graph[Y.getIndex()].begin(); it != this->graph[Y.getIndex()].end(); it++) {
        neighbors.push_back(*it);
        if (neighbors.size() == E)
            return neighbors;
    }

    // return the requested number of neighbors, of size <= E
    return neighbors;

}

// get the centroid of the graph. Retrun the image that is closest to the virtual centroid
Image *Graph::getCentroid(){
    return this->centroid;
}

// get the neighbors of an image, aka connected points in graph
std::list<Image *> Graph::getNeighbors(Image &image){

    // don't return the first element of the list, it is the image itself
    std::list<Image *> neighbors;
    for (auto it = ++this->graph[image.getIndex()].begin(); it != this->graph[image.getIndex()].end(); it++)
        neighbors.push_back(*it);
    return neighbors;

}