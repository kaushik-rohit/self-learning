#include <image.hpp>

Image process_image(const Image& img, const Filter& filter){
    Image imgCpy;
    std::memcpy(&imgCpy, &img, sizeof(Image));

    int width = imgCpy.width;
    int height = imgCpy.height;

    int filter_size = filter.height;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            
        }
    }

    return imgCpy;
}