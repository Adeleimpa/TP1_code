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

#include "SceneObject.h"

class Plane : public SceneObject {

private:
    double width;
    double height;

    int w;
    int h;

public:
    Plane() {}
    Plane( double wi , double he , int nw, int nh) {
        width = wi;
        height = he;
        h = nw;
        w = nh;
    }

    void setDimension(int nw, int nh){
        h = nw;
        w = nh;
    }

    void addRelief(//std::vector<glm::vec3> &indexed_vertices,
                   char fix_coord){
        for(int i = 0; i < indexed_vertices.size(); i++){

            double f = (double)rand() / RAND_MAX;
            double rand_d = -0.2 + f * (0.2 - (-0.2));

            if(fix_coord == 'x'){
                indexed_vertices[i][0] += rand_d;
            }else if(fix_coord == 'y'){
                indexed_vertices[i][1] += rand_d;
            }else if(fix_coord == 'z'){
                indexed_vertices[i][2] += rand_d;
            }

        }
    }

    void generatePlane(  //std::vector<unsigned short> &indices,
                         //std::vector<std::vector<unsigned short> > &triangles,
                         //std::vector<glm::vec3> &indexed_vertices,
                         //std::vector<glm::vec3> &normals,
                         //std::vector<glm::vec2> &coord_texture,
                         char fix_coord){


        glm::vec3 start_corner;

        if(fix_coord == 'x'){
            start_corner = {0.0, -width/2.0, -height/2.0};
        }else if(fix_coord == 'y'){
            start_corner = {-width/2.0, 0.0, -height/2.0};
        }else if(fix_coord == 'z'){
            start_corner = {-width/2.0, -height/2.0, 0.0};
        }else{
            std::cout << "WARNING: wrong fixed coordinate in parameters to generate plane" << std::endl;
        }

        double step_1 = width/(double)h;
        double step_2 = height/(double)w;

        glm::vec3 current_corner;

        // fill indexed_vertices
        for(int i = 0; i <= h; i++) {
            for (int j = 0; j <= w; j++) {

                glm::vec3 normal;

                if(fix_coord == 'x'){
                    current_corner = {start_corner[0], start_corner[1] + i*step_1, start_corner[2] + j*step_2};
                    normal = {1.0, 0.0, 0.0};
                    // texture
                    coord_texture.push_back({current_corner[1]/width, 1.0-current_corner[2]/height});

                }else if(fix_coord == 'y') {
                    current_corner = {start_corner[0] + i*step_1, start_corner[1], start_corner[2] + j*step_2};
                    normal = {0.0, 1.0, 0.0};
                    // texture
                    coord_texture.push_back({current_corner[0]/width, 1.0-current_corner[2]/height});

                }else if(fix_coord == 'z'){
                    current_corner = {start_corner[0] + i*step_1, start_corner[1] + j*step_2, start_corner[2]};
                    normal = {0.0, 0.0, 1.0};
                    // texture
                    coord_texture.push_back({current_corner[0]/width, 1.0-current_corner[1]/height});
                }

                //std::cout << current_corner[0] << current_corner[1] << current_corner[2] << std::endl;
                indexed_vertices.push_back(current_corner);
                normals.push_back(normal);
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


    void addHeightMap(unsigned char *HM_data, int height_HM, int width_HM,
                      //std::vector<glm::vec3> &indexed_vertices,
                      char fix_coord){
        double max = 1.0; // maximum height
        double min = 0.0; // minimum height
        // TODO adapt to any max and min value -> shader

        int height_plane = h + 1;
        int width_plane = w + 1;

        int range_ndg_HM = 256;

        //int count = 0;
        //int line = 0;
        for(int i = 0; i < indexed_vertices.size(); i++){

            /*if(count%10 == 0){
                line++;
                std::cout << "LINE " << line << std::endl;
            }
            count++;*/

            //int dat = (int) HM_data[i]
            int row = floor(i / height_plane);
            float row_plane = (float)row/(float)height_plane;
            int row_HM = floor(height_HM*row_plane);
            int col = i - row*width_plane;
            float col_plane = (float)col/(float)width_plane;
            int col_HM = floor(width_HM*col_plane);
            int dat = (int) HM_data[row_HM*width_HM + col_HM];
            //std::cout << "data HeightMap :" << dat << std::endl;

            double ratio = (double)dat/(double)range_ndg_HM;
            //std::cout << "ratio :" << ratio << std::endl;
            double difference = max*ratio;
            //std::cout << "difference :" << difference << std::endl;

            if(fix_coord == 'x'){
                indexed_vertices[i][0] = max - difference;
            }else if(fix_coord == 'y'){
                indexed_vertices[i][1] = max - difference;
            }else if(fix_coord == 'z'){
                indexed_vertices[i][2] = max - difference;
                //std::cout << "new coord (z):" << indexed_vertices[i][2] << std::endl;
            }
        }
    }

};


#endif //TUTORIALS_PLANE_H
