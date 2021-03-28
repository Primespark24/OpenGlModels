#version 450 core  

out vec4 vs_color; //Ouput to fragment shader

uniform mat4 transform; //Transformation matrix
uniform mat4 perspective; //Perspective transform
uniform mat4 toCamera; //world to Camera transform

in vec4 obj_vertex; //Currently being drawn point (of a triangle)
                                                                  
void main(void) {
    // Used to add an additional movement to our object
    // Used to observe effects of projection (if we want)
    mat4 translate = mat4(
		1.0, 0.0, 0.0, 0.0,      //x col
		0.0, 1.0, 0.0, 0.0,      //y col
		0.0, 0.0, 1.0, 0.0,      //z col
		0.0, 0.0, 0.0, 1.0);     //w col


    //All modifications are pulled in via attributes    
    //                                                             VVVVVVVVVV Pulled in via attribute from buffer
    gl_Position = perspective * toCamera * translate * transform * obj_vertex;

    vs_color = vec4(0.5,0.5,0.5,1.0);                          
}                                                                 