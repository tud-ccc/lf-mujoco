#ifndef COMMON_BLUE_CENTER_HPP
#define COMMON_BLUE_CENTER_HPP

#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>
#include <functional>
#include <queue>
#include "data_structures.hpp"



rs2::vertex pixel_to_3d(const rs2::depth_frame &depth_frame, int x, int y)
{
    // Get the depth value at the specified pixel
    if (x < 0 || y < 0)
    {
        return rs2::vertex{};
    }
    float depth = depth_frame.get_distance(x, y);
    if (depth == 0)
    {
        std::cerr << "No depth data at this pixel" << std::endl;
        return rs2::vertex{};
    }

    rs2_intrinsics intrinsics = depth_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data

    // Convert from pixel coordinates to normalized coordinates ([-1, 1] range)
    float pixel[2] = {static_cast<float>(x), static_cast<float>(y)};
    float point[3];

    // Use RealSense SDK's deproject function
    rs2_deproject_pixel_to_point(point, &intrinsics, pixel, depth);

    // Return the 3D coordinate
    return rs2::vertex{point[0], point[1], point[2]};
}

void print_vertex(const rs2::vertex &vertex)
{
    std::cout << "3D coordinate: ("
              << vertex.x << ", "
              << vertex.y << ", "
              << vertex.z << ")"
              << std::endl;
}

pixel calculate_center(std::vector<pixel> &pixels, int bound_x, int bound_y)
{
    if (pixels.empty())
        return {-1, -1};

    long double sum_x = 0;
    long double sum_y = 0;

    for (auto &pixel : pixels)
    {
        if (pixel.first < 0 || pixel.first > bound_x || pixel.second < 0 || pixel.second > bound_y)
        {
            std::cout << pixel.first << ", " << pixel.second << std::endl;
            std::cout << "Bounds: " << bound_x << ", " << bound_y << std::endl;
            continue;
        }

        sum_x += pixel.first;
        sum_y += pixel.second;
    }

    std::cout << "Sum x:" << sum_x << "Sum y: " << sum_y << std::endl;

    return std::make_pair(round_float_to_int(sum_x / pixels.size()), round_float_to_int(sum_y / pixels.size()));
}

std::vector<pixel> find_largest_blue_cluster(const std::vector<pixel> &blue_pixels, int grap_range)
{
    if (blue_pixels.empty())
        return {};

    std::vector<std::vector<int>> directions = {{0, grap_range}, {grap_range, 0}, {0, -grap_range}, {-grap_range, 0}};
    std::map<pixel, bool> visited;
    std::vector<pixel> largest_cluster;

    for (const auto &start : blue_pixels)
    {
        if (visited[start])
            continue;

        std::vector<pixel> current_cluster;
        std::queue<pixel> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty())
        {
            auto p = q.front();
            q.pop();
            current_cluster.push_back(p);

            for (const auto &d : directions)
            {
                pixel neighbor = {p.first + d[0], p.second + d[1]};
                if (std::find(blue_pixels.begin(), blue_pixels.end(), neighbor) != blue_pixels.end() && !visited[neighbor])
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        if (current_cluster.size() > largest_cluster.size())
        {
            largest_cluster = current_cluster;
        }
    }

    return largest_cluster;
}

pixel calculate_center_wrapper(std::vector<pixel> &largest_cluster, int threshold_accept_cluster, int bound_x, int bound_y)
{
    if (largest_cluster.size() >= threshold_accept_cluster)
    {
        std::cout << "Size of largest cluster: " << largest_cluster.size() << std::endl;
        pixel center_blue = calculate_center(largest_cluster, bound_x, bound_y);
        if (center_blue.first < 0 || center_blue.first > bound_x || center_blue.second < 0 || center_blue.second > bound_y)
        {
            std::cout << center_blue.first << ", " << center_blue.second << std::endl;
            std::cout << "Bounds: " << bound_x << ", " << bound_y << std::endl;
            throw std::runtime_error("The cluster center is out of bounds !");
        }
        return center_blue;
    }
    else
    {
        std::cout << "Only found single points in the picture, not large enough to construct a cluster !" << std::endl;
        return {std::make_pair(-1, -1)};
    }
}

#endif