#ifndef IMAGE_H
#define IMAGE_H

#include "./common.h"

// class to represent an Image as object
class Image {

    public:
        Image(std::vector<unsigned char>, int);
        ~Image();
        int getIndex();
        std::vector<unsigned char> getImage();
        unsigned char getPixel(int);
    private:
        std::vector<unsigned char> image;   // vector of unsigned chars to store the image's pixels
        int index;                          // index of the image in the dataset. (by the order they were read)
};

#endif