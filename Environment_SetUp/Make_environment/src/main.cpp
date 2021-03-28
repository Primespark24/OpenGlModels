/*
 * Based on work by Graham Sellers and OpenGL SuperBible7
 * Also: https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
 *       http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ 
 * 
 * Example of loading data from object files
 * 
 */

#include <sb7.h>
#include <shader.h>
#include <vmath.h>

#include <loadingFunctions.h>
#include <skybox.h>

//Needed for file loading (also vector)
#include <string>
#include <fstream>

// For error checking
#include <vector>
#include <cassert>
#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

class test_app : public sb7::application{

    public:

    void init(){
        // Set up appropriate title
        static const char title[] = "Brycen Object Loading";
        sb7::application::init();
        memcpy(info.title, title, sizeof(title));

        info.windowWidth = 900; //Make sure things are square to start with
        info.windowHeight = 900;
    }
    
    void startup(){
        //Load object from file into vector<obj_>s
        load_obj(".\\bin\\media\\PizzaPlate.obj",obj_verticies,obj_uv,obj_normals,obj_num);
        load_obj(".\\bin\\media\\SteveBlank.obj",obj_verticies_2,obj_uv_2,obj_normals_2,obj_num_2); 
        load_obj(".\\bin\\media\\Planet.obj",obj_verticies_3,obj_uv_3,obj_normals_3,obj_num_3);       
        //We won't use uv or normals until we get to textures and lighting

        // Placeholders for loaded shaders
        GLuint shaders[2];

        //Load respective shaders
        //These need to be co-located with main.cpp in src
        shaders[0] = sb7::shader::load(".\\src\\vs.glsl", GL_VERTEX_SHADER);
        compiler_error_check(shaders[0]);
        shaders[1] = sb7::shader::load(".\\src\\fs.glsl", GL_FRAGMENT_SHADER);
        compiler_error_check(shaders[1]);
        //Put together program from the two loaded shaders
        rendering_program = sb7::program::link_from_shaders(shaders, 2, true);
        GL_CHECK_ERRORS

        glUseProgram(rendering_program); //unkown if useful
        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        //Load device memory
        //Vertex Buffer
        glGenBuffers(1,&obj_vert_buffer); //Get buffer index
        glBindBuffer(GL_ARRAY_BUFFER,obj_vert_buffer); //Bind index to by array buffer
        glBufferData(GL_ARRAY_BUFFER,                                   //Buffer Array identification
                     obj_verticies.size() * sizeof(obj_verticies[0]),   //Number of bytes (length * size of element)
                     obj_verticies.data(),                              //source of data (direct pointer transfer)
                     GL_STATIC_DRAW );                                  //We only need to set this once (right now)

        //Vertex Buffer
        glGenBuffers(1,&obj_vert_buffer_2); //Get buffer index
        glBindBuffer(GL_ARRAY_BUFFER,obj_vert_buffer_2); //Bind index to by array buffer
        glBufferData(GL_ARRAY_BUFFER,                                   //Buffer Array identification
                     obj_verticies_2.size() * sizeof(obj_verticies_2[0]),   //Number of bytes (length * size of element)
                     obj_verticies_2.data(),                              //source of data (direct pointer transfer)
                     GL_STATIC_DRAW );                                  //We only need to set this once (right now)

        //vertex Buffer
        glGenBuffers(1,&obj_vert_buffer_3); //Get buffer index
        glBindBuffer(GL_ARRAY_BUFFER,obj_vert_buffer_3); //Bind Index to array buffer
        glBufferData(GL_ARRAY_BUFFER,                                       //Buffer Array Id
                     obj_verticies_3.size() * sizeof(obj_verticies_3[0]),   //number of bytes
                     obj_verticies_3.data(),                                //Source of data
                     GL_STATIC_DRAW );                                      //Only set now

        //////////////////////////////////////////////////////////////////////////////////////////////
        // This would be where we need to load the normal and UV data as well (if we were using it) //
        //////////////////////////////////////////////////////////////////////////////////////////////

        //Set Up Vertex Attribute IDs 
        transformID = glGetUniformLocation(rendering_program,"transform");
        perspecID = glGetUniformLocation(rendering_program,"perspective");
        toCamID = glGetUniformLocation(rendering_program,"toCamera");
        attrib_obj_vertex = glGetAttribLocation(rendering_program,"obj_vertex");

        //Depth Test Enable (only render things 'forward' of other things)
        glEnable(GL_DEPTH_TEST);
        // Passes if the fragment's depth values is less than stored value
        glDepthFunc(GL_LEQUAL);      

        glEnable(GL_DEPTH_CLAMP);

        //End of set up check
        GL_CHECK_ERRORS
    }

