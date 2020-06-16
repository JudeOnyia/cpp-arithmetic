#include<iostream>
#include"ra/kernel.hpp"
#include <CGAL/Cartesian.h>
#include "triangulation_2.hpp"
#include<vector>

using Kernel = CGAL::Cartesian<double>;
using kernel = ra::geometry::Kernel<double>;
using Triangulation = trilib::Triangulation_2<Kernel>;
using hEit = Triangulation::Halfedge_iterator;
using hE_hand = Triangulation::Halfedge_handle;
using point = CGAL::Cartesian<double>::Point_2;
using vector = CGAL::Cartesian<double>::Vector_2;


bool is_flippable(hE_hand edg_iter, kernel& obj){
	if(edg_iter->is_border_edge()){ return false; }
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
	hE_hand tmp_hand;

	
	std::vector<hE_hand> suspect_list;
	for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter !=tri.halfedges_end(); ++++halfedgeIter) {
		tmp_hand = &*halfedgeIter;
		if(is_flippable(tmp_hand,obj)){
			suspect_list.push_back(tmp_hand);
		}
	}

	while(!(suspect_list.empty())){
		hE_hand  it = suspect_list.back();
		if(is_flippable(it,obj)){
			point c = it->vertex()->point();
			point d = it->next()->vertex()->point();
			point a = it->opposite()->vertex()->point();
			point b = it->opposite()->next()->vertex()->point();
			if(!(obj.is_locally_pd_delaunay_edge(a,b,c,d,vector(1,0),vector(1,1)))){
				it = tri.flip_edge(it);
				suspect_list.push_back(it->next());
				suspect_list.push_back(it->next()->next());
				suspect_list.push_back(it->opposite()->next());
				suspect_list.push_back(it->opposite()->next()->next());
			}
		}
		suspect_list.pop_back();
	}

	tri.output_off(cout);
	
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
3 0 1 7
3 1 8 7
3 1 2 8
3 2 3 8
3 3 4 5
3 3 5 8
3 5 6 8
3 8 6 7
*/
