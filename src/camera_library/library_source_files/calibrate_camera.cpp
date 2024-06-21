
#include "../camera_library_includes.hpp"

void calibrate_camera(rs2::pipeline& pipe, int duration_seconds) {
    std::cout << "Calibrating camera for " << duration_seconds << " seconds..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time + std::chrono::seconds(duration_seconds);

    int total_frames = 0;
    long long total_r = 0, total_g = 0, total_b = 0;

    while (std::chrono::high_resolution_clock::now() < end_time) {
        // Wait for the next set of frames from the camera
        rs2::frameset frames = pipe.wait_for_frames();
        rs2::frame color_frame = frames.get_color_frame();

        // Get the width and height of the color frame
        int width = color_frame.as<rs2::video_frame>().get_width();
        int height = color_frame.as<rs2::video_frame>().get_height();

        // Get the central pixel coordinates
        int center_x = width / 2;
        int center_y = height / 2;

        // Get the RGB values of the central pixel
        const uint8_t* color_data = reinterpret_cast<const uint8_t*>(color_frame.get_data());
        int index = (center_y * width + center_x) * 3; // 3 channels: RGB

        uint8_t r = color_data[index];
        uint8_t g = color_data[index + 1];
        uint8_t b = color_data[index + 2];

        total_r += r;
        total_g += g;
        total_b += b;
        total_frames++;

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Add a small delay to reduce CPU usage
    }

    if (total_frames > 0) {
        uint8_t avg_r = total_r / total_frames;
        uint8_t avg_g = total_g / total_frames;
        uint8_t avg_b = total_b / total_frames;

        std::cout << "Average central pixel RGB values after " << duration_seconds << " seconds: R=" 
                  << static_cast<int>(avg_r) << " G=" << static_cast<int>(avg_g) 
                  << " B=" << static_cast<int>(avg_b) << std::endl;
    } else {
        std::cout << "No frames captured during calibration period." << std::endl;
    }
}

void print_3d_coords_in_camera_testings_and_transform(rs2::video_frame depth, std::optional<pixel> center_blue, std::optional<pixel> center_blue_by_fetch){
  if (center_blue.has_value() && center_blue_by_fetch.has_value()) {
      std::cout << "Center blue by render" << std::endl;
      auto coordiante_blue_center = pixel_to_3d(depth, center_blue.value().first, center_blue.value().second);
      print_vertex(coordiante_blue_center);
      std::cout << "Center blue by fetchreceive" << std::endl;
      auto coordiante_blue_center_rec =
          pixel_to_3d(depth, center_blue_by_fetch.value().first, center_blue_by_fetch.value().second);
      print_vertex(coordiante_blue_center_rec);
    } else {
      std::cout << "No object localized !" << std::endl;
    }

}