    void shutdown(){
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteBuffers(1, &obj_vert_buffer);
        glDeleteProgram(rendering_program);
    }

    void render(double curTime){

        //Depth Mask Reset
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(rendering_program);

        // Object to world transform: scale + rotate
        GLfloat angle = curTime*20; //vmath assumes degrees
        vmath::mat4 transform = vmath::rotate( static_cast<float>(angle), 0.0f, 0.0f, 1.0f) *
                    vmath::scale( 0.1f ) * //Scale down
                    vmath::rotate( 90.0f, 1.0f, 0.0f, 0.0f) * //orient the pizza plate be upright in accordance with steves body
                    vmath::translate(0.5f, -6.0f, -0.4f); //orient pizza plate to be in front of on steve

        // Link transform to shader
        glUniformMatrix4fv(transformID, 1, GL_FALSE, &transform[0][0]);

        //Basic frustum perspective set up
        vmath::mat4 perspective = vmath::frustum( -1.0f, 1.0f, -1.0f, 1.0f, 20.0f, -20.0f);
        glUniformMatrix4fv(perspecID, 1, GL_FALSE, &perspective[0][0]);

        //Set up and transfer camera transform
        //vmath::mat4 camera = vmath::rotate(100.0f,1.0f,0.0f,0.0f); //Rotate down on the x axis

        //Utilizing lookat function
        vmath::vec3 eye(0.15f, 0.2f, 0.2f);  //Where the camera is
        vmath::vec3 cent(0.1f, 0.0f, 0.1f); //Where the camera is looking
        vmath::vec3 up(0.0f, 0.0f, -1.0f);    //what direction is 'up'
        
        
        const float camSpeed = 1.0f;
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            eye += camSpeed * cent;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            eye -= camSpeed * cent; 
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            eye -= vmath::normalize(vmath::cross(cent, up)) * camSpeed;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            eye += vmath::normalize(vmath::cross(cent, up)) * camSpeed;
        }

        vmath::mat4 camera = vmath::lookat(eye,cent,up);
        glUniformMatrix4fv(toCamID, 1, GL_FALSE, &camera[0][0]); //Link to shader

        //Link obj_vert_buffer to attrib_obj_vertex
        glEnableVertexAttribArray(attrib_obj_vertex); //Set attribute active for obj. vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER,obj_vert_buffer);//Acttivate vertex buffer (to bind to attribute obj vertex)
        glVertexAttribPointer( //This will make attrib_obj_vertex (our link into the vertex shader) point to values in obj_vert_buffer
            attrib_obj_vertex, //Attribute in question
            4,                 //Number of elements (vec4)
            GL_FLOAT,          //Type of element
            GL_FALSE,          //Normalize? Nope
            0,                 //No stride (steps between indexes)
            0                  //initial offset
        );

        //Start drawing some triangles!
        glDrawArrays(GL_TRIANGLES, 0, obj_verticies.size());

        //////////////////////////////////
        // Object 2!                    //
        //////////////////////////////////

