#include<iostream>
#include"ra/kernel.hpp"
#include <CGAL/Cartesian.h>
#include "triangulation_2.hpp"
#include<vector>

using Kernel = CGAL::Cartesian<double>;
using kernel = ra::geometry::Kernel<double>;
using Triangulation = trilib::Triangulation_2<Kernel>;
using hEit = Triangulation::Halfedge_iterator;
using point = CGAL::Cartesian<double>::Point_2;
/*
using half_edge = Triangulation::Halfedge_handle;
using vertex = Triangulation::Vertex_handle;
*/

bool is_flippable(hEit edg_iter, kernel& obj){
	if((*edg_iter).is_border_edge()){ return false; }
	else{
		point a = edg_iter->vertex()->point();
		point b = edg_iter->next()->vertex()->point();
		point c = edg_iter->opposite()->vertex()->point();
		point d = edg_iter->opposite()->next()->vertex()->point();
		return (obj.is_strictly_convex_quad(a,b,c,d));
	}
}

int main(int argc, char** argv){
	using std::cout;
	using std::endl;
	Triangulation tri(std::cin);
	kernel obj;
	int count = 0;
	for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter !=tri.halfedges_end(); ++++halfedgeIter) {
		if(is_flippable(halfedgeIter,obj)){ ++count; }
		cout<<halfedgeIter->vertex()->point()<<endl;
	}
	cout<<"Count was: "<<count<<endl;

	//std::vector<hEit> suspect_list;

	

	/*
	vertex other_quad_pt_a; vertex other_quad_pt_b;
	int num_of_edge_faces = 0;
	// Iterate over every edge
	for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter !=tri.halfedges_end(); ++++halfedgeIter) {
		vertex he_a = halfedgeIter->vertex();
		vertex he_b = halfedgeIter->opposite()->vertex();

		// Check if that edge is flippable
		// iterate over faces
		for (auto faceIter = tri.faces_begin(); faceIter != tri.faces_end(); ++faceIter) {
			half_edge h = faceIter->halfedge();
			vertex face_a = h->vertex();
			vertex face_b = h->opposite()->vertex();
			vertex face_c = h->next()->vertex();
			if( ((face_a==he_a)&&(face_b==he_b)) || ((face_a==he_b)&&(face_b==he_a)) ){
				++num_of_edge_faces;
				if(num_of_edge_faces == 1){ other_quad_pt_a = face_c; }
				if(num_of_edge_faces == 2){ other_quad_pt_b = face_c; }

			}
			else if( ((face_a==he_a)&&(face_c==he_b)) || ((face_a==he_b)&&(face_c==he_a)) ){
				++num_of_edge_faces;
				if(num_of_edge_faces == 1){ other_quad_pt_a = face_b; }
				if(num_of_edge_faces == 2){ other_quad_pt_b = face_b; }
			}
			else if( ((face_b==he_a)&&(face_c==he_b)) || ((face_b==he_b)&&(face_c==he_a)) ){
				++num_of_edge_faces;
				if(num_of_edge_faces == 1){ other_quad_pt_a = face_a; }
				if(num_of_edge_faces == 2){ other_quad_pt_b = face_a; }
			}
			else{}
		}
		// If edge is attached to 2 faces, check if quad is convex (if edge is flippable)
		if(num_of_edge_faces == 2){
			if(obj.is_strictly_convex_quad((he_a.point()),(other_quad_pt_a.point()),(he_b.point()),(other_quad_pt_b.point()))){
				// It is flippable
			}
		}
		num_of_edge_faces = 0;
		
	}
	*/
}




/*
OFF
5 4 0
-1 -1 0
1 -1 0
1 1 0
-1 1 0
0 0 1
3 0 1 4
3 1 2 4
3 2 3 4
3 0 4 3
*/


/*
OFF
9 8 0
-1 -1 0
0 -1 0
1 -1 0
1 0 0
1 1 0
0 1 0
-1 1 0
-1 0 0
0 0 0
3 0 1 8
3 0 8 7
3 1 2 3
3 1 3 8
3 3 4 8
3 8 4 5
3 8 5 7
3 7 5 6
 */
