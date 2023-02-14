//
// Created by Adèle Imparato on 13/02/2023.
//

#ifndef TUTORIALS_PLANE_H
#define TUTORIALS_PLANE_H

#include <vector>
#include <iostream>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Plane {

private:
    double width;
    double height;

public:
    Plane() {}
    Plane( double w , double h ) {
        width = w;
        height = h;
    }

    void generatePlaneXY(int h, int w,
                         std::vector<unsigned short> &indices,
                         std::vector<std::vector<unsigned short> > &triangles,
                         std::vector<glm::vec3> &indexed_vertices,
                         std::vector<glm::vec2> &coord_texture){

        glm::vec3 corner1(-width/2.0, -height/2.0, 0.0); // bottom left
        glm::vec3 corner2(width/2.0, -height/2.0, 0.0); // bottom right
        glm::vec3 corner3(-width/2.0, height/2.0, 0.0); // top left
        glm::vec3 corner4(width/2.0, height/2.0, 0.0); // top right
        double x_step = width/(double)h;
        double y_step = height/(double)w;

        glm::vec3 current_c1;
        glm::vec3 current_c2;
        glm::vec3 current_c3;
        glm::vec3 current_c4;

        int index = -1;

        for(int i = 0; i < h; i++){
            for(int j = 0; j < w; j++){
                std::vector<unsigned short> triangle1;
                std::vector<unsigned short> triangle2;

                current_c1[0] = corner1[0] + i*x_step;
                current_c1[1] = corner1[1] + j*y_step;
                current_c1[2] = corner1[2];

                current_c2 = current_c1;
                current_c2[0] += x_step;

                current_c3 = current_c1;
                current_c3[1] += y_step;

                current_c4 = current_c1;
                current_c4[0] += x_step;
                current_c4[1] += y_step;

                indexed_vertices.push_back(current_c1);
                indexed_vertices.push_back(current_c2);
                indexed_vertices.push_back(current_c3);
                indexed_vertices.push_back(current_c4);

                indices.push_back(index + 1);
                indices.push_back(index + 2);
                indices.push_back(index + 3);
                indices.push_back(index + 3);
                indices.push_back(index + 4);
                indices.push_back(index + 2);

                triangle1.push_back(index + 1);
                triangle1.push_back(index + 2);
                triangle1.push_back(index + 3);
                triangle2.push_back(index + 3);
                triangle2.push_back(index + 4);
                triangle2.push_back(index + 2);

                index+=4;

                triangles.push_back(triangle1);
                triangles.push_back(triangle2);

                // texture
                /*coord_texture.push_back({i*x_step, j*y_step});
                coord_texture.push_back({i*x_step, j*y_step + y_step});
                coord_texture.push_back({i*x_step + x_step, j*y_step});
                coord_texture.push_back({i*x_step + x_step, j*y_step + y_step});*/
                coord_texture.push_back({current_c1[0]/width, current_c1[1]/height});
                coord_texture.push_back({current_c2[0]/width, current_c2[1]/height});
                coord_texture.push_back({current_c3[0]/width, current_c3[1]/height});
                coord_texture.push_back({current_c4[0]/width, current_c4[1]/height});
            }
        }
    }

    void addZRelief(std::vector<glm::vec3> &indexed_vertices){
        for(glm::vec3 vertex : indexed_vertices){

            double f = (double)rand() / RAND_MAX;
            double rand_z = -0.2 + f * (0.2 - (-0.2));

            vertex[2] = vertex[2] + rand_z;
        }
    }

    void addYRelief(std::vector<glm::vec3> &indexed_vertices){
        for(glm::vec3 vertex : indexed_vertices){

            double f = (double)rand() / RAND_MAX;
            double rand_z = -0.2 + f * (0.2 - (-0.2));

            vertex[1] = vertex[1] + rand_z;
        }
    }

    // PLANE XZ - TO TEST
    void generatePlaneXZ(int h, int w,
                         std::vector<unsigned short> &indices,
                         std::vector<std::vector<unsigned short> > &triangles,
                         std::vector<glm::vec3> &indexed_vertices,
                         std::vector<glm::vec2> &coord_texture){

        glm::vec3 corner1(-width/2.0, 0.0, 0.0); // bottom left
        glm::vec3 corner2(width/2.0, 0.0, 0.0); // bottom right
        glm::vec3 corner3(-width/2.0, 0.0, -height); // top left
        glm::vec3 corner4(width/2.0, 0.0, -height); // top right

        double x_step = width/(double)w;
        double z_step = height/(double)h;

        glm::vec3 current_c1;
        glm::vec3 current_c2;
        glm::vec3 current_c3;
        glm::vec3 current_c4;

        int index = -1;

        for(int i = 0; i < w ; i++){
            for(int j = 0 ; j < h; j++){
                std::vector<unsigned short> triangle1;
                std::vector<unsigned short> triangle2;

                current_c1[0] = corner1[0] + i*x_step;
                current_c1[1] = corner1[1];
                current_c1[2] = corner1[2] + j*z_step;

                current_c2 = current_c1;
                current_c2[0] += (i+1)*x_step;

                current_c3 = current_c1;
                current_c3[2] += (j+1)*z_step;

                current_c4 = current_c1;
                current_c4[0] += (i+1)*x_step;
                current_c4[2] += (j+1)*z_step;

                indexed_vertices.push_back(current_c1);
                indexed_vertices.push_back(current_c2);
                indexed_vertices.push_back(current_c3);
                indexed_vertices.push_back(current_c4);

                indices.push_back(index + 1);
                indices.push_back(index + 2);
                indices.push_back(index + 3);
                indices.push_back(index + 3);
                indices.push_back(index + 4);
                indices.push_back(index + 2);

                triangle1.push_back(index + 1);
                triangle1.push_back(index + 2);
                triangle1.push_back(index + 3);
                triangle2.push_back(index + 3);
                triangle2.push_back(index + 4);
                triangle2.push_back(index + 2);

                index+=4;

                triangles.push_back(triangle1);
                triangles.push_back(triangle2);

                // texture
                coord_texture.push_back({current_c1[0]/width, current_c1[1]/height});
                coord_texture.push_back({current_c2[0]/width, current_c2[1]/height});
                coord_texture.push_back({current_c3[0]/width, current_c3[1]/height});
                coord_texture.push_back({current_c4[0]/width, current_c4[1]/height});
            }
        }
    }

};


#endif //TUTORIALS_PLANE_H
