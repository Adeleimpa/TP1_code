#version 330 core

// Ouput data
out vec3 color;

out vec4 FragColor;

// texture
uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snowrocks;
in vec2 coord_txt;

// heightmap
in float height;

float coef_grass;
float coef_rock;
float coef_snowrocks;

void main(){

        // color = vec3(0.2, 0.2,0.4);

        if (height < 0.2){
                //color = vec3(1.0, 1.0, 1.0); // white
                FragColor = texture(texture_grass, coord_txt);
        } else if (height >= 0.2 && height < 0.7){
                //color = vec3(0.5, 0.5, 0.5); // grey
                FragColor = texture(texture_rock, coord_txt);
        } else if (height >= 0.7){
                //color = vec3(0.0, 0.0, 0.0); // black
                FragColor = texture(texture_snowrocks, coord_txt);
        }

        // SMOOTH : mix of textures
        if (height >= 0.18 && height <= 0.22){
                if(height - 0.2 < 0.0){
                        coef_grass = 1.0 - height;
                        coef_rock = 1.0 - coef_grass;
                }else{
                        coef_rock = 1.0 - height;
                        coef_grass = 1.0 - coef_rock;
                }
                FragColor = coef_grass*texture(texture_grass, coord_txt) + coef_rock*texture(texture_rock, coord_txt);
        }

        if (height >= 0.68 && height <= 0.72){
                if(height - 0.7 < 0.0){
                        coef_snowrocks = 1.0 - height;
                        coef_rock = 1.0 - coef_snowrocks;
                }else{
                        coef_rock = 1.0 - height;
                        coef_snowrocks = 1.0 - coef_rock;
                }
                FragColor = coef_rock*texture(texture_rock, coord_txt) + coef_snowrocks*texture(texture_snowrocks, coord_txt);
        }


}
