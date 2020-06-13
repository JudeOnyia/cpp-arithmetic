#include <CGAL/Cartesian.h>
#include"ra/kernel.hpp"
#include<iostream>


int main(){
	using std::cout;
	using std::endl;
	using knlD = typename ra::geometry::Kernel<double>;
	using ort = typename ra::geometry::Kernel<double>::Orientation;
	using point = typename CGAL::Cartesian<double>::Point_2;

	knlD obj_A;
	point a(0,0); point b(2,2); point c(2,0); point d(1,1); point e(0,2);
	// Test orientation
	cout<<"Test for right(true): "<<((obj_A.orientation(a,b,c)) == (ort::right_turn))<<endl;
	cout<<"Test for collinear(true): "<<((obj_A.orientation(a,b,d)) == (ort::collinear))<<endl;
	cout<<"Test for left(true): "<<((obj_A.orientation(a,b,e)) == (ort::left_turn))<<endl;



}
