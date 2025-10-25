#include<vector>
#include<iostream>
#include<string>
#include "stb_image.h"

void read_image() {

}

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

int main(int argc, char** argv) {
    if (argc < 2){
        std::cerr<<"Usage: ./image-processing algo\n";
        return 1;
    }

    try {
        Algo algo = parse_algo(argv[1]);

        switch(algo) {
            case Algo::CCL:
                
            case Algo::MEDIAN_FILTER:
            
        }

    }
}