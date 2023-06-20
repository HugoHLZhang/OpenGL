#pragma once
#if !defined(TINYOBJLOADER_IMPLEMENTATION)
#define TINYOBJLOADER_IMPLEMENTATION
#endif

#include "../libs/tinyobjloader/tiny_obj_loader.h" 
#include "Object_Loader.h"
#include <string>
#include <vector>
#include <iostream>
#include "Struct.h"
#include "../glutilities/Texture.h"
#include "../glutilities/GLObject.h"
#include "../libs/stb/stb_image.h"
#include "../glutilities/BufferLayout.h"
#include "Vector.h"

bool Object_Loader::ImportObj(const std::string& path, GLObject& object) {
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path)) {
        std::cerr << "Failed to parse " << path << std::endl;
        return false;
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    int index = path.find_last_of('.');

	std::string fileName = path.substr(0, index).substr(path.find_last_of('/') + 1);

    object.SetName(fileName);

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    BufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);

    std::vector<Material> mats;
    for (auto mat : materials) {
        Material m = {
            Vector3 (mat.ambient[0], mat.ambient[1], mat.ambient[2]),
            Vector3 ( mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] ),
			Vector3 ( mat.specular[0], mat.specular[1], mat.specular[2] ),
			mat.shininess
		};
        mats.push_back(m);
    }

    uint8_t count = 0;
    
    for (size_t s = 0; s < shapes.size(); s++) {
        
        size_t index_offset = 0;
        std::vector<Vertex> vertices;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            if (fv != 3) return false;

            

            tinyobj::real_t vx = 0;
            tinyobj::real_t vy = 0;
            tinyobj::real_t vz = 0;

            tinyobj::real_t tx = 0;
            tinyobj::real_t ty = 0;
			
            tinyobj::real_t nx = 0;
            tinyobj::real_t ny = 0;
            tinyobj::real_t nz = 0;


            for (size_t v = 0; v < fv; v++) {
                
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                
                if (idx.texcoord_index >= 0) {
                    tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
               
                if (idx.normal_index >= 0) {
                    nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }
                vertices.push_back(Vertex{
                    {vx,vy,vz},
                    {tx,ty},
                    {nx, ny, nz}
                    });
            }
			
            index_offset += fv;
        }
		int index = shapes[s].mesh.material_ids[0];
        if (index >= 0) {
            std::string text_name = materials[index].diffuse_texname;
            if(text_name != "")
                object.AddShape(vertices, layout, mats[index], materials[index].diffuse_texname);
            else
				object.AddShape(vertices, layout, mats[index]);
        }
        else
			object.AddShape(vertices, layout);
        object.SetObjectName(shapes[s].name);
		
    }
    return true;
}