        transform = vmath::rotate( static_cast<float>(angle), 0.0f, 0.0f, 1.0f) *
                    vmath::scale( 0.025f ) * //Scale down
                    vmath::rotate( 180.0f, 1.0f, 0.0f, 0.0f) * //orient the Steve to be upright in correlation to planet
                    vmath::translate(0.0f, 0.0f, 18.0f); //orient Steve to be standing on planet surface

        // Link transform to shader
        glUniformMatrix4fv(transformID, 1, GL_FALSE, &transform[0][0]);

        //Link obj_vert_buffer to attrib_obj_vertex
        glEnableVertexAttribArray(attrib_obj_vertex); //Set attribute active for obj. vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER,obj_vert_buffer_2);//Acttivate vertex buffer (to bind to attribute obj vertex)
        glVertexAttribPointer( //This will make attrib_obj_vertex (our link into the vertex shader) point to values in obj_vert_buffer
            attrib_obj_vertex, //Attribute in question
            4,                 //Number of elements (vec4)
            GL_FLOAT,          //Type of element
            GL_FALSE,          //Normalize? Nope
            0,                 //No stride (steps between indexes)
            0                  //initial offset
        );

        glDrawArrays(GL_TRIANGLES, 0, obj_verticies_2.size());
        
        //////////////////////////////////
        // Object 3!                    //
        //////////////////////////////////

        transform = vmath::rotate( static_cast<float>(-angle), 0.0f, 0.0f, 1.0f) *
                    vmath::scale( 0.1f ) * //Scale down
                    vmath::rotate( -90.0f, 1.0f, 0.0f, 0.0f); //orient the planet 'right side up'

        // Link transform to shader
        glUniformMatrix4fv(transformID, 1, GL_FALSE, &transform[0][0]);

        //Link obj_vert_buffer to attrib_obj_vertex
        glEnableVertexAttribArray(attrib_obj_vertex); //Set attribute active for obj. vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER,obj_vert_buffer_3);//Acttivate vertex buffer (to bind to attribute obj vertex)
        glVertexAttribPointer( //This will make attrib_obj_vertex (our link into the vertex shader) point to values in obj_vert_buffer
            attrib_obj_vertex, //Attribute in question
            4,                 //Number of elements (vec4)
            GL_FLOAT,          //Type of element
            GL_FALSE,          //Normalize? Nope
            0,                 //No stride (steps between indexes)
            0                  //initial offset
        );

        glDrawArrays(GL_TRIANGLES, 0, obj_verticies_3.size());

        //Reset Attribute Array
        glDisableVertexAttribArray(attrib_obj_vertex);        

