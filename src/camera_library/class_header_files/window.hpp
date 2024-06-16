#ifndef WINDOW_HPP
#define WINDOW_HPP
class window
{
public:
    std::function<void(bool)>           on_left_mouse = [](bool) {};
    std::function<void(double, double)> on_mouse_scroll = [](double, double) {};
    std::function<void(double, double)> on_mouse_move = [](double, double) {};
    std::function<void(int)>            on_key_release = [](int) {};

    window(int width, int height, const char* title)
        : _width(width), _height(height), _canvas_left_top_x(0), _canvas_left_top_y(0), _canvas_width(width), _canvas_height(height)
    {
        glfwInit();
        win = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!win)
            throw std::runtime_error("Could not open OpenGL window, please check your graphic drivers or use the textual SDK tools");
        glfwMakeContextCurrent(win);

        glfwSetWindowUserPointer(win, this);
        glfwSetMouseButtonCallback(win, [](GLFWwindow* w, int button, int action, int mods)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                if (button == 0) s->on_left_mouse(action == GLFW_PRESS);
            });

        glfwSetScrollCallback(win, [](GLFWwindow* w, double xoffset, double yoffset)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                s->on_mouse_scroll(xoffset, yoffset);
            });

        glfwSetCursorPosCallback(win, [](GLFWwindow* w, double x, double y)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                s->on_mouse_move(x, y);
            });

        glfwSetKeyCallback(win, [](GLFWwindow* w, int key, int scancode, int action, int mods)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                if (0 == action) // on key release
                {
                    s->on_key_release(key);
                }
            });
    }

    //another c'tor for adjusting specific frames in specific tiles, this window is NOT resizeable
    window(unsigned width, unsigned height, const char* title, unsigned tiles_in_row, unsigned tiles_in_col, float canvas_width = 0.8f,
        float canvas_height = 0.6f, float canvas_left_top_x = 0.1f, float canvas_left_top_y = 0.075f)
        : _width(width), _height(height), _tiles_in_row(tiles_in_row), _tiles_in_col(tiles_in_col)

    {
        //user input verification for mosaic size, if invalid values were given - set to default
        if (canvas_width < 0 || canvas_width > 1 || canvas_height < 0 || canvas_height > 1 ||
            canvas_left_top_x < 0 || canvas_left_top_x > 1 || canvas_left_top_y < 0 || canvas_left_top_y > 1)
        {
            std::cout << "Invalid window's size parameter entered, setting to default values" << std::endl;
            canvas_width = 0.8f;
            canvas_height = 0.6f;
            canvas_left_top_x = 0.15f;
            canvas_left_top_y = 0.075f;
        }

        //user input verification for number of tiles in row and column
        if (_tiles_in_row <= 0) {
            _tiles_in_row = 4;
        }
        if (_tiles_in_col <= 0) {
            _tiles_in_col = 2;
        }

        //calculate canvas size
        _canvas_width = int(_width * canvas_width);
        _canvas_height = int(_height * canvas_height);
        _canvas_left_top_x = _width * canvas_left_top_x;
        _canvas_left_top_y = _height * canvas_left_top_y;

        //calculate tile size
        _tile_width_pixels = float(std::floor(_canvas_width / _tiles_in_row));
        _tile_height_pixels = float(std::floor(_canvas_height / _tiles_in_col));

        glfwInit();
        // we don't want to enable resizing the window
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        win = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!win)
            throw std::runtime_error("Could not open OpenGL window, please check your graphic drivers or use the textual SDK tools");
        glfwMakeContextCurrent(win);

        glfwSetWindowUserPointer(win, this);
        glfwSetMouseButtonCallback(win, [](GLFWwindow* w, int button, int action, int mods)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                if (button == 0) s->on_left_mouse(action == GLFW_PRESS);
            });

        glfwSetScrollCallback(win, [](GLFWwindow* w, double xoffset, double yoffset)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                s->on_mouse_scroll(xoffset, yoffset);
            });

        glfwSetCursorPosCallback(win, [](GLFWwindow* w, double x, double y)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                s->on_mouse_move(x, y);
            });

        glfwSetKeyCallback(win, [](GLFWwindow* w, int key, int scancode, int action, int mods)
            {
                auto s = (window*)glfwGetWindowUserPointer(w);
                if (0 == action) // on key release
                {
                    s->on_key_release(key);
                }
            });
    }

    ~window()
    {
        glfwDestroyWindow(win);
        glfwTerminate();
    }

    void close()
    {
        glfwSetWindowShouldClose(win, 1);
    }

    float width() const { return float(_width); }
    float height() const { return float(_height); }

    operator bool()
    {
        glPopMatrix();
        glfwSwapBuffers(win);

        auto res = !glfwWindowShouldClose(win);

        glfwPollEvents();
        glfwGetFramebufferSize(win, &_width, &_height);

        // Clear the framebuffer
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, _width, _height);

        // Draw the images
        glPushMatrix();
        glfwGetWindowSize(win, &_width, &_height);
        glOrtho(0, _width, _height, 0, -1, +1);

        return res;
    }

    void show(rs2::frame frame)
    {
        show(frame, { 0, 0, (float)_width, (float)_height });
    }

    void show(const rs2::frame& frame, const rect& rect)
    {
        if (auto fs = frame.as<rs2::frameset>())
            render_frameset(fs, rect);
        else if (auto vf = frame.as<rs2::depth_frame>())
            render_video_frame(vf.apply_filter(_colorizer), rect);
        else if (auto vf = frame.as<rs2::video_frame>())
            render_video_frame(vf, rect);
        else if (auto mf = frame.as<rs2::motion_frame>())
            render_motion_frame(mf, rect);
        else if (auto pf = frame.as<rs2::pose_frame>())
            render_pose_frame(pf, rect);
    }

    void show(const std::map<int, rs2::frame>& frames)
    {
        // Render openGl mosaic of frames
        if (frames.size())
        {
            int cols = int(std::ceil(std::sqrt(frames.size())));
            int rows = int(std::ceil(frames.size() / static_cast<float>(cols)));

            float view_width = float(_width / cols);
            float view_height = float(_height / rows);
            unsigned int stream_no = 0;
            for (auto& frame : frames)
            {
                rect viewport_loc{ view_width * (stream_no % cols), view_height * (stream_no / cols), view_width, view_height };
                show(frame.second, viewport_loc);
                stream_no++;
            }
        }
        else
        {
            _main_win.put_text("Connect one or more Intel RealSense devices and rerun the example",
                0.4f, 0.5f, { 0.f,0.f, float(_width) , float(_height) });
        }
    }

    //gets as argument a map of the -need to be drawn- frames with their tiles properties,
    //which indicates where and what size should the frame be drawn on the canvas 
    void show(const frames_mosaic& frames)
    {
        // Render openGl mosaic of frames
        if (frames.size())
        {
            // create vector of frames from map, and sort it by priority
            std::vector <frame_and_tile_property> vector_frames;
            //copy: map (values) -> vector
            for (const auto& frame : frames) { vector_frames.push_back(frame.second); }
            //sort in ascending order of the priority
            std::sort(vector_frames.begin(), vector_frames.end(),
                [](const frame_and_tile_property& frame1, const frame_and_tile_property& frame2)
                {
                    return frame1.second.priority < frame2.second.priority;
                });
            //create margin to the shown frame on tile
            float frame_width_size_from_tile_width = 1.0f;
            //iterate over frames in ascending priority order (so that lower priority frame is drawn first, and can be over-written by higher priority frame )
            for (const auto& frame : vector_frames)
            {
                tile_properties attr = frame.second;
                rect viewport_loc{ _tile_width_pixels * attr.x + _canvas_left_top_x, _tile_height_pixels * attr.y + _canvas_left_top_y,
                    _tile_width_pixels * attr.w * frame_width_size_from_tile_width, _tile_height_pixels * attr.h };
                show(frame.first, viewport_loc);
            }
        }
        else
        {
            _main_win.put_text("Connect one or more Intel RealSense devices and rerun the example",
                0.3f, 0.5f, { float(_canvas_left_top_x), float(_canvas_left_top_y), float(_canvas_width) , float(_canvas_height) });
        }
    }

    /////////////////////////////////////////////////////////////
    //     get_pos_on_current_image:
    // There may be several windows displayed on the sceen, as described in the frames_mosaic structure.
    // The windows are displayed in a reduced resolution, appropriate the amount of space allocated for them on the screen.
    // This function converts from screen pixel to original image pixel.
    // 
    // Input:
    // pos - pixel in screen coordinates.
    // frames - structure of separate windows displayed on screen.
    // Returns: 
    // The index of the window the screen pixel is in and the pixel in that window in the original window's resolution.
    frame_pixel get_pos_on_current_image(float2 pos, const frames_mosaic& frames)
    {
        frame_pixel res{ -1, -1,-1 };
        for (auto& frame : frames)
        {
            if (auto vf = frame.second.first.as<rs2::video_frame>())
            {
                tile_properties attr = frame.second.second;
                float frame_width_size_from_tile_width = 1.0f;
                rect viewport_loc{ _tile_width_pixels * attr.x + _canvas_left_top_x, _tile_height_pixels * attr.y + _canvas_left_top_y,
                    _tile_width_pixels * attr.w * frame_width_size_from_tile_width, _tile_height_pixels * attr.h };
                viewport_loc = viewport_loc.adjust_ratio({ (float)vf.get_width(), (float)vf.get_height() });
                if (pos.x >= viewport_loc.x && pos.x < viewport_loc.x + viewport_loc.w &&
                    pos.y >= _height - (viewport_loc.y + viewport_loc.h) && pos.y < _height - viewport_loc.y)
                {
                    float image_rect_ratio = (float)vf.get_width() / viewport_loc.w;   //Ratio for y-axis is the same.
                    res.frame_idx = frame.first;
                    res.pixel.x = (pos.x - viewport_loc.x) * image_rect_ratio;
                    res.pixel.y = (pos.y - (_height - (viewport_loc.y + viewport_loc.h))) * image_rect_ratio;
                    break;
                }
            }
        }

        return res;
    }

    operator GLFWwindow* () { return win; }

