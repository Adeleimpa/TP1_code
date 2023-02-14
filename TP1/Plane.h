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

        glm::vec3 start_corner(-width/2.0, -height/2.0, 0.0); // bottom left
        //glm::vec3 corner2(width/2.0, -height/2.0, 0.0); // bottom right
        //glm::vec3 corner3(-width/2.0, height/2.0, 0.0); // top left
        //glm::vec3 corner4(width/2.0, height/2.0, 0.0); // top right
        double x_step = width/(double)h;
        double y_step = height/(double)w;

        glm::vec3 current_corner;

        // fill indexed_vertices
        for(int i = 0; i <= h; i++) {
            for (int j = 0; j <= w; j++) {
                current_corner[0] = start_corner[0] + i*x_step;
                current_corner[1] = start_corner[1] + j*y_step;
                current_corner[2] = start_corner[2];
                //std::cout << current_corner[0] << current_corner[1] << current_corner[2] << std::endl;

                indexed_vertices.push_back(current_corner);

                // texture
                coord_texture.push_back({current_corner[0]/width, 1.0-current_corner[1]/height});
            }
        }

        for(int i = 0; i < h; i++){
            for(int j = 0; j < w; j++){
                std::vector<unsigned short> triangle1;
                std::vector<unsigned short> triangle2;

                int c1, c2, c3, c4;

                c1 = i + j*(w+1);
                c2 = (i+1) + j*(w+1);
                c3 = i + (j+1)*(w+1);
                c4 = (i+1) + (j+1)*(w+1);

                /*std::cout << c1 << std::endl;
                std::cout << c2 << std::endl;
                std::cout << c3 << std::endl;
                std::cout << c4 << std::endl;
                std::cout << "-------" << std::endl;*/

                indices.push_back(c1);
                indices.push_back(c2);
                indices.push_back(c3);

                indices.push_back(c3);
                indices.push_back(c4);
                indices.push_back(c2);

                triangle1.push_back(c1);
                triangle1.push_back(c2);
                triangle1.push_back(c3);

                triangle2.push_back(c3);
                triangle2.push_back(c4);
                triangle2.push_back(c2);

                triangles.push_back(triangle1);
                triangles.push_back(triangle2);
            }
        }
    }

    void addZRelief(std::vector<glm::vec3> &indexed_vertices){
        for(int i = 0; i < indexed_vertices.size(); i++){

            double f = (double)rand() / RAND_MAX;
            double rand_z = -0.2 + f * (0.2 - (-0.2));

            indexed_vertices[i][2] = indexed_vertices[i][2] + rand_z;
        }
    }

    void addYRelief(std::vector<glm::vec3> &indexed_vertices){
        for(int i =0; i < indexed_vertices.size(); i++){

            double f = (double)rand() / RAND_MAX;
            double rand_z = -0.2 + f * (0.2 - (-0.2));

            indexed_vertices[i][1] = indexed_vertices[i][1] + rand_z;
        }
    }

    // PLANE XZ - TO FIX
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
                coord_texture.push_back({current_c1[0]/width, current_c1[2]/height});
                coord_texture.push_back({current_c2[0]/width, current_c2[2]/height});
                coord_texture.push_back({current_c3[0]/width, current_c3[2]/height});
                coord_texture.push_back({current_c4[0]/width, current_c4[2]/height});
            }
        }
    }

};


#endif //TUTORIALS_PLANE_H
