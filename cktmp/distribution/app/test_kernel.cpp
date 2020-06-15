#include <CGAL/Cartesian.h>
#include"ra/kernel.hpp"
#include<iostream>

int main(){
	using std::cout;
	using std::endl;
	using knlD = typename ra::geometry::Kernel<double>;
	using ort = typename ra::geometry::Kernel<double>::Orientation;
	using ort_side = typename ra::geometry::Kernel<double>::Oriented_side;
	using stat = typename ra::geometry::Kernel<double>::Statistics;
	using point = typename CGAL::Cartesian<double>::Point_2;
	using vector = typename CGAL::Cartesian<double>::Vector_2;

	knlD obj_A;
	point a(0,0); point b(2,2); point c(2,0); point d(1,1); point e(0,2);
	// Test orientation
	cout<<"Test for right(true): "<<((obj_A.orientation(a,b,c)) == (ort::right_turn))<<endl;
	cout<<"Test for collinear(true): "<<((obj_A.orientation(a,b,d)) == (ort::collinear))<<endl;
	cout<<"Test for left(true): "<<((obj_A.orientation(a,b,e)) == (ort::left_turn))<<endl;
	// Test side_of_oriented_circle
	point ac(0,0); point bc(2,0); point cc(0,2); point dc(1,1); point ec(2,2); point fc(3,3);
	cout<<"Test for on_negative_side(true): "<<((obj_A.side_of_oriented_circle(ac,bc,cc,fc)) == (ort_side::on_negative_side))<<endl;
	cout<<"Test for on_boundary(true): "<<((obj_A.side_of_oriented_circle(ac,bc,cc,ec)) == (ort_side::on_boundary))<<endl;
	cout<<"Test for on_positive_side(true): "<<((obj_A.side_of_oriented_circle(ac,bc,cc,dc)) == (ort_side::on_positive_side))<<endl;
	// Test preferred_direction
	point ap(0,0); point bp(2,2); point cp(2,0); point dp(0,2);
	vector up(2,1); vector vp(1,0); vector wp(-1,2);
	cout<<"Test for more close(true): "<<((obj_A.preferred_direction(ap,bp,cp,dp,up)) == 1)<<endl;
	cout<<"Test for equally close(true): "<<((obj_A.preferred_direction(ap,bp,cp,dp,vp)) == 0)<<endl;
	cout<<"Test for less close(true): "<<((obj_A.preferred_direction(ap,bp,cp,dp,wp)) == -1)<<endl;
	// Test is_strictly_convex_quad
	point asc(0,0); point bsc(1,0); point csc(2,2); point dsc(1,2);
	point anc(0,0); point bnc(1,0); point cnc(2,0); point dnc(2,1);
	cout<<"Test for convexity(true): "<<(obj_A.is_strictly_convex_quad(asc,bsc,csc,dsc))<<endl;
	cout<<"Test for convexity(false): "<<(obj_A.is_strictly_convex_quad(anc,bnc,cnc,dnc))<<endl;
	//Test is_locally_delaunay_edge
	point ad(0,0); point bd(1,0); point cd(1,1); point dd(0,1);
	point adn(0,0); point bdn(5,0); point cdn(5,5); point ddn(2,3);
	cout<<"Test edge for locally delaunay(true): "<<(obj_A.is_locally_delaunay_edge(ad,bd,cd,dd))<<endl;
	cout<<"Test edge for locally delaunay(false): "<<(obj_A.is_locally_delaunay_edge(adn,bdn,cdn,ddn))<<endl;
	// Test is_locally_pd_delaunay_edge
	cout<<"Test edge for pref dir loc delaunay(true): "<<(obj_A.is_locally_pd_delaunay_edge(ad,bd,cd,dd,up,vp))<<endl;
	cout<<"Test edge for pref dir loc delaunay(false): "<<(obj_A.is_locally_pd_delaunay_edge(ad,bd,cd,dd,wp,vp))<<endl;
	// Test get_statistics and clear_statistics
	stat st;
	obj_A.get_statistics(st);
	cout<<"Current statistics: ort tot="<<(st.orientation_total_count)<<" ort ex="<<(st.orientation_exact_count)<<" pd tot="<<(st.preferred_direction_total_count)<<" pd ex="<<(st.preferred_direction_exact_count)<<" crc tot="<<(st.side_of_oriented_circle_total_count)<<" crc ex="<<(st.side_of_oriented_circle_exact_count)<<endl;
	obj_A.clear_statistics();
	obj_A.get_statistics(st);
	cout<<"Cleared statistics: ort tot="<<(st.orientation_total_count)<<" ort ex="<<(st.orientation_exact_count)<<" pd tot="<<(st.preferred_direction_total_count)<<" pd ex="<<(st.preferred_direction_exact_count)<<" crc tot="<<(st.side_of_oriented_circle_total_count)<<" crc ex="<<(st.side_of_oriented_circle_exact_count)<<endl;




}
