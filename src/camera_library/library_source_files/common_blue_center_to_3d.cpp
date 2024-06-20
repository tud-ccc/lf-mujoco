#ifndef COMMON_BLUE_CENTER_HPP
#define COMMON_BLUE_CENTER_HPP

#include "../camera_library_includes.hpp"
#include <queue>

int find_blue_pixels(const rs2::video_frame& frame, int width, int height, int grap_range,
                     std::vector<uint8_t>& marked_data, std::vector<pixel>& blue_pixels) {
  const uint8_t* data = (const uint8_t*)frame.get_data();

  for (int y = 0; y < height; y += grap_range) {
    for (int x = 0; x < width; x += grap_range) {
      int index = (y * width + x) * 4;
      uint8_t r = data[index];
      uint8_t g = data[index + 1];
      uint8_t b = data[index + 2];
      uint8_t a = data[index + 3];

      if (b > 65 && g < 30 && r < 20) // A simple threshold to detect blue objects
      {
        // Mark the pixel in red for visual debugging
        marked_data[index] = 255;
        marked_data[index + 1] = 0;
        marked_data[index + 2] = 0;
        marked_data[index + 3] = a;
        blue_pixels.push_back(std::make_pair(x, y));
      } else {
        // Copy the original pixel data
        marked_data[index] = r;
        marked_data[index + 1] = g;
        marked_data[index + 2] = b;
        marked_data[index + 3] = a;
      }
    }
  }
  return 1;
}

int mark_blue_pixels(std::vector<uint8_t>& marked_data, std::vector<pixel>& largest_cluster, int width) {
  for (const auto& p : largest_cluster) {
    int index = (p.second * width + p.first) * 4;
    marked_data[index] = 0;
    marked_data[index + 1] = 255;
    marked_data[index + 2] = 0;
    marked_data[index + 3] = 255;
  }
  return 1;
}

int round_float_to_int(float x) {
  x = x + 0.5 - (x < 0);
  int y = (int)x;
  return y;
}

rs2::vertex pixel_to_3d(const rs2::depth_frame& depth_frame, int x, int y) {
  // Get the depth value at the specified pixel
  if (x < 0 || y < 0) {
    return rs2::vertex{};
  }
  float depth = depth_frame.get_distance(x, y);
  if (depth == 0) {
    std::cerr << "No depth data at this pixel" << std::endl;
    return rs2::vertex{};
  }

  rs2_intrinsics intrinsics =
      depth_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data

  // Convert from pixel coordinates to normalized coordinates ([-1, 1] range)
  float pixel[2] = {static_cast<float>(x), static_cast<float>(y)};
  float point[3];

  // Use RealSense SDK's deproject function
  rs2_deproject_pixel_to_point(point, &intrinsics, pixel, depth);

  // Return the 3D coordinate
  return rs2::vertex{point[0], point[1], point[2]};
}

void print_vertex(const rs2::vertex& vertex) {
  std::cout << "3D coordinate: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
}

std::optional<pixel> calculate_center(std::vector<pixel>& pixels, int bound_x, int bound_y) {
  if (pixels.empty())
    return {};

  long double sum_x = 0;
  long double sum_y = 0;

  for (auto& pixel : pixels) {
    if (pixel.first < 0 || pixel.first > bound_x || pixel.second < 0 || pixel.second > bound_y) {
      std::cout << pixel.first << ", " << pixel.second << std::endl;
      std::cout << "Bounds: " << bound_x << ", " << bound_y << std::endl;
      continue;
    }

    sum_x += pixel.first;
    sum_y += pixel.second;
  }

  return std::make_pair(round_float_to_int(sum_x / pixels.size()), round_float_to_int(sum_y / pixels.size()));
}

std::vector<pixel> find_largest_blue_cluster(const std::vector<pixel>& blue_pixels, int grap_range) {
  if (blue_pixels.empty())
    return {};

  std::vector<std::vector<int>> directions = {{0, grap_range}, {grap_range, 0}, {0, -grap_range}, {-grap_range, 0}};
  std::map<pixel, bool> visited;
  std::vector<pixel> largest_cluster;

  for (const auto& start : blue_pixels) {
    if (visited[start])
      continue;

    std::vector<pixel> current_cluster;
    std::queue<pixel> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
      auto p = q.front();
      q.pop();
      current_cluster.push_back(p);

      for (const auto& d : directions) {
        pixel neighbor = {p.first + d[0], p.second + d[1]};
        if (std::find(blue_pixels.begin(), blue_pixels.end(), neighbor) != blue_pixels.end() && !visited[neighbor]) {
          visited[neighbor] = true;
          q.push(neighbor);
        }
      }
    }

    if (current_cluster.size() > largest_cluster.size()) {
      largest_cluster = current_cluster;
    }
  }

  return largest_cluster;
}

std::optional<pixel> calculate_center_wrapper(std::vector<pixel>& largest_cluster, int threshold_accept_cluster, int bound_x,
                               int bound_y) {
  if (largest_cluster.size() >= threshold_accept_cluster) {
    std::cout << "Size of largest cluster: " << largest_cluster.size() << std::endl;
    std::optional<pixel> center_blue = calculate_center(largest_cluster, bound_x, bound_y);
    if (center_blue.value().first < 0 || center_blue.value().first > bound_x || center_blue.value().second < 0 ||
        center_blue.value().second > bound_y) {
      std::cout << center_blue.value().first << ", " << center_blue.value().second << std::endl;
      std::cout << "Bounds: " << bound_x << ", " << bound_y << std::endl;
      throw std::runtime_error("The cluster center is out of bounds !");
    }
    return center_blue;
  } else {
    std::cout << "Only found single points in the picture, not large enough to construct a cluster !" << std::endl;
    return {};
  }
}

std::optional<pixel> fetch_blue_center_pixel_wrapper(const rs2::frame& frame) {
  if (auto video_frame = frame.as<rs2::video_frame>()) {
    auto width = video_frame.get_width();
    auto height = video_frame.get_height();
    return fetch_blue_center_pixel(video_frame, width, height);
  } else
    throw std::runtime_error("Rendering is currently supported for video, motion and pose frames only");
}
std::optional<pixel> fetch_blue_center_pixel(const rs2::video_frame& frame, int width, int height) {
  // Allocate buffer
  std::vector<uint8_t> marked_data(width * height * 4);
  std::vector<pixel> blue_pixels;
  int grap_range = 5;  

  find_blue_pixels(frame, width, height, grap_range, marked_data, blue_pixels);
  std::vector<pixel> largest_cluster = find_largest_blue_cluster(blue_pixels, grap_range);
  int threshold = 10;
  return calculate_center_wrapper(largest_cluster, threshold, width, height);
 
}

#endif