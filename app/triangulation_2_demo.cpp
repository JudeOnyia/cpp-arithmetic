////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <CGAL/Cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include "triangulation_2.hpp"

////////////////////////////////////////////////////////////////////////////////
// Global types
////////////////////////////////////////////////////////////////////////////////

// The geometry kernel to be used.
using Kernel = CGAL::Cartesian<double>;

// The triangulation type to be used.
using Triangulation = trilib::Triangulation_2<Kernel>;

////////////////////////////////////////////////////////////////////////////////
// Example code
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	Triangulation tri(std::cin);

	std::cout << "number of vertices " << tri.size_of_vertices() << '\n';
	std::cout << "number of faces " << tri.size_of_faces() << '\n';
	std::cout << "number of edges " << tri.size_of_edges() << '\n';
	std::cout << "number of halfedges " << tri.size_of_halfedges() << '\n';

	// Print the vertices in the triangulation.
	std::cout << "vertices in triangulation:\n";
	for (auto vertexIter = tri.vertices_begin(); vertexIter !=
	  tri.vertices_end(); ++vertexIter) {
		const Triangulation::Point& p = vertexIter->point();
		std::cout << "vertex " << '(' << p.x() << ',' << p.y() << ')' << '\n';
	}

	// Print the faces in the triangulation.
	std::cout << "faces in triangulation:\n";
	for (auto faceIter = tri.faces_begin(); faceIter != tri.faces_end();
	  ++faceIter) {
		Triangulation::Halfedge_const_handle h = faceIter->halfedge();
		Triangulation::Vertex_const_handle a = h->vertex();
		Triangulation::Vertex_const_handle b = h->opposite()->vertex();
		Triangulation::Vertex_const_handle c = h->next()->vertex();
		std::cout << "face " << a->point() << ' ' << b->point() << ' '
		  << c->point() << '\n';
	}

	// Print the edges in the triangulation.
	std::cout << "edges in triangulation:\n";
	for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter !=
	  tri.halfedges_end(); ++++halfedgeIter) {
		Triangulation::Vertex_const_handle a = halfedgeIter->vertex();
		Triangulation::Vertex_const_handle b =
		  halfedgeIter->opposite()->vertex();
		std::cout << "edge " << a->point() << ' ' << b->point() << '\n';
	}

	// Print the halfedges in the triangulation.
	for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter !=
	  tri.halfedges_end(); ++halfedgeIter) {
		Triangulation::Vertex_const_handle a = halfedgeIter->vertex();
		Triangulation::Vertex_const_handle b =
		  halfedgeIter->opposite()->vertex();
		std::cout << "halfedge " << a->point() << ' ' << b->point() << '\n';
	}

	// Perform some processing for each halfedge.
	for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter !=
	  tri.halfedges_end(); ++halfedgeIter) {
		Triangulation::Halfedge_const_handle h = &*halfedgeIter;
		Triangulation::Halfedge_const_handle opposite_h = h->opposite();
		// The edge member function always returns the same halfedge
		// for an edge.
		assert(h->edge() == h->opposite()->edge());

		// Perform some other sanity checks.
		assert(h->is_border() || h->next()->next() == h->prev());
		assert(h->is_border() || h->prev()->prev() == h->next());
		assert(h->opposite()->opposite() == h);
		assert(h->is_border() || h->next()->next()->next() == h);
		assert(h->is_border() || h->prev()->prev()->prev() == h);
	}

	// Output the triangulation in OFF format to standard output.
	std::cout << "Triangulation in OFF format:\n";
	tri.output_off(std::cout);
}