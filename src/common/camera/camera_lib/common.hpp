// Copyright(c) 2015 Intel Corporation. All Rights Reserved.
#ifndef COMMON_WINDOW_HPP
#define COMMON_WINDOW_HPP


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

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "stb_easy_font.hpp"
#include "data_structures.hpp"

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

struct text_renderer
{
    // Provide textual representation only
    void put_text(const std::string &msg, float norm_x_pos, float norm_y_pos, const rect &r)
    {
        set_viewport(r);
        draw_text(int(norm_x_pos * r.w), int(norm_y_pos * r.h), msg.c_str());
    }
};

static void draw_circle(float xx, float xy, float xz, float yx, float yy, float yz, float radius = 1.1, float3 center = {0.0, 0.0, 0.0}, float intensity = 0.5f)
{
    const auto N = 50;
    glColor3f(intensity, intensity, intensity);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= N; i++)
    {
        const double theta = (2 * PI / N) * i;
        const auto cost = static_cast<float>(cos(theta));
        const auto sint = static_cast<float>(sin(theta));
        glVertex3f(
            center.x + radius * (xx * cost + yx * sint),
            center.y + radius * (xy * cost + yy * sint),
            center.z + radius * (xz * cost + yz * sint));
    }
    glEnd();
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