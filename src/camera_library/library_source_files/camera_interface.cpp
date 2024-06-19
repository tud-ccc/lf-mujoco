#ifndef PI
#define PI  3.14159265358979323846
#define PI_FL  3.141592f
#endif

#ifndef CAMERA_INTERFACE_HPP
#define CAMERA_INTERFACE_HPP

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLU

#include "../camera_library_includes.hpp"
#include <optional>

window init_camera_generate_window(rs2::pipeline& pipe, int stream_width, int stream_height)
 {

  std::string serial;
  if (!device_with_streams({RS2_STREAM_COLOR, RS2_STREAM_DEPTH}, serial)) {
    std::cout << "The device does not have stream !" << std::endl;
    assert(false && "No connection to camera establishes");
  }

  rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);
  // Create a simple OpenGL window for rendering:
  // Declare RealSense pipeline, encapsulating the actual device and sensors

  rs2::config cfg;
  if (!serial.empty())
    cfg.enable_device(serial);
  cfg.enable_stream(RS2_STREAM_DEPTH, stream_width, stream_height, RS2_FORMAT_Z16,
                    30); // Enable depth stream at 640x480 resolution, Z16 format, and 30 FPS    // For the color
                         // stream, set format to RGBA To allow blending of the color frame on top of the depth frame
  cfg.enable_stream(RS2_STREAM_COLOR, stream_width, stream_height, RS2_FORMAT_RGBA8,
                    30); // Enable color stream at 640x480 resolution, RGB8 format, and 30 FPS

  rs2::align align_to(RS2_STREAM_COLOR); // Align depth frame to color frame

  auto profile = pipe.start(cfg);

  auto stream = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();

  window app(stream.width(), stream.height(), "RealSense Measure Example");

  return app;
}

Vector receive_current_target_show(rs2::pipeline& pipe) {

  rs2::frameset current_frameset = pipe.wait_for_frames();

  auto color = current_frameset.get_color_frame();

  auto depth = current_frameset.get_depth_frame();

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  pixel center_blue = fetch_blue_center_pixel_wrapper(color);

  auto coordinate_blue_center = pixel_to_3d(depth, center_blue.first, center_blue.second);

  glColor3f(1.f, 1.f, 1.f);

  glDisable(GL_BLEND);
  
  return Vector{coordinate_blue_center.x ,coordinate_blue_center.y ,coordinate_blue_center.z};
}

Vector receive_current_target(rs2::pipeline& pipe) {

  rs2::frameset current_frameset = pipe.wait_for_frames();

  auto color = current_frameset.get_color_frame();

  auto depth = current_frameset.get_depth_frame();

  pixel center_blue = fetch_blue_center_pixel_wrapper(color);

  auto coordinate_blue_center = pixel_to_3d(depth, center_blue.first, center_blue.second);
  
  return Vector{coordinate_blue_center.x ,coordinate_blue_center.y ,coordinate_blue_center.z};
}

#endif // Interface