#ifndef TEXTURE_HPP
#define TEXTURE_HPP
class texture {
public:
  void upload(const rs2::video_frame& frame) {
    if (!frame)
      return;

    if (!_gl_handle)
      glGenTextures(1, &_gl_handle);
    GLenum err = glGetError();

    auto format = frame.get_profile().format();
    auto width = frame.get_width();
    auto height = frame.get_height();
    _stream_type = frame.get_profile().stream_type();
    _stream_index = frame.get_profile().stream_index();

    glBindTexture(GL_TEXTURE_2D, _gl_handle);

    switch (format) {
    case RS2_FORMAT_RGB8:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.get_data());
      break;
    case RS2_FORMAT_RGBA8:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame.get_data());
      break;
    case RS2_FORMAT_Y8:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame.get_data());
      break;
    case RS2_FORMAT_Y10BPACK:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, frame.get_data());
      break;
    default:
      throw std::runtime_error("The requested format is not supported by this demo!");
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void show(const rect& r, float alpha = 1.f) const {
    if (!_gl_handle)
      return;

    set_viewport(r);

    glBindTexture(GL_TEXTURE_2D, _gl_handle);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(0, 1);
    glVertex2f(0, r.h);
    glTexCoord2f(1, 1);
    glVertex2f(r.w, r.h);
    glTexCoord2f(1, 0);
    glVertex2f(r.w, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    draw_text(int(0.05f * r.w), int(0.05f * r.h), rs2_stream_to_string(_stream_type));
  }

  GLuint get_gl_handle() { return _gl_handle; }

  void render(const rs2::frame& frame, const rect& rect, float alpha = 1.f) {
    if (auto vf = frame.as<rs2::video_frame>()) {
      upload(vf);
      show(rect.adjust_ratio({(float)vf.get_width(), (float)vf.get_height()}), alpha);
    } else if (auto mf = frame.as<rs2::motion_frame>()) {
      _imu_render.render(frame, rect.adjust_ratio({IMU_FRAME_WIDTH, IMU_FRAME_HEIGHT}));
    } else if (auto pf = frame.as<rs2::pose_frame>()) {
      _pose_render.render(frame, rect.adjust_ratio({IMU_FRAME_WIDTH, IMU_FRAME_HEIGHT}));
    } else
      throw std::runtime_error("Rendering is currently supported for video, motion and pose frames only");
  }

private:
  GLuint _gl_handle = 0;
  rs2_stream _stream_type = RS2_STREAM_ANY;
  int _stream_index{};
  imu_renderer _imu_render;
  pose_renderer _pose_render;
};

class custom_benes_texture {

private:
  GLuint _gl_handle = 0;
  rs2_stream _stream_type = RS2_STREAM_ANY;
  int _stream_index{};

public:

  pixel analyze_frame_and_mark_blue_objects_and_pipe_to_frame_buffer(const rs2::video_frame& frame, int width, int height) {

    // Allocate a buffer for the marked image
    std::vector<uint8_t> marked_data(width * height * 4);
    std::vector<pixel> blue_pixels;
    int grap_range = 2;
    find_blue_pixels(frame, width, height, grap_range, marked_data, blue_pixels);

    // Function to find the largest cluster of blue pixels
    std::vector<pixel> largest_cluster = find_largest_blue_cluster(blue_pixels, grap_range);
    mark_blue_pixels(marked_data, largest_cluster, width);

    // Mark the largest cluster in green for visual debugging

    int threshold = 20;

    glBindTexture(GL_TEXTURE_2D, _gl_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, marked_data.data());
    pixel center_blue = calculate_center_wrapper(largest_cluster, threshold, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
    return center_blue;
  }

  pixel upload(const rs2::video_frame& frame) {
    if (!frame)
      return {std::make_pair(-1, -1)};

    if (!_gl_handle)
      glGenTextures(1, &_gl_handle);
    GLenum err = glGetError();

    auto format = frame.get_profile().format();
    auto width = frame.get_width();
    auto height = frame.get_height();

    _stream_type = frame.get_profile().stream_type();
    _stream_index = frame.get_profile().stream_index();

    glBindTexture(GL_TEXTURE_2D, _gl_handle);

    switch (format) {
    // case RS2_FORMAT_RGB8:
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.get_data());
    //     std::cout << "1" << std::endl;
    //     break;
    case RS2_FORMAT_RGBA8:
      // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame.get_data());
      break;
      // case RS2_FORMAT_Y8:
      //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame.get_data());
      //     std::cout<< "3" << std::endl;
      //     break;
      // case RS2_FORMAT_Y10BPACK:
      //     glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT,
      //     frame.get_data()); std::cout <<"4" << std::endl;
      break;
    default:
      throw std::runtime_error("The requested format is not supported by this demo!");
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    pixel center_blue =
        analyze_frame_and_mark_blue_objects_and_pipe_to_frame_buffer(frame, width, height); // Analyze and mark blue objects after texture upload

    glBindTexture(GL_TEXTURE_2D, 0);

    return center_blue;
  }

  void show(const rect& r, float alpha = 1.f) const {
    if (!_gl_handle)
      return;

    set_viewport(r);

    glBindTexture(GL_TEXTURE_2D, _gl_handle);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(0, 1);
    glVertex2f(0, r.h);
    glTexCoord2f(1, 1);
    glVertex2f(r.w, r.h);
    glTexCoord2f(1, 0);
    glVertex2f(r.w, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  GLuint get_gl_handle() { return _gl_handle; }

  pixel render(const rs2::frame& frame, const rect& rect, float alpha = 1.f) {
    if (auto vf = frame.as<rs2::video_frame>()) {
      pixel center_blue = upload(vf);
      show(rect.adjust_ratio({(float)vf.get_width(), (float)vf.get_height()}), alpha);
      return center_blue;
    } else
      throw std::runtime_error("Rendering is currently supported for video, motion and pose frames only");
  }
 };

#endif