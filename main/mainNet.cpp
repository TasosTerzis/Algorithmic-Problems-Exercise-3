#include "../include/utilitiesGraph.h"

int main(int argc, char **argv) {

    // get the arguments from the command line. We set some default values in case the user doesn't give them
    srand(time(NULL));
    std::string input_file = "", query_file = "", output_file = "";
    std::string input_file_REDUCED = "";
    std::string query_file_REDUCED = "";
    int E = 30, R = 1, N = 1, k = 50, l = 20, m = 1;
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-d") == 0)
            input_file = argv[i + 1];
        else if (strcmp(argv[i], "-q") == 0)
            query_file = argv[i + 1];
        else if (strcmp(argv[i], "-k") == 0)
            k = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-E") == 0)
            E = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-R") == 0)
            R = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-N") == 0)
            N = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-l") == 0)
            l = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-m") == 0)
            m = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-o") == 0)
            output_file = argv[i + 1];
        else if(strcmp(argv[i], "-rd") == 0)
            input_file_REDUCED = argv[i + 1];
        else if(strcmp(argv[i], "-rq") == 0)
            query_file_REDUCED = argv[i + 1];
    }

    // check that the arguments are valid
    while (l < N) {
        std::cout << "N must be smaller than l. Please select new values:" << std::endl;
        std::cout << "N: ";
        std::cin >> N;
        std::cout << "l: ";
        std::cin >> l;
    }

    // if input file doesn't exist, ask for it
    while (input_file == "") {
        std::cout << "Please enter the input file: ";
        std::cin >> input_file;
        // check that the file was created successfully
        std::ifstream file(input_file);
        if (!file.good()) {
            std::cout << "File not found" << std::endl;
            input_file = "";
        }
    }

    while (input_file_REDUCED == ""){
        std::cout << "Please enter the reduced input file: ";
        std::cin >> input_file_REDUCED;
        // check that the file was created successfully
        std::ifstream file(input_file_REDUCED);
        if (!file.good()) {
            std::cout << "File not found" << std::endl;
            input_file_REDUCED = "";
        }
    }
    
    // Create the dataset based on the input file
    // save the dimension and number of images of the dataset
    DataSet *input_set = new DataSet(input_file);
    int dimension = input_set->getDimension();
    int num_images = input_set->getNumImages();
    std::cout << "Number of images: " << num_images << std::endl;
    std::cout << "Dimension: " << dimension << std::endl;

    // save the dimension and number of images of the dataset
    DataSet *reduced_input_set = new DataSet(input_file_REDUCED);
    int dimension_reduced = reduced_input_set->getDimension();
    std::cout << "Reduced Dimension: " << dimension_reduced << std::endl;

    // create the graph, based on the user's choice (GNNS or MRNG)
    Graph *graph;
    if (m == 1) {
        graph = new Graph(*reduced_input_set, k);
        std::cout << "\nGraph created" << std::endl;
    }
    else if (m == 2) {
        graph = new Graph(*reduced_input_set, k, l);
        std::cout << "\nGraph created" << std::endl;
    }
    // start the loop that asks for multiple program executions
    bool running_flag = true;
    while (running_flag){

        // if query file doesn't exist, ask for it
        while (query_file == ""){
            std::cout << "Please enter the query file: ";
            std::cin >> query_file;
            // check that the file was created successfully
            std::ifstream file(query_file);
            if (!file.good()) {
                std::cout << "File not found" << std::endl;
                query_file = "";
            }
        }

        while(query_file_REDUCED == ""){
            std::cout << "Please enter the reduced query file: ";
            std::cin >> query_file_REDUCED;
            // check that the file was created successfully
            std::ifstream file(query_file_REDUCED);
            if (!file.good()) {
                std::cout << "File not found" << std::endl;
                query_file_REDUCED = "";
            }
        }

        // If output file doesn't exist, ask for it
        while (output_file == ""){
            std::cout << "Please enter the output file: ";
            std::cin >> output_file;
        }

        // Create the query dataset
        DataSet *query_set = new DataSet(query_file);
        // create the reduced dataset and query set
        DataSet *reduced_query_set = new DataSet(query_file_REDUCED);

        // print to output file
        std::ofstream output;
        output.open(output_file, std::ios_base::app);
        if (m == 1)
            output << "GNNS Results" << std::endl;
        else if(m == 2)
            output << "MRNG Results" << std::endl;
        else
            output << "Exhaustive Search Results" << std::endl;

        // for every image in the query set, call graphNNSearch / searchOnGraph for approximate values. Also call bruteForce search for true values
        // need to save the stats
        double seconds = 0;
        double bf_seconds = 0;
        double total_af = 0;
        
        for (int i = 0; i < reduced_query_set->getNumImages(); i++){

            // get the query image
            Image *reduced_query_image = reduced_query_set->getImage(i);
            Image *query_image = query_set->getImage(i);

            // print the query image to the output file
            output << "Query: " << reduced_query_image->getIndex() << std::endl;

            // calculate time needed for graph search
            auto start = std::chrono::high_resolution_clock::now();
            std::map<double, Image *> neighbours;
            if (m == 1)
                neighbours = graphNNSearch(*graph, *reduced_query_image, E, R, N);
            else if(m == 2)
                neighbours = searchOnGraph(*graph, *reduced_query_image, l, N);
            else
                neighbours = bruteForce(*reduced_input_set, *reduced_query_image, N);
            auto finish = std::chrono::high_resolution_clock::now();
            
            // get the duration, save in seconds
            std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
            seconds += duration.count();

            // for each image in the neighbours, get the corresponding image from the original input set, and calculate the distance
            std::map<double, Image *> approx_nearest_neighbours;
            for (auto it = neighbours.begin(); it != neighbours.end(); it++){
                // get the index of the image
                int index = it->second->getIndex();
                // get the image from the original input set
                Image* image = input_set->getImage(index);
                // calculate the distance between the query image and the image
                double distance = nnDistance(*query_image, *image, input_set->getDimension(), 2);
                // add the distance and the image to the map
                approx_nearest_neighbours.insert(std::pair<double, Image *>(distance, image));
            }

            // calculate time for brute force in original dimension
            auto bf_start = std::chrono::high_resolution_clock::now();
            std::map<double, Image *> real_nearest_neighbours = bruteForce(*input_set, *query_image, N);
            auto bf_finish = std::chrono::high_resolution_clock::now();
            
            // get the duration of brute force, save in seconds
            std::chrono::duration<double> bf_duration = std::chrono::duration_cast<std::chrono::duration<double>>(bf_finish - bf_start);
            bf_seconds += bf_duration.count();

            // calculate the approximation factor (using the first image in the map)
            total_af += approx_nearest_neighbours.begin()->first / real_nearest_neighbours.begin()->first;
 
            // loop through the results and print them to the output file
            auto it1 = approx_nearest_neighbours.begin();
            auto it2 = real_nearest_neighbours.begin();
            int j = 1;
            while ((it1 !=neighbours.end()) && (it2 != real_nearest_neighbours.end()))
            {
                output << "Nearest neighbours-" << j << ": " << it1->second->getIndex() << std::endl;
                output << "distanceApproximate: " << it1->first << std::endl;
                output << "distanceTrue: " << it2->first << std::endl;
                it1++, it2++, j++;
            }

        }

        // print final time results, and MAF
        output << "tAverageApproximate: " << seconds / query_set->getNumImages() << " s" << std::endl;
        output << "tAverageTrue: " << bf_seconds / query_set->getNumImages() << " s" << std::endl;
        //calculate the average approximation factor
        double af = total_af / query_set->getNumImages();
        output << "AF: " << af << std::endl  << std::endl;

        // free the query set
        delete query_set;
        // free the reduced query set
        delete reduced_query_set;
        // ask if user wants to continue 
        std::string answer;
        std::cout << "Do you want to continue? (y/n): ";
        std::cin >> answer;
        if (answer == "n" || answer == "N")
            running_flag = false;
        else if (answer == "y" || answer == "Y")
            query_file = "";

    }

    // free the reduced input set
    delete reduced_input_set;
    if(m == 1 || m == 2)
        delete graph;

    // Delete the dataset and the graph
    delete input_set;
    return 0;
    
}