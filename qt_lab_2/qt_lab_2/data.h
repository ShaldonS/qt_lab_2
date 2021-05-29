#pragma once
#pragma comment (lib, "opengl32.lib")
#include <QApplication>
#include <QGLWidget>
#include <QGLFunctions>
#include <QDebug>
#include <QKeyEvent>
#include <string>
#include <fstream>
#include <windows.h>

class Data //read from bin file
{
    std::string path;
    int width, height, depth;
    short* data, min, max;
    float x_voxel_size, y_voxel_size, z_voxel_size;
public:
    Data() : width(0), height(0), depth(0), min(0), max(0) {};

    Data(std::string filename)
    {
        std::fstream input(filename, std::ios::in);

        input >> width >> height >> depth;
        input >> x_voxel_size >> y_voxel_size >> z_voxel_size;

        data = new short[width * height * depth];
        min = SHRT_MAX; max = SHRT_MIN;

        for (int i = 0; i < width * height * depth; i++)
        {
            input >> data[i];
            if (data[i] < min) min = data[i];
            if (data[i] > max) max = data[i];
        }
        path = filename;
    }

    std::string getPath()
    {
        return path;
    }

    void clear()
    {
        delete[] data;
    }
    short getMin() { return min; } //– вернуть минимальное значение плотности, встречающейся в данных
    short getMax() { return max; } //– вернуть максимальное значение плотности, встречающейся в данных
    void SetMin(short val) { min = val; }
    void SetMax(short val) { max = val; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getDepth() { return depth; }
    void setDepth(int num) { depth = num; }

    void readFile(std::string filename) //– прочитать файл и вычислить минимум и максимум
    {
        std::ifstream input(filename, std::ios::binary);

        input.read((char*)&width, sizeof(int));
        input.read((char*)&height, sizeof(int));
        input.read((char*)&depth, sizeof(int));

        input.read((char*)&x_voxel_size, sizeof(float));
        input.read((char*)&y_voxel_size, sizeof(float));
        input.read((char*)&z_voxel_size, sizeof(float));

        data = new short[width * height * depth];
        min = SHRT_MAX; max = SHRT_MIN;

        for (int i = 0; i < width * height * depth; i++)
        {
            input.read((char*)(data + i), sizeof(short));
            if (data[i] < min) min = data[i];
            if (data[i] > max) max = data[i];
        }
        path = filename;
    }
    int operator[] (int num) { return data[num]; }
};


