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

void main(){

        // color = vec3(0.2, 0.2,0.4);

        /*if(height < 0.5){
                FragColor = texture(texture_grass, coord_txt);
        }else if (height > 0.5 && height < 0.7){
                FragColor = texture(texture_rock, coord_txt);
        }else{
                FragColor = texture(texture_snowrocks, coord_txt);
        }*/

        FragColor = texture(texture_grass, coord_txt);

}
