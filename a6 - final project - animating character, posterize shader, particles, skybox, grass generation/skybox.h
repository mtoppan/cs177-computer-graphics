#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Common.h"
#include "Driver.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "TinyObjLoader.h"
#include <StbImage.h>

#ifndef SKYBOX_H_
#define SKYBOX_H_

// Generates face names for a cubemap
std::vector<std::string> Get_Faces(std::string weather) {
    std::vector<std::string> faces
    {
        weather + "/px.png",
        weather + "/nx.png",
        weather + "/py.png",
        weather + "/ny.png",
        weather + "/pz.png",
        weather + "/nz.png"
    };
    return faces;
}

// Loads a cubemap
// Modified from https://learnopengl.com/Advanced-OpenGL/Cubemaps
std::shared_ptr<OpenGLTexture> Load_Cubemap(std::vector<std::string> faces, int i)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + i); // Load texture to unique unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // load and generate the textures
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = nullptr;
        Stb::Read_Image(faces[i], width, height, nrChannels, data);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
        }

        free(data);
    }

    return std::shared_ptr<OpenGLTexture>(new OpenGLTexture(texture));
}

// Adds a skybox to the scene
int Create_Skybox(OpenGLViewer* viewer, std::vector<OpenGLTriangleMesh*>* mesh_object_array)
{
    // Set depth function
    glDepthFunc(GL_LEQUAL);

    // Make cubemaps seamless
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Create the skybox mesh
    auto mesh_obj = viewer->Add_Interactive_Object<OpenGLTriangleMesh>();

    // Vertex positions
    // Taken from https://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330
    std::vector<Vector3> triangle_vertices={Vector3(-1,-1,0),Vector3(1,-1,0),Vector3(1,1,0),Vector3(-1,1,0)};
    std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
    vertices=triangle_vertices;

    // mesh elements
    std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
    elements={Vector3i(0,1,3),Vector3i(1,2,3)};

    // Create the shader
    OpenGLShaderLibrary::Instance()->Add_Shader_From_File("skybox.vert", "skybox.frag", "skybox");
    std::shared_ptr<OpenGLShaderProgram> shader = OpenGLShaderLibrary::Get_Shader("skybox");

    // Set texture unit bindings for each cubemap
    shader->Set_Uniform("tex_spring", 0);
    shader->Set_Uniform("tex_snow", 1);
    shader->Set_Uniform("tex_sand", 2);

    // Set mesh shader to skybox shader
    mesh_obj->Add_Shader_Program(shader);

    // Load cubemaps
    mesh_obj->Add_Texture("tex_spring", Load_Cubemap(Get_Faces("spring"), 0));
    mesh_obj->Add_Texture("tex_snow", Load_Cubemap(Get_Faces("snow"), 1));
    mesh_obj->Add_Texture("tex_sand", Load_Cubemap(Get_Faces("sand"), 2));
    mesh_obj->polygon_mode=PolygonMode::Fill;
    mesh_obj->shading_mode=ShadingMode::Texture;

    // initialize
    mesh_obj->Set_Data_Refreshed();
    mesh_obj->Initialize();	
    mesh_object_array->push_back(mesh_obj);
    return mesh_object_array->size()-1;
}

// Set the weather type for the skybox
void Set_Skybox_Weather(int weatherType) {
    // Get the shader
    std::shared_ptr<OpenGLShaderProgram> shader = OpenGLShaderLibrary::Get_Shader("skybox");

    // Update the weather type in the shader
    shader->Begin();
    shader->Set_Uniform("weather_type", weatherType);
    shader->End();
}

#endif