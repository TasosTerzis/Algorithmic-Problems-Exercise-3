#include "../include/utilitiesCube.h"

int main(int argc, char** argv){

    // get the arguments from the command line 
    srand(time(NULL));
    std::string input_file = "", query_file = "", output_file = "";
    int N = 1, R = 10000, k = 14, M = 10, probes = 2;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            input_file = argv[i + 1];
        else if (strcmp(argv[i], "-q") == 0)
            query_file = argv[i + 1];
        else if (strcmp(argv[i], "-o") == 0)
            output_file = argv[i + 1];
        else if (strcmp(argv[i], "-k") == 0)
            k = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-M") == 0)
            M = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-probes") == 0)
            probes = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-N") == 0)
            N = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-R") == 0)
            R = atoi(argv[i + 1]);
    }

    //if input file doesn't exist, ask for it
    while (input_file == "") {
        std::cout << "Please enter the input file: ";
        std::cin >> input_file;
        //check that the file was created successfully
        std::ifstream file(input_file);
        if (!file.good()) {
            std::cout << "File not found" << std::endl;
            input_file = "";
        }
    }

    // Create the dataset based on the input file
    DataSet *input_set = new DataSet(input_file);

    // save the dimension and number of images of the dataset
    int dimension = input_set->getDimension();
    int num_images = input_set->getNumImages();
    std::cout << "Dimension: " << dimension << std::endl;
    std::cout << "Number of images: " << num_images << std::endl;

    // Create the Cube object, to represent the hypercube. Pass the dataset in order to hash the images, and the parameters k, M, probes
    HyperCube *input_cube = new HyperCube(*input_set, dimension, W_CUBE, k, M, probes);
    std::cout << "Added dataset to Hypercube" << std::endl;

    // start the loop that asks for multiple program executions
    bool running_flag = true;
    while (running_flag){

        // if query file doesn't exist, ask for it
        while (query_file == "") {
            std::cout << "Please enter the query file: ";
            std::cin >> query_file;
            //check that the file was created successfully
            std::ifstream file(query_file);
            if (!file.good()) {
                std::cout << "File not found" << std::endl;
                query_file = "";
            }
        }

        //If output file doesn't exist, ask for it
        while (output_file == "") {
            std::cout << "Please enter the output file: ";
            std::cin >> output_file;
        }
        
        // Create the query dataset
        DataSet *query_set = new DataSet(query_file);

        std::ofstream output;
        output.open(output_file, std::ios_base::app);

        // for every image in the query set, call kNearestNeighbour:
        double kNN_seconds = 0;
        double bf_seconds = 0;
        double af = 0;
        double maf = std::numeric_limits<double>::min();
        for (int i = 0; i < query_set->getNumImages(); i++) {

            // get the query image
            Image *query_image = query_set->getImage(i);
            
            // print to output file
            output << "Query image: " << query_image->getIndex() << std::endl;

            // calculate time needed to find k nearest neighbours
            auto kNN_start = std::chrono::high_resolution_clock::now();
            std::multimap<double, Image *> nearest_neighbours = kNearestNeighbour(*input_cube, *query_image, N);
            auto kNN_finish = std::chrono::high_resolution_clock::now();
            // get the duration of kNN, save in seconds
            std::chrono::duration<double> kNN_duration = std::chrono::duration_cast<std::chrono::duration<double>>(kNN_finish - kNN_start);
            kNN_seconds += kNN_duration.count();

            // calculate time for brute force
            auto bf_start = std::chrono::high_resolution_clock::now();
            std::multimap<double, Image *> real_nearest_neighbours = bruteForceCube(*input_set, *query_image, N);
            auto bf_finish = std::chrono::high_resolution_clock::now();
            // get the duration of brute force, save in seconds
            std::chrono::duration<double> bf_duration = std::chrono::duration_cast<std::chrono::duration<double>>(bf_finish - bf_start);
            bf_seconds += bf_duration.count();

            // calculate the maximum approximation factor
            double current_maf = nearest_neighbours.begin()->first / real_nearest_neighbours.begin()->first;
            af += current_maf;
            if (current_maf > maf)
                maf = current_maf;
            
            // loop through the results and print them to the output file
            auto it1 = nearest_neighbours.begin();
            auto it2 = real_nearest_neighbours.begin();
            int j = 1;
            while ((it1 != nearest_neighbours.end()) && (it2 != real_nearest_neighbours.end())) {
                output << "Nearest neighbours-" << j << ": " << it1->second->getIndex() << std::endl;
                output << "distanceCUBE: " << it1->first << std::endl;
                output << "distanceTrue: " << it2->first << std::endl;
                it1++, it2++, j++;
            }

        }

        // print final time results, and MAF
        output << "tAverageApproximate: " << kNN_seconds / query_set->getNumImages() << " s" << std::endl;
        output << "tAverageTrue: " << bf_seconds / query_set->getNumImages() << " s" << std::endl;
        output << "AAF: " << af / query_set->getNumImages() << std::endl;
        output << "MAF: " << maf << std::endl << std::endl;
        output << "w: " << W_CUBE << std::endl;
        output << "k: " << k << std::endl;
        output << "M: " << M << std::endl;
        output << "probes: " << probes << std::endl << std::endl;

        // free the query set
        delete query_set;

        // ask if user wants to continue
        std::string answer;
        std::cout << "Do you want to continue? (y/n): ";
        std::cin >> answer;
        if (answer == "n" || answer == "N")
            running_flag = false;
        else if(answer == "y" || answer == "Y")
            query_file = "";
        
    }

    // free resources
    delete input_set;
    delete input_cube;
    return 0;
}