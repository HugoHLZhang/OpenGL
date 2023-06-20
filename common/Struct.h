#pragma once

#include <cstdint>
#include "../common/Vector.h"

struct vec2 {
    float x, y;
};

struct vec3 {
    float x, y, z;
};

struct Color {
    uint8_t r, g, b, a;
};

struct Material
{
    Vector3 ambient; // Ka
    Vector3 diffuse; // Kd
    Vector3 specular; // Ks
	float specularExponent; // Ns
};

struct Vertex
{
    vec3 position;
    vec2 uv;
    vec3 normal;
};