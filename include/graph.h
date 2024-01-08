#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <list>
#include "./image.h"
#include "./dataset.h"
#include "./hashLSH.h"
#include "./hashtable.h"
#include "./utilitiesLsh.h"

// We use euclidean distance for the metric
#define METRIC 2

// This class represents a graph. It is a vector of lists of images, illustrating the edges of the graph.
class Graph {

    public:
        Graph (DataSet& dataset, int);          // constructor for GNNS use
        Graph (DataSet& dataset, int, int);     // constructor for MRNG use
        ~Graph();
        int getSize();
        Image* getImage(int index);
        int getDimension();
        std::vector<Image *> nFunc(Image&, int);
        std::list<Image *> getNeighbors(Image &image);  // get the neighbors of an image, aka connected points in graph
        void setCentroid();                             // set the centroid of the graph, used for MRNG
        Image* getCentroid ();                          // get the centroid of the graph, used for MRNG

    private: 
        // The first image of each list is the image itself, the rest are its neighbors
        // the graph is a vector of lists of images.
        std::vector<std::list<Image *>> graph;           
        int k, dimension;                               // k is for gnns (lsh argument), dimension is the image dimension
        Image* centroid;                                // centroid of the graph, used only for MRNG

};
#endif