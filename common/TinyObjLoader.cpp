#pragma once
#if !defined(TINYOBJLOADER_IMPLEMENTATION)
#define TINYOBJLOADER_IMPLEMENTATION
#endif

#include "../libs/tinyobjloader/tiny_obj_loader.h" 
#include "TinyObjLoader.h"
#include <string>
#include <vector>
#include <iostream>
#include "ObjData.h"
#include "../common/Texture.h"
#include "../common/GLObject.h"
#include "../libs/stb/stb_image.h"
#include "../common/BufferLayout.h"
#include "Vertex.h"

bool TinyObjLoader::ImportObject(const std::string& path, GLObject& object) {
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path)) { std::cerr << "Failed to read object path : " << path << std::endl; return false; }

    if (!reader.Warning().empty()) std::cout << "tinyobj warning : " << reader.Warning();

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
        Material m = { Vector3 (mat.ambient[0], mat.ambient[1], mat.ambient[2]), Vector3 ( mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] ), Vector3 ( mat.specular[0], mat.specular[1], mat.specular[2] ), mat.shininess };
        mats.push_back(m);
    }

    uint8_t cnt = 0;
    
    for (size_t i = 0; i < shapes.size(); i++) {
        
        size_t index_offset = 0;
        std::vector<Vertex> vertices;
        for (size_t j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++) {
            
            size_t fv = size_t(shapes[i].mesh.num_face_vertices[j]);
            if (fv != 3) return false;

            tinyobj::real_t vx = 0;
            tinyobj::real_t vy = 0;
            tinyobj::real_t vz = 0;
            tinyobj::real_t tx = 0;
            tinyobj::real_t ty = 0;
            tinyobj::real_t nx = 0;
            tinyobj::real_t ny = 0;
            tinyobj::real_t nz = 0;

            for (size_t k = 0; k < fv; k++) {
                
                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + k];

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
                vertices.push_back(Vertex{ {vx,vy,vz}, {tx,ty}, {nx, ny, nz} });
            }
			
            index_offset += fv;
        }
		int index = shapes[i].mesh.material_ids[0];
        std::string text_name = materials[index].diffuse_texname;

        (index >= 0) ? ((text_name != "") ? object.AddShape(vertices, layout, mats[index], materials[index].diffuse_texname) : object.AddShape(vertices, layout, mats[index])) : object.AddShape(vertices, layout);

        object.SetObjectName(shapes[i].name);
		
    }
    return true;
}
