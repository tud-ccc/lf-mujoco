#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP


const float IMU_FRAME_WIDTH = 1280.f;
const float IMU_FRAME_HEIGHT = 720.f;
#ifndef PI
#define PI  3.14159265358979323846
#define PI_FL  3.141592f
#endif

using pixel = std::pair<int, int>;


struct float3 {
    float x, y, z;
    float3 operator*(float t)
    {
        return { x * t, y * t, z * t };
    }

    float3 operator-(float t)
    {
        return { x - t, y - t, z - t };
    }

    void operator*=(float t)
    {
        x = x * t;
        y = y * t;
        z = z * t;
    }

    void operator=(float3 other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    void add(float t1, float t2, float t3)
    {
        x += t1;
        y += t2;
        z += t3;
    }
};


struct float2 { float x, y; };
struct rect
{
    float x, y;
    float w, h;

    // Create new rect within original boundaries with give aspect ration
    rect adjust_ratio(float2 size) const
    {
        auto H = static_cast<float>(h), W = static_cast<float>(h) * size.x / size.y;
        if (W > w)
        {
            auto scale = w / W;
            W *= scale;
            H *= scale;
        }

        return{ x + (w - W) / 2, y + (h - H) / 2, W, H };
    }
};

enum class Priority { high = 0, medium = -1, low = -2 };

struct tile_properties
{
   
    unsigned int x, y; //location of tile in the grid
    unsigned int w, h; //width and height by number of tiles
    Priority priority; //when should the tile be drawn?: high priority is on top of all, medium is a layer under top layer, low is a layer under medium layer

};

using frame_and_tile_property = std::pair<rs2::frame, tile_properties>;
using frames_mosaic = std::map<int, frame_and_tile_property>;

struct frame_pixel
{
    int frame_idx;
    float2 pixel;
};

int round_float_to_int(float x)
{
    x = x + 0.5 - (x < 0);
    int y = (int)x;
    return y;
}



#endif