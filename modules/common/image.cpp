#include "../../include/image.h"

// Constructor of Image class. Stores the image and its index (decided by the order of the images in the binary file)
Image::Image(std::vector<unsigned char> image, int index) {
    this->image = image;
    this->index = index;
}

// Destructor of Image class
Image::~Image() {
    
}

// Returns the index of the image
int Image::getIndex() {
    return this->index;
}

// Returns the image
std::vector<unsigned char> Image::getImage() {
    return this->image;
}

// Returns the pixel at position i
unsigned char Image::getPixel(int i) {
    return this->image[i];
}
