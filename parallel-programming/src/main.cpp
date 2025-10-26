#include <vector>
#include <iostream>
#include <format>
#include <string>
#include "image.h"

enum class Algo {
    MEDIAN_FILTER,
    SGB,
    SOBEL,
    LUCAS_KADANE,
    CCL,
    CANNY_EDGE
};

Algo parse_algo(const std::string& arg) {
    static const std::unordered_map<std::string, Algo> map = {
        {"median_filter", Algo::MEDIAN_FILTER},
        {"sgb", Algo::SGB},
        {"sobel", Algo::SOBEL}
    };

    auto it = map.find(arg);

    if(it == map.end()){
        throw std::invalid_argument("Unknown algorithm: " + arg);
    }

    return it->second;
}

void summarize_image(const Image& img) {
    std::cout<<std::format("Image is of width {} height {} and channels {}", img.width, img.height, img.channels)<<std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2){
        std::cerr<<"Usage: ./image-processing algo\n";
        return 1;
    }
    try {
        Algo algo = parse_algo(argv[1]);

        Image image = read_image("../data/orig.jpg");
        summarize_image(image);

        switch(algo) {
            case Algo::CCL:
                
            case Algo::MEDIAN_FILTER:
                std::cout<<"Running median filter"<<std::endl;
            case Algo::CANNY_EDGE:

            case Algo::LUCAS_KADANE:

            case Algo::SGB:

            case Algo::SOBEL:
            
        }
    } catch(const std::exception& e) {
        std::cerr<< "Caught exception "<< e.what() <<"\n";
        return 1;
    }

    return 0;
}