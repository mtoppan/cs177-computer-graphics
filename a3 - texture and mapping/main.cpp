//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
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
#include "TinyObjLoader.h"

#include <cmath>

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class ShaderDriver : public Driver, public OpenGLViewer
{using Base=Driver;
	std::vector<OpenGLTriangleMesh*> mesh_object_array;						////mesh objects, every object you put in this array will be rendered.
	clock_t startTime;

public:
	virtual void Initialize()
	{
		draw_bk=true;						////this flag specifies a customized way to draw the background. If you turn it off, there is no background.
		draw_axes=false;					////if you don't like the axes, turn them off!
		startTime = clock();
		OpenGLViewer::Initialize();
	}

	////This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		std::cout<<"load tri_mesh from obj file, #vtx: "<<mesh_obj->mesh.Vertices().size()<<", #ele: "<<mesh_obj->mesh.Elements().size()<<std::endl;		

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a sphere mesh
	int Add_Sphere_Object(const double radius=1.)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Initialize_Sphere_Mesh(radius,&mesh_obj->mesh,3);		////add a sphere with radius=1. if the obj file name is not specified

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a square with two triangles (the X-hour demo)
	int Add_Square_Object(const std::vector<Vector3>& vertices)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////manually initialize the vertices and elements for a triangle mesh
		mesh.Vertices().resize(4);
		for(int i=0;i<vertices.size();i++)mesh.Vertices()[i]=vertices[i];

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function demonstrates how to manipulate the color and normal arrays of a mesh on the CPU end.
	////The updated colors and normals will be sent to GPU for rendering automatically.
	void Update_Vertex_Color_And_Normal_For_Mesh_Object(OpenGLTriangleMesh* obj)
	{
		int vn=(int)obj->mesh.Vertices().size();					////number of vertices of a mesh
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		////you might find this array useful
		std::vector<Vector3i>& elements=obj->mesh.Elements();		////you might find this array also useful

		std::vector<Vector4f>& vtx_color=obj->vtx_color;
		vtx_color.resize(vn);
		std::fill(vtx_color.begin(),vtx_color.end(),Vector4f::Zero());

		for(int i=0;i<vn;i++){
			vtx_color[i]=Vector4f(0.,1.,0.,1.);	////specify color for each vertex
		}

		////The vertex normals are calculated on the back-end for this assignment. You don't need to worry about the normal calculation this time.
	}

	void Update_Vertex_UV_For_Mesh_Object(OpenGLTriangleMesh* obj)
	{
		int vn=(int)obj->mesh.Vertices().size();					////number of vertices of a mesh
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		////you might find this array useful
		std::vector<Vector2>& uv=obj->mesh.Uvs();					////you need to set values in uv to specify the texture coordinates
		uv.resize(vn);
		for(int i=0;i<vn;i++){uv[i]=Vector2(0.,0.);}				////set uv to be zero by default

		Update_Uv_Using_Spherical_Coordinates(vertices,uv);
	}

	////TODO [Step 0]: update the uv coordinates for each vertex using the spherical coordinates.
	////NOTICE: This code updates the vertex color array on the CPU end. The array will then be sent to GPU and read it the vertex shader as v_color.
	////You don't need to implement the CPU-GPU data transfer code.
	void Update_Uv_Using_Spherical_Coordinates(const std::vector<Vector3>& vertices,std::vector<Vector2>& uv)
	{
		/*Your implementation starts*/	
		// IMPLEMENTED IN FRAGMENT SHADER //
		/*Your implementation ends*/
	}

	virtual void Initialize_Data()
	{
		//////Add a manually built square mesh (with two triangles). This is the demo code in X-hour.
		//// You don't need this part for your homework. Just put them here for your reference.
		//{
		//	std::vector<Vector3> triangle_vertices={Vector3(0,0,0),Vector3(1,0,0),Vector3(0,1,0),Vector3(1,1,0)};
		//	int obj_idx=Add_Square_Object(triangle_vertices);	////add a sphere
		//	auto obj=mesh_object_array[obj_idx];
		//	
		//	//specify the vertex colors on the CPU end
		//	std::vector<Vector4f>& vtx_color=obj->vtx_color;
		//	vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		//	std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		//	vtx_normal={Vector3(0.,0.,1.),Vector3(0.,0.,1.),Vector3(0.,0.,1.),Vector3(0.,0.,1.)};

		//	std::vector<Vector2>& uv=obj->mesh.Uvs();
		//	uv={Vector2(0.,0.),Vector2(1.,0.),Vector2(0.,1.),Vector2(1.,1.)};

		//	std::vector<Vector3i>& elements=obj->mesh.Elements();
		//	elements={Vector3i(0,1,3),Vector3i(0,3,2)};
		//}

		//////Add a sphere mesh
		// {
		// 	int obj_idx=Add_Sphere_Object();
		// 	auto obj=mesh_object_array[obj_idx];
		// 	Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		// 	Update_Vertex_UV_For_Mesh_Object(obj);			////This is the function you need to implement from Step 0 (for sphere only!)
		// }

		//////Add an obj mesh
		////TODO (Step 4): uncomment this part and use your own mesh for Step 4.

		///Uncomment for Step 4///

		{
			 int obj_idx=Add_Obj_Mesh_Object("car.obj");
			 auto obj=mesh_object_array[obj_idx];
			 Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		}
		{
			 int obj_idx=Add_Obj_Mesh_Object("tires.obj");
			 auto obj=mesh_object_array[obj_idx];
			 Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		}
		{
			 int obj_idx=Add_Obj_Mesh_Object("garage.obj");
			 auto obj=mesh_object_array[obj_idx];
			 Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		}

		////initialize shader
		// std::string vertex_shader_file_name="checkerboard.vert";		////TODO (Step 1 and 2): switch the file name to normal_mapping.vert
		// std::string fragment_shader_file_name="checkerboard.frag";		////TODO (Step 1 and 2): switch the file name to normal_mapping.frag
		std::string vertex_shader_file_name="normal_mapping.vert";		////TODO (Step 1 and 2): switch the file name to normal_mapping.vert
		std::string fragment_shader_file_name="normal_mapping.frag";		////TODO (Step 1 and 2): switch the file name to normal_mapping.frag
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File(vertex_shader_file_name,fragment_shader_file_name,"a3_shader");

		////specifying the textures
		// OpenGLTextureLibrary::Instance()->Add_Texture_From_File("earth_albedo.png", "albedo");		////TODO (Step 4): use a different texture color image here for your own mesh!
		// OpenGLTextureLibrary::Instance()->Add_Texture_From_File("earth_normal.png", "normal");		////TODO (Step 4): use a different texture normal image here for your own mesh!

		///Uncomment for Step4///
		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("car_albedo.jpg", "albedo_c");		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("car_normal.jpg", "normal_c");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tires_albedo.jpg", "albedo_t");		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tires_normal.jpg", "normal_t");		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("garage_albedo.jpg", "albedo_g");		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("garage_normal.jpg", "normal_g");		


		////bind the shader with each mesh object in the object array
		// for(auto& mesh_obj: mesh_object_array){
		// 	mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shader"));
		// 	mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("albedo"));
		// 	mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("normal"));
		// 	Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		// 	Set_Shading_Mode(mesh_obj,ShadingMode::Texture);
		// 	mesh_obj->Set_Data_Refreshed();
		// 	mesh_obj->Initialize();	
		// }

		///Uncomment for Step 4///

		int i = 0;
		for(auto& mesh_obj: mesh_object_array){
			if (i==0){
				mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shader"));
				mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("albedo_c"));
				mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("normal_c"));
				Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
				Set_Shading_Mode(mesh_obj,ShadingMode::Texture);
				mesh_obj->Set_Data_Refreshed();
				mesh_obj->Initialize();	
			}
			if (i==1){
				mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shader"));
				mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("albedo_t"));
				mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("normal_t"));
				Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
				Set_Shading_Mode(mesh_obj,ShadingMode::Texture);
				mesh_obj->Set_Data_Refreshed();
				mesh_obj->Initialize();	
			}
			if (i==2){
				mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shader"));
				mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("albedo_g"));
				mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("normal_g"));
				Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
				Set_Shading_Mode(mesh_obj,ShadingMode::Texture);
				mesh_obj->Set_Data_Refreshed();
				mesh_obj->Initialize();	
			}
			i +=1;
		}
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		for (auto& mesh_obj : mesh_object_array) {
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	ShaderDriver driver;
	driver.Initialize();
	driver.Run();	
}

#endif