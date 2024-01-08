#include "../../include/helpers.h"

// Fast distance function. works only for k=2, as it uses sqrt instead of pow
double nnDistanceFast(Image &image1, Image &image2, int dimension) {

    // Get the pixels of the images
    const std::vector<unsigned char>& image1_pixels = image1.getImage();
    const std::vector<unsigned char>& image2_pixels = image2.getImage();
    
    double distance = 0.0;
    for (int i = 0; i < dimension; i++) {
        double diff = static_cast<double>(image1_pixels[i]) - static_cast<double>(image2_pixels[i]);
        distance += diff * diff;
    }

    return sqrt(distance); // Use sqrt instead of pow for k=2 (Euclidean distance)
}

// Distance function. Works for every k
double nnDistance(Image &image1, Image &image2, int dimension, int k = 2) {
    
    // Use the fast distance function for k=2
    if(k == 2) 
        return nnDistanceFast(image1, image2, dimension);
    
    std::vector <unsigned char> image1_pixels = image1.getImage();
    std::vector <unsigned char> image2_pixels = image2.getImage();
    
    double distance = 0.0;
    for (int i = 0; i < dimension; i++) 
        distance += pow(std::abs(image1_pixels[i] - image2_pixels[i]), k);

    return pow(distance, 1.0 / k);
}

// Function to swap the endian of an integer
int swapEndian(int value) {
    return ((value >> 24) & 0x000000FF) |
           ((value >> 8) & 0x0000FF00) |
           ((value << 8) & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

// Function to calculate the modulo of two numbers
u_int32_t modulo(u_int32_t a, u_int32_t b) {
    int32_t r = a % b;
    return r < 0 ? r + b : r;
}

// Helper function to set or clear a specific bit at a given position
int32_t setBit(int32_t num, int position, bool value) {
    return value ? (num | (1 << position)) : (num & ~(1 << position));
}

// Function to calculate all binary numbers with a given Hamming distance
std::vector<int32_t> hammingNumbers(int32_t binaryNumber, int k, int hammingDistance) {
    
    // Create a vector to store the Hamming numbers
    std::vector<int32_t> hammingNumbers;

    // Use a binary mask to generate numbers with the specified Hamming distance
    for (int i = 0; i < (1 << k); ++i) {
        
        // Count the number of set bits
        int setBits = __builtin_popcount(i);

        // If the number of set bits is equal to the Hamming distance, then add the number to the vector  
        if (setBits == hammingDistance) {
            int32_t hammingNumber = binaryNumber;
            for (int j = 0; j < k; ++j) 
                if ((i >> j) & 1)   // If the j-th bit is set, then flip it
                    hammingNumber = setBit(hammingNumber, j, !((hammingNumber >> j) & 1));
            hammingNumbers.push_back(hammingNumber);
        }
    }

    // Return the vector of all Hamming numbers
    return hammingNumbers;
}

// get all info from config file
void getConfInfo(ConfInfo& conf, std::string conf_file) {
    
    // open config file
    std::ifstream fp;
    fp.open(conf_file);
    std::string line;

    // get first line, save it to conf.k_means
    std::getline(fp, line);
    conf.k_means = std::stoi(line.substr(line.find(":") + 1));

    // get second line, save it to conf.L
    std::getline(fp, line);
    conf.L = std::stoi(line.substr(line.find(":") + 1));

    // get third line, save it to conf.k_lsh
    std::getline(fp, line);
    conf.k_lsh = std::stoi(line.substr(line.find(":") + 1));

    // get fourth line, save it to conf.M
    std::getline(fp, line);
    conf.M = std::stoi(line.substr(line.find(":") + 1));

    // get fifth line, save it to conf.k_cube
    std::getline(fp, line);
    conf.k_cube = std::stoi(line.substr(line.find(":") + 1));

    // get sixth line, save it to conf.probes
    std::getline(fp, line);
    conf.probes = std::stoi(line.substr(line.find(":") + 1));
    
}

