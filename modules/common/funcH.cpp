#include "../../include/funcH.h"

// Constructor of HiFunc. Creates a random vector v and a random single-precision real number t
HiFunc::HiFunc(int d, int w) {
    
    this->w = w;
    std::random_device rd;
    std::default_random_engine generator(rd());

    // Create a random vector v with values between 0 and 1
    std::uniform_real_distribution<double> v_distribution(0.0, 1.0);
    for (int i = 0; i < d; i++)
        this->v.push_back(v_distribution(generator));

    // Create a random single-precision real number t in the range [0, w)
    std::uniform_real_distribution<double> t_distribution(0.0, static_cast<double>(w));
    this->t = t_distribution(generator);

}

// Destructor of HiFunc
HiFunc::~HiFunc() {

}

// Returns the value of h(p)
u_int32_t HiFunc::hi(Image &p) {
    
    // use inner_product
    std::vector<unsigned char> pixels = p.getImage();
    std::vector<double> image_pixels(pixels.begin(), pixels.end());
    double dot_product = std::inner_product(image_pixels.begin(), image_pixels.end(), this->v.begin(), 0.0);
    
    // calculate the value of h(p)
    u_int32_t h_value = static_cast<u_int32_t>(std::floor((dot_product + this->t) / this->w));
    return h_value;
}