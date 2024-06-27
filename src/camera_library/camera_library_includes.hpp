#ifndef PI
#define PI 3.14159265358979323846
#define PI_FL 3.141592f
#endif

#ifndef CAMERA_INTERFACE_HPP_INCL
#define CAMERA_INTERFACE_HPP_INCL

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLU

#include "common_vector.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <map>
#include <sstream>
#include <string>
#include <optional>
#include <chrono>               // Include for timing functions
#include <thread>               // Include for sleep functions


#include "header_files/stb_easy_font.hpp"
// consts
const float IMU_FRAME_WIDTH = 1280.f;
const float IMU_FRAME_HEIGHT = 720.f;

const int GLOBAL_RED_THRESHOLD_GT = 140;
const int GLOBAL_RED_THRESHOLD_LT = 255;

const int GLOBAL_GREEN_THRESHOLD_GT = 120;
const int GLOBAL_GREEN_THRESHOLD_LT = 255;

const int GLOBAL_BLUE_THRESHOLD_GT = 0;
const int GLOBAL_BLUE_THRESHOLD_LT = 100;

// usings
using pixel = std::pair<int, int>;

// structs
struct float3 {
  float x, y, z;
  float3 operator*(float t);
  float3 operator-(float t);
  void operator*=(float t);
  void operator=(float3 other);
  void add(float t1, float t2, float t3);
};

struct float2 {
  float x, y;
};
struct rect {
  float x, y;
  float w, h;
  rect adjust_ratio(float2 size) const;
};

enum class Priority { high = 0, medium = -1, low = -2 };

struct tile_properties {
  unsigned int x, y; // location of tile in the grid
  unsigned int w, h; // width and height by number of tiles
  Priority priority; // when should the tile be drawn?: high priority is on top of all, medium is a layer under top
                     // layer, low is a layer under medium layer
};
struct frame_pixel {
  int frame_idx;
  float2 pixel;
};

int round_float_to_int(float x);
void draw_text(int x, int y, const char* text);
void set_viewport(const rect& r);

struct text_renderer {
  void put_text(const std::string& msg, float norm_x_pos, float norm_y_pos, const rect& r) {
    set_viewport(r);
    draw_text(int(norm_x_pos * r.w), int(norm_y_pos * r.h), msg.c_str());
  }
};

// blue_center
rs2::vertex pixel_to_3d(const rs2::depth_frame& depth_frame, int x, int y);
void print_vertex(const rs2::vertex& vertex);
std::optional<pixel> calculate_center(std::vector<pixel>& pixels, int bound_x, int bound_y);
std::vector<pixel> find_largest_blue_cluster(const std::vector<pixel>& blue_pixels, int grap_range);
std::optional<pixel> calculate_center_wrapper(std::vector<pixel>& largest_cluster, int threshold_accept_cluster, int bound_x,
                               int bound_y);
// common
bool device_with_streams(std::vector<rs2_stream> stream_requests, std::string& out_serial);

using frame_and_tile_property = std::pair<rs2::frame, tile_properties>;
using frames_mosaic = std::map<int, frame_and_tile_property>;
int mark_blue_pixels(std::vector<uint8_t>& marked_data, std::vector<pixel>& largest_cluster, int width);
int find_blue_pixels(const rs2::video_frame& frame, int width, int height, int grap_range,
                     std::vector<uint8_t>& marked_data, std::vector<pixel>& blue_pixels);
std::optional<pixel> fetch_blue_center_pixel_wrapper(const rs2::frame& frame);
std::optional<pixel> fetch_blue_center_pixel(const rs2::video_frame& frame, int width, int height);
#include "class_header_files/renderer.hpp"
#include "class_header_files/texture.hpp"
#include "class_header_files/window.hpp"
#include "class_header_files/coordinate_transformation.hpp"

// interface
window init_camera_generate_window(rs2::pipeline& pipe, int stream_width, int stream_height);
std::optional<Vector> receive_current_target(rs2::pipeline& pipe);
std::optional<Vector> sanity_check_position_by_camera(Vector vec);
void calibrate_camera(rs2::pipeline& pipe, int duration_seconds);
void print_3d_coords_in_camera_testings_and_transform(rs2::video_frame depth, std::optional<pixel> center_blue, std::optional<pixel> center_blue_by_fetch);


#endif