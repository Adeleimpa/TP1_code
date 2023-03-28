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
    //unsigned int nTheta, nPhi;

    Sphere() : MeshObject() {}

    Sphere(glm::vec3 c, float r) : MeshObject(), m_center(c), m_radius(r) {}

    void build_arrays() {
        unsigned int nTheta = 20, nPhi = 20;
        indexed_vertices.resize(nTheta * nPhi);
        normals.resize(nTheta * nPhi);
        coord_texture.resize(nTheta * nPhi);
        for (unsigned int thetaIt = 0; thetaIt < nTheta; ++thetaIt) {
            float u = (float) (thetaIt) / (float) (nTheta - 1);
            float theta = u * 2 * M_PI;
            for (unsigned int phiIt = 0; phiIt < nPhi; ++phiIt) {
                unsigned int vertexIndex = thetaIt + phiIt * nTheta;
                float v = (float) (phiIt) / (float) (nPhi - 1);
                float phi = -M_PI / 2.0 + v * M_PI;
                glm::vec3 xyz = SphericalCoordinatesToEuclidean(theta, phi);
                indexed_vertices[vertexIndex] = m_center + m_radius * xyz;
                normals[vertexIndex] = xyz;
                coord_texture[vertexIndex][0] = u;
                coord_texture[vertexIndex][1] = v;
            }
        }
        triangles.clear();
        for (unsigned int thetaIt = 0; thetaIt < nTheta - 1; ++thetaIt) {
            for (unsigned int phiIt = 0; phiIt < nPhi - 1; ++phiIt) {
                unsigned short vertexuv = thetaIt + phiIt * nTheta;
                unsigned short vertexUv = thetaIt + 1 + phiIt * nTheta;
                unsigned short vertexuV = thetaIt + (phiIt + 1) * nTheta;
                unsigned short vertexUV = thetaIt + 1 + (phiIt + 1) * nTheta;

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
