#include <omp.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define RGB_COMPONENT_COLOR 255

struct PPMPixel
{
    int red;
    int green;
    int blue;
};

struct PPMImage
{
    int x;
    int y;
    int all;
    PPMPixel *data;
};

void readPPM(const char *filename, PPMImage &img)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "the file: " << filename << " was not found" << std::endl;
        exit(1);
    }

    std::string format;
    int rgb_comp_color;
    file >> format;
    if (format != "P3")
    {
        std::cout << "error in format" << std::endl;
        exit(2);
    }

    file >> img.x >> img.y;
    file >> rgb_comp_color;
    img.all = img.x * img.y;
    img.data = new PPMPixel[img.all];

    for (int i = 0; i < img.all; ++i)
    {
        file >> img.data[i].red >> img.data[i].green >> img.data[i].blue;
    }

    std::cout << format << std::endl;
    std::cout << "x=" << img.x << " y=" << img.y << " all=" << img.all << std::endl;
}

void writePPM(const char *filename, const PPMImage &img)
{
    std::ofstream file(filename);
    file << "P3\n";
    file << img.x << " " << img.y << "\n";
    file << RGB_COMPONENT_COLOR << "\n";

    for (int i = 0; i < img.all; ++i)
    {
        file << img.data[i].red << " " << img.data[i].green << " " << img.data[i].blue;
        file << (((i + 1) % img.x == 0) ? "\n" : " ");
    }
}

void shiftRightOneColumn(PPMImage &img, PPMPixel *buffer)
{
#pragma omp parallel for
    for (int row = 0; row < img.y; ++row)
    {
        int base = row * img.x;
        buffer[row] = img.data[base + img.x - 1];
        for (int col = img.x - 1; col > 0; --col)
        {
            img.data[base + col] = img.data[base + col - 1];
        }
        img.data[base] = buffer[row];
    }
}

int main(int argc, char *argv[])
{
    int frames = 20;
    int save_each = 5;
    if (argc > 1)
    {
        frames = atoi(argv[1]);
    }
    if (argc > 2)
    {
        save_each = atoi(argv[2]);
    }
    if (save_each <= 0)
    {
        save_each = frames + 1;
    }

    PPMImage image;
    readPPM("car.ppm", image);
    PPMPixel *extra_column = new PPMPixel[image.y];

    double start = omp_get_wtime();
    int saved = 0;
    for (int frame = 1; frame <= frames; ++frame)
    {
        shiftRightOneColumn(image, extra_column);
        if (frame % save_each == 0 || frame == frames)
        {
            std::stringstream name;
            name << "car_frame_" << frame << ".ppm";
            writePPM(name.str().c_str(), image);
            ++saved;
        }
    }
    double finish = omp_get_wtime();

    std::cout << "frames = " << frames << std::endl;
    std::cout << "save_each = " << save_each << std::endl;
    std::cout << "saved = " << saved << std::endl;
    std::cout << "threads = " << omp_get_max_threads() << std::endl;
    std::cout << "time = " << finish - start << " seconds" << std::endl;

    delete[] extra_column;
    delete[] image.data;
    return 0;
}
