#include "../../include/dataset.h"

// Constructor of DataSet class. Reads the binary file and stores the images in a vector
DataSet::DataSet(std::string inputFile){
    
    // Open the binary file
    std::ifstream file(inputFile, std::ios::binary);
    if (!file){
        std::cerr << "Failed to open the file." << std::endl; return; }

    // skip the magic number
    file.ignore(4);
    
    // Read and display the number of images (32-bit integer) in big-endian
    int temp_num_images;
    file.read(reinterpret_cast<char *>(&temp_num_images), sizeof(temp_num_images));
    this->num_images = swapEndian(temp_num_images); // Swap to big-endian

    // Read and display the number of rows (32-bit integer) in big-endian
    int num_rows;
    file.read(reinterpret_cast<char *>(&num_rows), sizeof(num_rows));
    num_rows = swapEndian(num_rows); // Swap to big-endian

    // Read and display the number of columns (32-bit integer) in big-endian
    int num_columns;
    file.read(reinterpret_cast<char *>(&num_columns), sizeof(num_columns));
    num_columns = swapEndian(num_columns); // Swap to big-endian

    // Calculate the dimension of the image
    this->dimension = num_rows * num_columns;

    // start a loop to read every image
    for(int i = 0; i < num_images; i++){

        // create a vector to store the image. read every pixel of the image
        std::vector <unsigned char> pixels(dimension);
        for(int j = 0; j < dimension; j++){
            unsigned char temp;
            file.read(reinterpret_cast<char *>(&temp), sizeof(temp));
            pixels[j] = temp;
        }

        // create an Image object and push it in DataSet
        Image* image = new Image(pixels, i);
        this->data_set.push_back(image);
    }

    // Close the file
    file.close();
}

// Destructor of DataSet class. Deletes all the images
DataSet::~DataSet(){
    for(int i = 0; i < this->num_images; i++)
        delete this->data_set[i];
}

// Returns the dimension of the images
int DataSet::getDimension () {
    return this->dimension;
}

// Returns the number of images
int DataSet::getNumImages () {
    return this->num_images;
}

// Returns the vector of images
std::vector <Image*> DataSet::getImages () {
    return this->data_set;
}

// Returns the image at position i
Image* DataSet::getImage(int i) {
    return this->data_set[i];
}


