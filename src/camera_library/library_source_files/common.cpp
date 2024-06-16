// Copyright(c) 2015 Intel Corporation. All Rights Reserved.
#ifndef COMMON_WINDOW_HPP
#define COMMON_WINDOW_HPP
#include "../camera_library_includes.hpp"


inline void draw_text(int x, int y, const char *text)
{
    std::vector<char> buffer;
    buffer.resize(60000); // ~300 chars
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, &(buffer[0]));
    glDrawArrays(GL_QUADS,
                 0,
                 4 * stb_easy_font_print((float)x,
                                         (float)(y - 7),
                                         (char *)text,
                                         nullptr,
                                         &(buffer[0]),
                                         int(sizeof(char) * buffer.size())));
    glDisableClientState(GL_VERTEX_ARRAY);
}

void set_viewport(const rect &r)
{
    glViewport((int)r.x, (int)r.y, (int)r.w, (int)r.h);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, r.w, r.h, 0, -1, +1);
}

bool device_with_streams(std::vector<rs2_stream> stream_requests, std::string &out_serial)
{
    rs2::context ctx;
    auto devs = ctx.query_devices();
    std::vector<rs2_stream> unavailable_streams = stream_requests;
    for (auto dev : devs)
    {
        std::map<rs2_stream, bool> found_streams;
        for (auto &type : stream_requests)
        {
            found_streams[type] = false;
            for (auto &sensor : dev.query_sensors())
            {
                for (auto &profile : sensor.get_stream_profiles())
                {
                    if (profile.stream_type() == type)
                    {
                        found_streams[type] = true;
                        unavailable_streams.erase(std::remove(unavailable_streams.begin(), unavailable_streams.end(), type), unavailable_streams.end());
                        if (dev.supports(RS2_CAMERA_INFO_SERIAL_NUMBER))
                            out_serial = dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
                    }
                }
            }
        }
        // Check if all streams are found in current device
        bool found_all_streams = true;
        for (auto &stream : found_streams)
        {
            if (!stream.second)
            {
                found_all_streams = false;
                break;
            }
        }
        if (found_all_streams)
            return true;
    }
    // After scanning all devices, not all requested streams were found
    for (auto &type : unavailable_streams)
    {
        switch (type)
        {
        case RS2_STREAM_POSE:
        case RS2_STREAM_FISHEYE:
            std::cerr << "Connect T26X and rerun the demo" << std::endl;
            break;
        case RS2_STREAM_DEPTH:
            std::cerr << "The demo requires Realsense camera with DEPTH sensor" << std::endl;
            break;
        case RS2_STREAM_COLOR:
            std::cerr << "The demo requires Realsense camera with RGB sensor" << std::endl;
            break;
        default:
            throw std::runtime_error("The requested stream: " + std::to_string(type) + ", for the demo is not supported by connected devices!"); // stream type
        }
    }
    return false;
}

#endif