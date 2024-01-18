#ifndef __LoopSubdivision_h__
#define __LoopSubdivision_h__
#include <unordered_map>
#include <vector>
#include "Mesh.h"
#include <iostream>

inline void LoopSubdivision(TriangleMesh<3>& mesh)
{
	std::vector<Vector3>& old_vtx=mesh.Vertices(); //xyz in space
	std::vector<Vector3i>& old_tri=mesh.Elements(); //index values directing to verts
	std::vector<Vector3> new_vtx;		////vertex array for the new mesh
	std::vector<Vector3i> new_tri;		////element array for the new mesh
	
	new_vtx=old_vtx;	////copy all the old vertices to the new_vtx array

	std::unordered_map<Vector2i,int>  edge_vtx_map; //edge (key) indices, midpoint index
	std::unordered_map<Vector2i,std::vector<int> > edge_tri_map;  
	std::unordered_map<int,std::vector<int> > vtx_vtx_map;   

	////step 1: add mid-point vertices and triangles
	////for each old triangle, 
	////add three new vertices (in the middle of each edge) to new_vtx 
	////add four new triangles to new_tri

	/*your implementation here*/
	//loop through all the tris
	for (int i = 0; i < old_tri.size(); i++){
		//get the triangle
		const Vector3i& triangle = old_tri[i];

		//make the thingy to store the midpoint triangle
		Vector3i mid_triangle;

		for (int vert = 0; vert < 3; vert++){
			Vector2i edge;
			edge[0] = triangle[vert];
			edge[1] = triangle[(vert+1)%3]; //make sure it doesn't go over 3 (wrap around to 1)
			
			if (edge[0]>edge[1]){
				int temp = edge[0];
				edge[0] = edge[1];
				edge[1] = temp;
			}

			int mid_vert_index;

			if (edge_vtx_map.find(edge) == edge_vtx_map.end()){
				//find the endpoint vertice positions
				Vector3 new_mid;
				new_mid = 0.5*(old_vtx[edge[0]] + old_vtx[edge[1]]);
				new_vtx.push_back(new_mid);
				mid_vert_index = new_vtx.size()-1;
				edge_vtx_map[edge] = mid_vert_index;
			}
			
			else{
				mid_vert_index = edge_vtx_map[edge];
			}
			mid_triangle[vert] = mid_vert_index; 
		}

		//a triangle with a new triangle cut into the middle results in four triangles: add all of them to new_tri
		new_tri.push_back(Vector3i(triangle[0], mid_triangle[0], mid_triangle[2]));
		new_tri.push_back(Vector3i(mid_triangle[0], triangle[1], mid_triangle[1]));
		new_tri.push_back(Vector3i(mid_triangle[2], mid_triangle[1], triangle[2]));
		new_tri.push_back(mid_triangle);
	}
	////step 2: update the position for each new mid-point vertex: 
	////for each mid-point vertex, find its two end-point vertices A and B, 
	////and find the two opposite-side vertices on the two incident triangles C and D,
	////then update the new position as .375*(A+B)+.125*(C+D)

	/*your implementation here*/
	for (int i = 0; i < old_tri.size(); i++){// go through each index of old_tri, relay to vector3i of a triangle
		const Vector3i& triangle = old_tri[i];

		for (int vert = 0; vert < 3; vert++){ //for every vert in a triangle
			Vector2i edge; //to store an edge
			edge[0] = triangle[vert]; // put one vert at the start of the edge
			edge[1] = triangle[(vert+1)%3]; //make sure it doesn't go over 3 (wrap around to 1) // another after, don't go over three
			//so now we have a full edge
			if (edge[0]>edge[1]){ // sort
				int temp = edge[0];
				edge[0] = edge[1];
				edge[1] = temp;
			}
			if (edge_tri_map.find(edge) == edge_tri_map.end()){
				edge_tri_map[edge] = std::vector<int>();
			}
			//index into the unordered map at edge: should get an array of ints. append i (triangle index) to the array
			edge_tri_map[edge].push_back(i); //  //
		}
	}

	//for every edge on the original triangles
	for (const auto &[edge, vtx]: edge_vtx_map){
		int mid_vert_index;
		mid_vert_index = edge_vtx_map[edge];
		

		int t0_idx, t1_idx;
		t0_idx = edge_tri_map[edge][0];
		t1_idx = edge_tri_map[edge][1];

		Vector3i t0, t1;
		t0 = old_tri[t0_idx];
		t1 = old_tri[t1_idx];

		Vector3 opp_v0;
		Vector3 opp_v1;
		// check the vertices on a triangle
		for (int vert = 0; vert < 3; vert++){
			if (t0[vert] != edge[0] && t0[vert] != edge[1]){ //if the vertex is in the triangle0 and not on the edge
				opp_v0 = new_vtx[t0[vert]];
			}
			//loop through the triangle and find the vert that isn't equal to 
			if (t1[vert] != edge[0] && t1[vert] != edge[1]){ //if the vertex is in the triangle1 and not on the edge
				opp_v1 = new_vtx[t1[vert]];
			}
		}
		// update the new position as .375*(A+B)+.125*(C+D)
		new_vtx[mid_vert_index] = 0.375*(new_vtx[edge[0]] + new_vtx[edge[1]]) + 0.125*(opp_v0 + opp_v1);
	}


	////step 3: update vertices of each old vertex
	////for each old vertex, find its incident old vertices, and update its position according its incident vertices

	/*your implementation here*/

	for (const auto &[edge, vtx]: edge_vtx_map){
		int v0, v1;
		v0 = edge[0];
		v1 = edge[1];

		if (vtx_vtx_map.find(v0) == vtx_vtx_map.end()){ // if it's not in the map already, add it and make an array stored in it
			vtx_vtx_map[v0] = std::vector<int>();
		}
		if (vtx_vtx_map.find(v1) == vtx_vtx_map.end()){
			vtx_vtx_map[v1] = std::vector<int>();
		}

		vtx_vtx_map[v0].push_back(v1); //add to the array
		vtx_vtx_map[v1].push_back(v0);
	}

	std::vector<Vector3> smoothed_vtces = new_vtx; //to store smoothed verts so that they don't interfere with each other during smoothing

	for (const auto &[vtx, vtces]: vtx_vtx_map){
		int nbs;
		nbs = vtces.size();
		
		//create beta (neighbor mult factor)
		real beta;
		if (nbs > 3){
			beta = (0.375/nbs);
		}
		else{
			beta = (0.375);
		}
		//create weighted average (factor for og)
		real weighted_avg;
		weighted_avg = 1-(beta*nbs);

		Vector3 smoothed;
		smoothed = weighted_avg * old_vtx[vtx];

		//update the smoothed central vertex with influence from the neighbors
		for (int v = 0; v<vtces.size(); v++){
			smoothed += (old_vtx[vtces[v]])*beta;
		}
		//add smoothed value to the array
		smoothed_vtces[vtx] = smoothed;
	}
	//replace new with smoothed!
	new_vtx = smoothed_vtces;

	////update subdivided vertices and triangles onto the input mesh
	old_vtx=new_vtx;
	old_tri=new_tri;
}

#endif