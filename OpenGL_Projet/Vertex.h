#pragma once

#include <cstdint>


struct vect2
{
    float x; // offset (ou adresse relative) = 0
    float y; // offset = sizeof(x) = 4
};

struct vect3
{
    float x; // offset = 0
    float y; // offset = sizeof(x) = 4
    float z; // offset = sizeof(x) + sizeof(y) = 8
};

using Color2 = vect3; // equivalent de typedef vec3 color;

struct Vertex2
{
    vect3 position;
    Color2 color; // todo, vec3 normal
    vect2 uv;
};
