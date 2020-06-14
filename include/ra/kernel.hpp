#ifndef KERNEL_HPP
#define KERNEL_HPP
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
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
			// Type used for exact arithmetic
			using exct = typename CGAL::MP_Float; 
			
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
			static DR determinant_3_by_3(const DR (&m)[3][3]) {
				DR det = ( (m[0][0]*m[1][1]*m[2][2])+(m[0][1]*m[1][2]*m[2][0])+(m[0][2]*m[1][0]*m[2][1]) ) - ( (m[2][0]*m[1][1]*m[0][2])+(m[2][1]*m[1][2]*m[0][0])+(m[2][2]*m[1][0]*m[0][1]) );
				return det;
			}

			// Member function to compute 4 by 4 determinant
			template<class DDR>
			static DDR determinant_4_by_4(const DDR (&m)[4][4]) {
				DDR sub0[3][3] = {m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3]};
				DDR sub1[3][3] = {m[0][1],m[0][2],m[0][3],m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3]};
				DDR sub2[3][3] = {m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3],m[3][1],m[3][2],m[3][3]};
				DDR sub3[3][3] = {m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3]};
				DDR det = (m[0][0]*determinant_3_by_3(sub0)) - (m[1][0]*determinant_3_by_3(sub1)) + (m[2][0]*determinant_3_by_3(sub2)) - (m[3][0]*determinant_3_by_3(sub3));
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
					exct matrix[3][3] = {(a.x()),(b.x()),(c.x()),(a.y()),(b.y()),(c.y()),exct(1.0),exct(1.0),exct(1.0)};
					exct det = determinant_3_by_3(matrix);
					if(det < exct(0)){ return (Orientation::right_turn); }
					else if(det > exct(0)){ return (Orientation::left_turn); }
					else{ return (Orientation::collinear); }
				}
			}

			// Determines how the point d is positioned relative to the
			// oriented circle passing through the points a, b, and c
			// (in that order).
			// Precondition: The points a, b, and c are not collinear.
			Oriented_side side_of_oriented_circle(const Point& a, const Point& b, const Point& c, const Point& d)const{
				try{
					++(stat_.side_of_oriented_circle_total_count);
					itv ax(a.x()); itv ay(a.y()); itv bx(b.x()); itv by(b.y()); itv cx(c.x()); itv cy(c.y());
					itv dx(d.x()); itv dy(d.y());
					itv ar((ax*ax)+(ay*ay)); itv br((bx*bx)+(by*by)); itv cr((cx*cx)+(cy*cy));
					itv dr((dx*dx)+(dy*dy));
					itv matrix[4][4] = {ax,bx,cx,dx,ay,by,cy,dy,ar,br,cr,dr,itv(1.0),itv(1.0),itv(1.0),itv(1.0)};
					itv det = determinant_4_by_4(matrix);
					if( (det.sign()) == -1 ) { return (Oriented_side::on_negative_side); }
					else if( (det.sign()) == 1 ) { return (Oriented_side::on_positive_side); }
					else{ return (Oriented_side::on_boundary); }
				}
				catch(const idr& e){
					++(stat_.side_of_oriented_circle_exact_count);
					exct ax(a.x()); exct ay(a.y()); exct bx(b.x()); exct by(b.y()); 
					exct cx(c.x()); exct cy(c.y());
					exct dx(d.x()); exct dy(d.y());
					exct ar((ax*ax)+(ay*ay)); exct br((bx*bx)+(by*by)); exct cr((cx*cx)+(cy*cy));
					exct dr((dx*dx)+(dy*dy));
					exct matrix[4][4] = {ax,bx,cx,dx,ay,by,cy,dy,ar,br,cr,dr,exct(1.0),exct(1.0),exct(1.0),exct(1.0)};
					exct det = determinant_4_by_4(matrix);
					if(det < exct(0)){ return (Oriented_side::on_negative_side); }
					else if(det > exct(0)){ return (Oriented_side::on_positive_side); }
					else{ return (Oriented_side::on_boundary); }
				}
			}

			// Determines if, compared to the orientation of line
			// segment cd, the orientation of the line segment ab is
			// more close, equally close, or less close to the
			// orientation of the vector v.
			// The value returned is 1, 0, or -1 if, compared to the
			// orientation of cd, the orientation of ab is more close,
			// equally close, or less close to the orientation of v,
			// respectively.
			// Precondition: The points a and b have distinct values; the
			// points c and d have distinct values; the vector v is not
			// the zero vector.
			int preferred_direction(const Point& a,const Point& b,const Point& c,const Point& d,const Vector& v)const{
				try{
					++(stat_.preferred_direction_total_count);
					itv ax(a.x()); itv ay(a.y()); itv bx(b.x()); itv by(b.y());
					itv cx(c.x()); itv cy(c.y()); itv dx(d.x()); itv dy(d.y());
					itv vx(v.x()); itv vy(v.y());
					itv first = ( (dx-cx)*(dx-cx) ) + ( (dy-cy)*(dy-cy) );
					itv second_inc = ( (bx-ax)*(vx) ) + ( (by-ay)*(vy) );
					itv second = second_inc * second_inc;
					itv third = ( (bx-ax)*(bx-ax) ) + ( (by-ay)*(by-ay) );
					itv fourth_inc = ( (dx-cx)*(vx) ) + ( (dy-cy)*(vy) );
					itv fourth = fourth_inc * fourth_inc;
					itv result = (first * second) - (third * fourth);
					return (result.sign());
				}
				catch(const idr& e){
					++(stat_.preferred_direction_exact_count);
					exct ax(a.x()); exct ay(a.y()); exct bx(b.x()); exct by(b.y());
					exct cx(c.x()); exct cy(c.y()); exct dx(d.x()); exct dy(d.y());
					exct vx(v.x()); exct vy(v.y());
					exct first = ( (dx-cx)*(dx-cx) ) + ( (dy-cy)*(dy-cy) );
					exct second_inc = ( (bx-ax)*(vx) ) + ( (by-ay)*(vy) );
					exct second = second_inc * second_inc;
					exct third = ( (bx-ax)*(bx-ax) ) + ( (by-ay)*(by-ay) );
					exct fourth_inc = ( (dx-cx)*(vx) ) + ( (dy-cy)*(vy) );
					exct fourth = fourth_inc * fourth_inc;
					exct result = (first * second) - (third * fourth);
					if( result < exct(0) ) { return -1; }
					else if( result > exct(0) ) { return 1; }
					else { return 0; }
				}
			}

			// Tests if the quadrilateral with vertices a, b, c, and d
			// specified in CCW order is strictly convex.
			// Precondition: The vertices a, b, c, and d have distinct
			// values and are specified in CCW order.
			bool is_strictly_convex_quad(const Point& a,const Point& b,const Point& c,const Point& d) const {
				Orientation t_1 = orientation(a,b,c);
				Orientation t_2 = orientation(b,c,d);
				Orientation t_3 = orientation(c,d,a);
				Orientation t_4 = orientation(d,a,b);
				Orientation left = Orientation::left_turn;
				if( (t_1==left) && (t_2==left) && (t_3==left) && (t_4==left) ){ return true; }
				else{ return false; }
			}

		private:
			static Statistics stat_;

	};

	template<class Real>
	typename Kernel<Real>::Statistics Kernel<Real>::stat_ = {.orientation_total_count = 0, .orientation_exact_count = 0, .preferred_direction_total_count = 0, .preferred_direction_exact_count = 0, .side_of_oriented_circle_total_count = 0, .side_of_oriented_circle_exact_count = 0};
	


}
#endif
