#include <type_traits>
#include <iostream>
#include <ra/kernel.hpp>

template <class T>
void test()
{
	namespace rg = ra::geometry;

	using Real = T;
	using Kernel = typename rg::Kernel<Real>;

	static_assert(std::is_same_v<typename Kernel::Real, T>);
	static_assert(std::is_same_v<typename Kernel::Point,
	  typename CGAL::Cartesian<T>::Point_2>);
	static_assert(std::is_same_v<typename Kernel::Vector,
	  typename CGAL::Cartesian<T>::Vector_2>);

	sizeof(typename Kernel::Orientation);
	sizeof(typename Kernel::Oriented_side);
	sizeof(typename Kernel::Statistics);

	Kernel::Orientation::right_turn;
	Kernel::Orientation::collinear;
	Kernel::Orientation::left_turn;

	Kernel::Oriented_side::on_negative_side;
	Kernel::Oriented_side::on_boundary;
	Kernel::Oriented_side::on_positive_side;

	typename Kernel::Statistics stats;
	stats.orientation_total_count = 0;
	stats.orientation_exact_count = 0;
	stats.preferred_direction_total_count = 0;
	stats.preferred_direction_exact_count = 0;
	stats.side_of_oriented_circle_total_count = 0;
	stats.side_of_oriented_circle_exact_count = 0;

	const typename Kernel::Point a(Real(0), Real(0));
	const typename Kernel::Point b(Real(1), Real(0));
	const typename Kernel::Point c(Real(1), Real(1));
	const typename Kernel::Point d(Real(0), Real(1));
	const typename Kernel::Vector u(Real(1), Real(0));
	const typename Kernel::Vector v(Real(1), Real(1));


	Kernel k;

	k.orientation(a, b, c);
	static_assert(std::is_same_v<decltype(k.orientation(a, b, c)),
	  typename Kernel::Orientation>);

	k.side_of_oriented_circle(a, b, c, d);
	static_assert(std::is_same_v<
	  decltype(k.side_of_oriented_circle(a, b, c, d)),
	  typename Kernel::Oriented_side>);

	k.preferred_direction(a, c, b, d, u);
	static_assert(std::is_same_v<decltype(
	  k.preferred_direction(a, c, b, d, u)), int>);

	k.is_strictly_convex_quad(a, b, c, d);
	static_assert(std::is_same_v<decltype(
	  k.is_strictly_convex_quad(a, b, c, d)), bool>);

	k.is_locally_delaunay_edge(a, b, c, d);
	static_assert(std::is_same_v<decltype(
	  k.is_locally_delaunay_edge(a, b, c, d)), bool>);

	k.is_locally_pd_delaunay_edge(a, b, c, d, u, v);
	static_assert(std::is_same_v<decltype(
	  k.is_locally_pd_delaunay_edge(a, b, c, d, u, v)), bool>);

	Kernel::clear_statistics();
	static_assert(std::is_same_v<decltype(Kernel::clear_statistics()),
	  void>);

	Kernel::get_statistics(stats);
	static_assert(std::is_same_v<decltype(Kernel::get_statistics(stats)),
	  void>);

	{
		const Kernel a;
		Kernel b(a);
		Kernel c(std::move(b));
		b = std::move(c);
		static_assert(std::is_same_v<decltype(b = std::move(c)),
		  Kernel&>);
		c = a;
		static_assert(std::is_same_v<decltype(c = a), Kernel&>);
	}
}

int main()
{
	test<float>();
	test<double>();
	test<long double>();
}
