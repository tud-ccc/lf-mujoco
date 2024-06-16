#include "camera_library_includes.hpp"

float3 float3::operator*(float t) { return {x * t, y * t, z * t}; }

float3 float3::operator-(float t) { return {x - t, y - t, z - t}; }

void float3::operator*=(float t) {
  x = x * t;
  y = y * t;
  z = z * t;
}

void float3::operator=(float3 other) {
  x = other.x;
  y = other.y;
  z = other.z;
}

void float3::add(float t1, float t2, float t3) {
  x += t1;
  y += t2;
  z += t3;
}

// Create new rect within original boundaries with give aspect ration
rect rect::adjust_ratio(float2 size) const {
  auto H = static_cast<float>(h), W = static_cast<float>(h) * size.x / size.y;
  if (W > w) {
    auto scale = w / W;
    W *= scale;
    H *= scale;
  }

  return {x + (w - W) / 2, y + (h - H) / 2, W, H};
}

void text_renderer::put_text(const std::string& msg, float norm_x_pos, float norm_y_pos, const rect& r);
{
  set_viewport(r);
  draw_text(int(norm_x_pos * r.w), int(norm_y_pos * r.h), msg.c_str());
}
