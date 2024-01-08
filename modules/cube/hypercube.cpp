#include "../../include/hypercube.h"

// Constructor
HyperCube::HyperCube(DataSet &dataset, int dimension, int w, int k, int M, int probes) {
    
    // Initialize the variables
    this->dimension = dimension;
    this->k = k;
    this->M = M;
    this->probes = probes;
    
    // Initialize the hi_funcs
    for (int i = 0; i < k; i++) {
        HiFunc *hi_func = new HiFunc(dimension, w);
        this->hi_funcs.push_back(hi_func);
    }
    
    // Initialize the hash table
    this->h_cube = new HashTable(pow(2, k));
    // For each image in the dataset push it to the hash table
    for (int i = 0; i < dataset.getNumImages(); i++){
        // Get the image
        Image *image = dataset.getImages()[i];
        // Get the id
        int32_t key = this->hashFunction(*image);
        // Push the image to the hash table
        this->h_cube->push(image, key);
    }
}

// Destructor
HyperCube::~HyperCube() {
    // Delete the hi_funcs
    for (int i = 0; i < this->k; i++)
        delete this->hi_funcs[i];
    // Delete the hash table
    delete this->h_cube;
}

// Returns the M value
int HyperCube::getM() {
    return this->M;
}

// Returns the probes value
int HyperCube::getProbes() {
    return this->probes;
}

// Returns the k value
int HyperCube::getK() {
    return this->k;
}

// Returns the dimension value
int HyperCube::getDimension() {
    return this->dimension;
}

// uses hamming distance in order to return at maximum probes closest edges from given edge. it returns a vector of buckets of images
std::vector<Image*> HyperCube::closestVertices(int32_t key) {

    // get the candidates from vertex that is represented by the key
    int M = this->getM();   // maximum candidates you should gather from any vertexes totally
    int probes = this->getProbes(); // maximum number of vertexes you should visit

    // for maximum probes vertices, get the candidates. start from hamming distance 0.
    std::vector<Image*> result;     // returns max M candidates
    int hamm_dist = 0, vertices_count = 0;

    // get the candidates from the vertex that is represented by the key
    std::vector<Image*> candidates = this->getCandidates(key, M);
    
    // one vertex visited
    vertices_count++;

    // decrease M by the number of candidates you already have
    M -= candidates.size();      

    // add the candidates to the result
    for (int i = 0; i < candidates.size(); i++)
        result.push_back(candidates[i]);
    
    // if you already have M candidates, return them and don't visit any other vertex
    if(M == 0 || vertices_count==probes) 
        return result;   

    // increase hamming distance and get the candidates from the next vertexes, as long as you dont go over M candidates, or you dont visit more than probes vertexes
    while(true) {

        // increase hamming distance
        hamm_dist++;

        // get all the keys-vertices that differ from the key by hamming distance    
        std::vector<int32_t> neighbours_keys = hammingNumbers(key, this->k, hamm_dist);
        
        // loop through all the keys-vertices
        for(int i=0; i<neighbours_keys.size(); i++) {

            // get the candidates from the vertex that is represented by the key
            std::vector<Image*> candidates = this->getCandidates(neighbours_keys[i], M);
            // one vertex visited
            vertices_count++;
            // decrease M by the number of candidates you already have
            M -= candidates.size();
            // add the candidates to the result      
            for (int i = 0; i < candidates.size(); i++)
                result.push_back(candidates[i]);

            // check if you already have M candidates, or you visited more than probes vertexes. If so, stop and return the result
            if(M == 0 || vertices_count==probes)
                return result;   // if you already have M candidates, return them
        }

    }

    return result;
}

// Hash function. Returns the vertex key for the given image
int32_t HyperCube::hashFunction(Image &p){

    // Get the h(p) values
    std::vector<int32_t> h_values(this->k);
    for (int i = 0; i < this->k; i++){
        
        // Get the h value for the i-th hi function
        h_values[i] = this->hi_funcs[i]->hi(p);

        // If f[i](h_values[i]) is not set, it's time to calculate it.
        if (this->f_values.find(h_values[i]) == this->f_values.end()) {
            // flip a coin to decide the f value (0 or 1), save it to the map
            int16_t f_value = coinToss();
            this->f_values[h_values[i]] = f_value;
        } 
    }

    // Combine the f values to get the key. (Form the binary number that corresponds to a vertex)
    int32_t key = 0;
    for (int i = 0; i < this->k; i++)
        key = (key << 1) | this->f_values[h_values[i]];

    // this is the key of the vertex that the image belongs to
    return key;
}

// Get maximum M candidates from the cube
std::vector <Image*> HyperCube::getCandidates(int32_t key, int M) {
    std::vector <Image*> candidates = this->h_cube->getCandidates(key, M);
    return candidates;
}

// Simple function for coin toss. Returns 0 or 1 randomly
int16_t HyperCube::coinToss() {
    int16_t random_number = rand() % 2;
    return random_number;
}