        runtime_error_check();
    }

    void runtime_error_check(GLuint tracker = 0)
    {
        GLenum err = glGetError();
        
        if (err) {
            char buf[50];
            sprintf(buf, "Error(%d) = %x", tracker, err);
            MessageBoxA(NULL, buf, "Error running program!", MB_OK);
        }
    }

    void compiler_error_check(GLuint shader)
    {
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            if(maxLength == 0){
                char buf[50];
                sprintf(buf, "Possibly shaders not found!", maxLength);
                MessageBoxA(NULL, buf, "Error in shader compilation", MB_OK);
            } else {
                // Provide the infolog in whatever manor you deem best.
                MessageBoxA(NULL, &errorLog[0], "Error in shader compilation", MB_OK);
            }

            // Exit with failure.
            glDeleteShader(shader); // Don't leak the shader.
        }
    }

    private:
        GLuint rendering_program;
        GLuint vertex_array_object;

        //Uniform attributes
        GLuint transformID; //Dynamic transform of object
        GLuint perspecID;   //Perspective transform
        GLuint toCamID;     //World to Camera transform

        //Buffer identifiers
        GLuint obj_vert_buffer;
        GLuint obj_vert_buffer_2;
        GLuint obj_vert_buffer_3;

        //Vertex Attributes
        GLuint attrib_obj_vertex; 

        //Data for object
        std::vector<vmath::vec4> obj_verticies;
        std::vector<vmath::vec4> obj_normals;
        std::vector<vmath::vec2> obj_uv;
        GLuint obj_num;

        //Data for object 2
        std::vector<vmath::vec4> obj_verticies_2;
        std::vector<vmath::vec4> obj_normals_2;
        std::vector<vmath::vec2> obj_uv_2;
        GLuint obj_num_2;

        //Data for object 3
        std::vector<vmath::vec4> obj_verticies_3;
        std::vector<vmath::vec4> obj_normals_3;
        std::vector<vmath::vec2> obj_uv_3;
        GLuint obj_num_3;

        
    //Object Loading Information
    //Referenced from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
    // and http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ 
    // When exporting from blender, keep everything default except:
    //       Make sure to include Normals
    //       Include UVs
    //       Triangulate Faces
    //       Don't 'Write Materials'

    // filename - Blender .obj file (see file formatting specifics above)
    // All vectors passed by reference and filled in function
    // vertices - list of, in order, verticies for object. In triangles
    // UVs - Texture mapping coordinates, indexed with the above vertices
    // normals - index with the above vertices
    // number - Total number of points in vertices (should be vertices.length())
    void load_obj(const char* filename, std::vector<vmath::vec4> &vertices, std::vector<vmath::vec2> &uvs, std::vector<vmath::vec4> &normals, GLuint &number)
    {
        //File to load in
        std::ifstream in(filename, std::ios::in);

        //Check to make sure file opened
        if (!in) {
            char buf[50];
            sprintf(buf, "OBJ file not found!");
            MessageBoxA(NULL, buf, "Error in loading obj file", MB_OK);
        }

        //Temp vectors to hold data
        //These will need to be indexed into the output vectors based on face info
        std::vector<vmath::vec4> tempVert; // from vertices lines 'v <x> <y> <z>'
        //This might need to be a more complicated structure, we can get away with strict indexing
        std::vector<GLuint> tempFace; // from face line 'f <v1>/<t1>/<n1> <v2>/<t2>/<n2> <v3>/<t3>/<n3>', should be indexes
        std::vector<vmath::vec2> tempUVs; // from texture line 'vt <x> <y>'
        std::vector<vmath::vec4> tempNorm; // from a normal line 'vn <x> <y> <z>' //Should this be a vec3 or vec4?

        std::string line;       // Complete line pulled from file    
        std::string sub = "";   // Substring working space
        GLfloat num;            // Temp floats from line processing
        while (std::getline(in, line)) { //grab every line of the file
            //Depending on what is in the line, parse differently 
            
            if (line.substr(0,2) == "v ") {
                //Process vertex line
                sub = line.substr(2); //Isolate substring without 'v'

                //vector to be filled
                vmath::vec4 tVec(0.0f,0.0f,0.0f,1.0f);                
                
                //Expect 3 numbers, iterate three times
                //Isolate number and clip substring [x,y,z,1]
                tVec[0] = stof(parseAndClip(sub," "));
                tVec[1] = stof(parseAndClip(sub," "));
                tVec[2] = stof(parseAndClip(sub," "));
                
                //Push vec4 onto verticies vector
                tempVert.push_back(tVec);
               
            } else if (line.substr(0,2) == "f ") {
                //Process face line 
                // Faces line f 14/25/9 60/19/9 56/97/9 : f <vertex1>/<texture1>/<normal1> <vertex2>/<texture2>/<normal2> <vertex3>/<texture3>/<normal3>
                sub = line.substr(2); //Current sub string of line
                //Expect 3 number sets, loop three times
                for(int i = 0; i < 3; i++) {
                    //Isolate next triangle set
                    //First before / /
                    tempFace.push_back( stoi(parseAndClip(sub,"/")) ); //sub_part 0 == vertex index
                    
                    //second value in triplet (between the / and /)
                    tempFace.push_back( stoi(parseAndClip(sub,"/")) ); //sub_part 1 == texture coordinate index

                    //third value in triplet (after both /, " " up next)
                    tempFace.push_back( stoi(parseAndClip(sub," ")) ); //sub_part 2 == normal index                    
                }
                //Already pushed info onto faces
            
            } else if (line.substr(0,3) == "vt ") {
                //Process Texture mappings
                sub = line.substr(3); //Isolate substring without 'vt '

                vmath::vec2 tUV(0.0f,0.0f);
                //Expect 2 numbers, loop twice
                tUV[0] = stof(parseAndClip(sub," ")); //x
                tUV[1] = stof(parseAndClip(sub," ")); //y

                //Push vec2 onto texture vector
                tempUVs.push_back(tUV);                

            } else if (line.substr(0,3) == "vn ") {
                //Process normal line
                sub = line.substr(3); //Isolate substring without 'vt '

                //4D? is w:0?
                vmath::vec4 tNorm(0.0f,0.0f,0.0f,0.0f);

                //Expect 3 numbers, loop thrice
                //Isolate number and clip substring
                tNorm[0] = stof(parseAndClip(sub," "));
                tNorm[1] = stof(parseAndClip(sub," "));
                tNorm[2] = stof(parseAndClip(sub," "));
                
                //Push normal onto tempNormal vector
                tempNorm.push_back(tNorm);

            } else {
                //other kind of line, ignoring
            }
        } //end of line parsing

        //Clear out output vectors (just to be safe)
        vertices.clear();
        uvs.clear();
        normals.clear();
        number = 0;

        //At this point out temp vectors are full of data
        // tempVert, tempUVs and tempNorm are indexed (starting at 0) in file order
        // tempFace correlates everything together in sets of 9 values (three triplets)
        // Using the data in tempFace the output vectors will be filled with the correct (in order) data

        //                   0    1    2    3    4    5    6    7    8
        // Faces striping: <v1>/<t1>/<n1> <v2>/<t2>/<n2> <v3>/<t3>/<n3>
        //Because the data in tempFace is striped buy sets of three triplets, step forward by 9 each time
        for(int i = 0; i < tempFace.size(); i += 9 ){
            //Pull data into vertices
            //                                   VVV Index offset pattern
            //                          VVV Holds vertex index to pull from tempVery (offset from starting at 1 to 0)    
            //                 VVV Indexed vertex info
            vertices.push_back(tempVert[tempFace[i+0]-1]); //v1
            vertices.push_back(tempVert[tempFace[i+3]-1]); //v2
            vertices.push_back(tempVert[tempFace[i+6]-1]); //v3

            //Pull data into uvs
            uvs.push_back(tempUVs[tempFace[i+1]-1]); //uv1
            uvs.push_back(tempUVs[tempFace[i+4]-1]); //uv2
            uvs.push_back(tempUVs[tempFace[i+7]-1]); //uv3

            //Pull data into normals
            normals.push_back(tempNorm[tempFace[i+2]-1]); //n1
            normals.push_back(tempNorm[tempFace[i+5]-1]); //n1
            normals.push_back(tempNorm[tempFace[i+8]-1]); //n1

            number++; //Sanity Check to make sure things line up
        }
        
    }

    //File parsing helper
    //Pull off the first element of sub up to delim
    // Ex: sub |0.877342 0.081279 -0.329742| delim: " "
    // returns |0.877342|
    // changes sub (via reference) to |0.081279 -0.329742|    
    std::string parseAndClip(std::string& sub, std::string delim){
        unsigned int curIndex = sub.find(delim); //Defined once to speed up
        //Grab substring upto first delimeter
        std::string ret = sub.substr(0,curIndex);
        //delete everything upto delimeter
        sub = sub.substr(curIndex+1);
        return ret;
    }
};


DECLARE_MAIN(test_app);
