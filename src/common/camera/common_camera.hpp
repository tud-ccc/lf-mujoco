#ifndef COMMON_VECTOR
#define COMMON_VECTOR

#include "common_texture.hpp" // Include short list of convenience functions for rendering
#include "example.hpp"
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

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
  std::cout << "3D coordinate: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")"
<< std::endl;
}

void init_camera(rs2::pipeline& pipe ) {

  std::string serial;
  if (!device_with_streams({RS2_STREAM_COLOR, RS2_STREAM_DEPTH}, serial))
    return EXIT_SUCCESS;

  rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);
  // Create a simple OpenGL window for rendering:
  // Declare RealSense pipeline, encapsulating the actual device and sensors  

  rs2::config cfg;
  if (!serial.empty())
    cfg.enable_device(serial);
  cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16,
                    30); // Enable depth stream at 640x480 resolution, Z16 format, and 30 FPS    // For the color
                         // stream, set format to RGBA To allow blending of the color frame on top of the depth frame
  cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGBA8,
                    30); // Enable color stream at 640x480 resolution, RGB8 format, and 30 FPS

  rs2::align align_to(RS2_STREAM_COLOR); // Align depth frame to color frame

  auto profile = pipe.start(cfg);

  auto stream = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();

  window app(stream.width(), stream.height(), "RealSense Measure Example");

}

void receive_current_target(rs2::pipeline& pipe, custom_benes_texture& color_image) {

  rs2::frameset current_frameset = pipe.wait_for_frames();

  auto color = current_frameset.get_color_frame();
  
  auto depth = current_frameset.get_depth_frame();
  
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  pixel center_blue = color_image.render(color, {0, 0, app.width(), app.height()});

  std::cout << center_blue.first << ", " << center_blue.second << std::endl;

  auto coordiante_blue_center = pixel_to_3d(depth, center_blue.first, center_blue.second);

  print_vertex(coordiante_blue_center);

  glColor3f(1.f, 1.f, 1.f);
 
  glDisable(GL_BLEND);

}

#endif // COMMON_VECTOR