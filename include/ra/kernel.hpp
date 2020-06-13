#ifndef KERNEL_HPP
#define KERNEL_HPP
#include <CGAL/Cartesian.h>
#include <cstddef>
#include"ra/interval.hpp"

namespace ra::geometry {
	// A geometry kernel with robust predicates.
	template<class R>
	class Kernel {
		public:
			// The type used to represent real numbers.
			using Real = R;
			
			// The type used to represent points in two dimensions.
			using Point =typename CGAL::Cartesian<R>::Point_2;
			
			// The type used to represent vectors in two dimensions.
			using Vector = typename CGAL::Cartesian<R>::Vector_2;

			// Type used for interval class
			using itv = typename ra::math::interval<R>;
			// Type used for indeterminate_result class
			using idr = typename ra::math::indeterminate_result;
			
			// The possible outcomes of an orientation test.
			enum class Orientation : int {
				right_turn = -1,
				collinear = 0,
				left_turn = 1,
			};

			// The possible outcomes of an oriented-side-of test.
			enum class Oriented_side : int {
				on_negative_side = -1,
				on_boundary = 0,
				on_positive_side = 1,
			};

			// The set of statistics maintained by the kernel.
			struct Statistics {
				// The total number of orientation tests.
				std::size_t orientation_total_count;
				// The number of orientation tests requiring exact arithmetic
				std::size_t orientation_exact_count;
				// The total number of preferred-direction tests.
				std::size_t preferred_direction_total_count;
				// The number of preferred-direction tests requiring exact arithmetic
				std::size_t preferred_direction_exact_count;
				// The total number of side-of-oriented-circle tests.
				std::size_t side_of_oriented_circle_total_count;
				// The number of side-of-oriented-circle tests requiring exact arithmetic
				std::size_t side_of_oriented_circle_exact_count;
			};

			// Since a kernel object is stateless, construction and destruction are trivial
			Kernel() = default;
			~Kernel() = default;

			// The kernel type is both movable and copyable.
			// Since a kernel object is stateless, a copy/move operation is trivial
			Kernel(const Kernel&) = default;
			Kernel& operator=(const Kernel&) = default;
			Kernel (Kernel&&) = default;
			Kernel& operator=(Kernel&&) = default;

			// Member function to compute 3 by 3 determinant
			template<class DR>
			DR determinant_3_by_3(const DR (&m)[3][3]) const {
				DR det = ( (m[0][0]*m[1][1]*m[2][2])+(m[0][1]*m[1][2]*m[2][0])+(m[0][2]*m[1][0]*m[2][1]) ) - ( (m[2][0]*m[1][1]*m[0][2])+(m[2][1]*m[1][2]*m[0][0])+(m[2][2]*m[1][0]*m[0][1]) );
				return det;
			}
			
			// Determines how the point c is positioned relative to the
			// directed line through the points a and b (in that order).
			// Precondition: The points a and b have distinct values.
			Orientation orientation(const Point& a, const Point& b, const Point& c) const {
				try{
					++(stat_.orientation_total_count);
					itv ax(a.x()); itv ay(a.y()); itv bx(b.x()); itv by(b.y()); itv cx(c.x()); itv cy(c.y());
					itv matrix[3][3] = {ax,bx,cx,ay,by,cy,itv(1.0),itv(1.0),itv(1.0)};
					itv det = determinant_3_by_3(matrix);
					if( (det.sign()) == -1 ) { return (Orientation::right_turn); }
					else if( (det.sign()) == 1 ) { return (Orientation::left_turn); }
					else{ return (Orientation::collinear); }
				}
				catch(const idr& e){
					++(stat_.orientation_exact_count);
					Real matrix[3][3] = {(a.x()),(b.x()),(c.x()),(a.y()),(b.y()),(c.y()),Real(1.0),Real(1.0),Real(1.0)};
					Real det = determinant_3_by_3(matrix);
					if(det < Real(0)){ return (Orientation::right_turn); }
					else if(det > Real(0)){ return (Orientation::left_turn); }
					else{ return (Orientation::collinear); }
				}
			}

		private:
			static Statistics stat_;

	};

	template<class Real>
	typename Kernel<Real>::Statistics Kernel<Real>::stat_ = {.orientation_total_count = 0, .orientation_exact_count = 0, .preferred_direction_total_count = 0, .preferred_direction_exact_count = 0, .side_of_oriented_circle_total_count = 0, .side_of_oriented_circle_exact_count = 0};
	


}
#endif
