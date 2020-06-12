#include"ra/interval.hpp"
#include<iostream>

int main(){
	using itvf = typename ra::math::interval<float>;
	using std::cout;
	using std::endl;

	// Test default constructor
	itvf obj_A;
	cout<<"obj_A: "<<(obj_A.lower())<<" "<<(obj_A.upper())<<endl;
	// Test 2 value constructor
	itvf obj_B(2.89, 2.91);
	itvf obj_C(3.91,3.89);
	cout<<"obj_B: "<<(obj_B.lower())<<" "<<(obj_B.upper())<<endl;
	cout<<"obj_C: "<<(obj_C.lower())<<" "<<(obj_C.upper())<<endl;
	// Test Compound add, sub, mult
	cout<<"obj_C+=obj_B: "<<((obj_C+=obj_B).lower())<<" "<<(obj_C.upper())<<endl;
	cout<<"obj_C-=obj_B: "<<((obj_C-=obj_B).lower())<<" "<<(obj_C.upper())<<endl;
	cout<<"obj_C*=obj_B: "<<((obj_C*=obj_B).lower())<<" "<<(obj_C.upper())<<endl;
	// Test Aliasing of Compound add, sub, mult
	cout<<"obj_B+=obj_B: "<<((obj_B+=obj_B).lower())<<" "<<(obj_B.upper())<<endl;
	cout<<"obj_B-=obj_B: "<<((obj_B-=obj_B).lower())<<" "<<(obj_B.upper())<<endl;
	cout<<"obj_B*=obj_B: "<<((obj_B*=obj_B).lower())<<" "<<(obj_B.upper())<<endl;
	// Test is_singleton
	itvf obj_D(1.7);
	cout<<"obj_D.is_singleton() must be true: "<<(obj_D.is_singleton())<<endl;
	cout<<"obj_C.is_singleton() must be false: "<<(obj_C.is_singleton())<<endl;
	// Test sign()
	itvf obj_E(-7.335,-7.339);
	itvf obj_F(3.442,3.448);
	itvf obj_G(-0.04,0.12);
	cout<<"Neg sign: "<<(obj_E.sign())<<endl;
	cout<<"Pos sign: "<<(obj_F.sign())<<endl;
	cout<<"Zero no sign: "<<(obj_A.sign())<<endl;
	//cout<<"Indeteminate for sign: "<<(obj_G.sign())<<endl;
	

	return 0;
}
