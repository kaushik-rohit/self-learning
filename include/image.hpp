#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include <vector>
#include <stb_image.h>

struct Image {
    int width;
    int height;
    int channels;

    float* pixels;
};

struct Filter {
    int width;
    int height;

    std::vector<std::vector<double>> values;
};

Image process_image(const Image& img, const Filter& filter);

Image read_image(char const* path) {
    Image image;
    image.pixels = stbi_loadf(path, &image.width, &image.height, &image.channels, 3);

    return image;
};