private:
    GLFWwindow* win;
    std::map<int, texture> _textures;
    std::map<int, imu_renderer> _imus;
    std::map<int, pose_renderer> _poses;
    text_renderer _main_win;
    int _width, _height;
    float _canvas_left_top_x, _canvas_left_top_y;
    int _canvas_width, _canvas_height;
    unsigned _tiles_in_row, _tiles_in_col;
    float _tile_width_pixels, _tile_height_pixels;
    rs2::colorizer _colorizer;

    void render_video_frame(const rs2::video_frame& f, const rect& r)
    {
        auto& t = _textures[f.get_profile().unique_id()];
        t.render(f, r);
    }

    void render_motion_frame(const rs2::motion_frame& f, const rect& r)
    {
        auto& i = _imus[f.get_profile().unique_id()];
        i.render(f, r);
    }

    void render_pose_frame(const rs2::pose_frame& f, const rect& r)
    {
        auto& i = _poses[f.get_profile().unique_id()];
        i.render(f, r);
    }

    void render_frameset(const rs2::frameset& frames, const rect& r)
    {
        std::vector<rs2::frame> supported_frames;
        for (auto f : frames)
        {
            if (can_render(f))
                supported_frames.push_back(f);
        }
        if (supported_frames.empty())
            return;

        std::sort(supported_frames.begin(), supported_frames.end(), [](rs2::frame first, rs2::frame second)
            { return first.get_profile().stream_type() < second.get_profile().stream_type();  });

        auto image_grid = calc_grid(r, supported_frames);

        int image_index = 0;
        for (auto f : supported_frames)
        {
            auto r = image_grid.at(image_index);
            show(f, r);
            image_index++;
        }
    }

    bool can_render(const rs2::frame& f) const
    {
        auto format = f.get_profile().format();
        switch (format)
        {
        case RS2_FORMAT_RGB8:
        case RS2_FORMAT_RGBA8:
        case RS2_FORMAT_Y8:
        case RS2_FORMAT_MOTION_XYZ32F:
        case RS2_FORMAT_Y10BPACK:
            return true;
        default:
            return false;
        }
    }

    rect calc_grid(rect r, size_t streams)
    {
        if (r.w <= 0 || r.h <= 0 || streams <= 0)
            throw std::runtime_error("invalid window configuration request, failed to calculate window grid");
        float ratio = r.w / r.h;
        auto x = sqrt(ratio * (float)streams);
        auto y = (float)streams / x;
        auto w = round(x);
        auto h = round(y);
        if (w == 0 || h == 0)
            throw std::runtime_error("invalid window configuration request, failed to calculate window grid");
        while (w * h > streams)
            h > w ? h-- : w--;
        while (w* h < streams)
            h > w ? w++ : h++;
        auto new_w = round(r.w / w);
        auto new_h = round(r.h / h);
        // column count, line count, cell width cell height
        return rect{ static_cast<float>(w), static_cast<float>(h), static_cast<float>(new_w), static_cast<float>(new_h) };
    }

    std::vector<rect> calc_grid(rect r, std::vector<rs2::frame>& frames)
    {
        auto grid = calc_grid(r, frames.size());

        std::vector<rect> rv;
        int curr_line = -1;

        for (int i = 0; i < frames.size(); i++)
        {
            auto mod = i % (int)grid.x;
            float fw = IMU_FRAME_WIDTH;
            float fh = IMU_FRAME_HEIGHT;
            if (auto vf = frames[i].as<rs2::video_frame>())
            {
                fw = (float)vf.get_width();
                fh = (float)vf.get_height();
            }
            float cell_x_postion = (float)(mod * grid.w);
            if (mod == 0) curr_line++;
            float cell_y_position = curr_line * grid.h;
            float2 margin = { grid.w * 0.02f, grid.h * 0.02f };
            auto r = rect{ cell_x_postion + margin.x, cell_y_position + margin.y, grid.w - 2 * margin.x, grid.h };
            rv.push_back(r.adjust_ratio(float2{ fw, fh }));
        }

        return rv;
    }
};

#endif