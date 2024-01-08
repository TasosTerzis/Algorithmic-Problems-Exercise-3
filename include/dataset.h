#ifndef CLASSES_H
#define CLASSES_H

#include "./common.h"
#include "./image.h"
#include "./helpers.h"

// Class to store all the images of the dataset
class DataSet{
    
    public:
        DataSet(std::string);
        ~DataSet();
        int getDimension();
        int getNumImages();
        std::vector <Image*> getImages();
        Image* getImage(int);
    
    private:
        int dimension;      // dimension of images
        int num_images;     // number of images
        std::vector <Image*> data_set;  // data set contains num_images Images
};

#endif