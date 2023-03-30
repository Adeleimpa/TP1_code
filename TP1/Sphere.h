//
// Created by Adèle Imparato on 16/03/2023.
//

#ifndef TP1_CODE_SPHERE_H
#define TP1_CODE_SPHERE_H

#include "MeshObject.h"

class Sphere : public MeshObject {
public:
    glm::vec3 m_center;
    float m_radius;

    // default resolution
    unsigned int resolution = 30;

    // resolutions depending on distance from camera
    const unsigned int front_resolution = 40;
    const unsigned int middle_resolution = 20;
    const unsigned int back_resolution = 10;

    Sphere() : MeshObject() {}

    Sphere(glm::vec3 c, float r) : MeshObject(), m_center(c), m_radius(r) {}

    void setResolution(unsigned int new_resolution){
        resolution = new_resolution;
        std::cout << "resolution =" << resolution << std::endl;
    }

    void build_arrays() {
        indexed_vertices.resize(resolution * resolution);
        normals.resize(resolution * resolution);
        coord_texture.resize(resolution * resolution);
        for (unsigned int thetaIt = 0; thetaIt < resolution; ++thetaIt) {
            float u = (float) (thetaIt) / (float) (resolution - 1);
            float theta = u * 2 * M_PI;
            for (unsigned int phiIt = 0; phiIt < resolution; ++phiIt) {
                unsigned int vertexIndex = thetaIt + phiIt * resolution;
                float v = (float) (phiIt) / (float) (resolution - 1);
                float phi = -M_PI / 2.0 + v * M_PI;
                glm::vec3 xyz = SphericalCoordinatesToEuclidean(theta, phi);
                indexed_vertices[vertexIndex] = m_center + m_radius * xyz;
                normals[vertexIndex] = xyz;
                coord_texture[vertexIndex][0] = u;
                coord_texture[vertexIndex][1] = v;
            }
        }
        triangles.clear();
        for (unsigned int thetaIt = 0; thetaIt < resolution - 1; ++thetaIt) {
            for (unsigned int phiIt = 0; phiIt < resolution - 1; ++phiIt) {
                unsigned short vertexuv = thetaIt + phiIt * resolution;
                unsigned short vertexUv = thetaIt + 1 + phiIt * resolution;
                unsigned short vertexuV = thetaIt + (phiIt + 1) * resolution;
                unsigned short vertexUV = thetaIt + 1 + (phiIt + 1) * resolution;

                indices.push_back(vertexuv);
                indices.push_back(vertexUv);
                indices.push_back(vertexUV);

                indices.push_back(vertexuv);
                indices.push_back(vertexUV);
                indices.push_back(vertexuV);

                std::vector<unsigned short> t1, t2;
                t1.push_back(vertexuv);
                t1.push_back(vertexUv);
                t1.push_back(vertexUV);

                t2.push_back(vertexuv);
                t2.push_back(vertexUV);
                t2.push_back(vertexuV);

                triangles.push_back(t1);
                triangles.push_back(t2);
            }
        }
    }

    glm::vec3 SphericalCoordinatesToEuclidean( float theta , float phi ) {
        return glm::vec3( cos(theta) * cos(phi) , sin(theta) * cos(phi) , sin(phi) );
    }
};


#endif //TP1_CODE_SPHERE_H
