#include <iostream>
#include <utility>
#include <type_traits>
#include <ra/interval.hpp>

template <class T>
void test()
{
	namespace ri = ra::math;
	using interval = typename ri::interval<T>;

	sizeof(ri::indeterminate_result);
	sizeof(typename interval::real_type);
	sizeof(typename interval::statistics);
	sizeof(interval::statistics::indeterminate_result_count);
	sizeof(interval::statistics::arithmetic_op_count);

	typename interval::statistics stats;
	interval::clear_statistics();
	interval::get_statistics(stats);
	stats.indeterminate_result_count = 0;
	stats.arithmetic_op_count = 0;

	{
		const interval a(T(1.0), T(2.0));
		const interval b(T(3.0), T(4.0));
		interval x(T(1.0), T(2.0));
		interval y(T(1.0));
		interval z;
		a.lower();
		static_assert(std::is_same_v<decltype(a.lower()),
		  typename interval::real_type>);
		a.upper();
		static_assert(std::is_same_v<decltype(a.upper()),
		  typename interval::real_type>);
		a.is_singleton();
		static_assert(std::is_same_v<decltype(a.is_singleton()), bool>);
		a.sign();
		static_assert(std::is_same_v<decltype(a.sign()), int>);
		a + a;
		static_assert(std::is_same_v<decltype(a + a), interval>);
		a - a;
		static_assert(std::is_same_v<decltype(a - a), interval>);
		a * a;
		static_assert(std::is_same_v<decltype(a * a), interval>);
		x += a;
		static_assert(std::is_same_v<decltype(x += a), interval&>);
		x -= a;
		static_assert(std::is_same_v<decltype(x -= a), interval&>);
		x *= a;
		static_assert(std::is_same_v<decltype(x *= a), interval&>);
		a < b;
		static_assert(std::is_same_v<decltype(a < b), bool>);
		std::cout << a << '\n';
		static_assert(std::is_same_v<decltype(std::cout << a),
		  std::ostream&>);
	}
	{
		const interval a(T(1.0));
		interval b(a);
		interval c(std::move(b));
		b = std::move(c);
		static_assert(std::is_same_v<decltype(b = std::move(c)),
		  interval&>);
		c = a;
		static_assert(std::is_same_v<decltype(c = a), interval&>);
	}
}

int main()
{
	test<float>();
	test<double>();
	test<long double>();
}
