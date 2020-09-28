# cpp-arithmetic
Several implementations are involved in this project:

1)  an interval class template called interval will be developed for performing interval arithmetic. Interval
arithmetic is useful in many applications where error bounds must be maintained on numerical calculations

2)  the code for a geometry kernel with robust geometric predicates is developed. This code consists of a class
template called Kernel. The functionality of the geometry kernel is encapsulated by a class template called Kernel.
The Kernel class template has one template parameter R, which is the real-number type to be used (e.g., for
coordinates of points, components of vectors, and so on).

3)  a program called delaunay_triangulation is developed that computes the preferred-directions Delaunay
triangulation of a set P of points, starting from an arbitrary triangulation of P. This is achieved by starting
with the given triangulation of P and then applying the Lawson local optimization procedure (LOP) until the
preferred-directions Delaunay triangulation is obtained. The program reads a triangulation from standard
input in OFF format. Then, the LOP should be applied to the triangulation to obtain the preferred-directions Delaunay
triangulation. The first and second preferred directions for the preferred-directions test are chosen as (1,0) and
(1,1), respectively. Finally, the resulting triangulation is written to standard output in OFF format.
