#ifndef INTERVAL_HPP
#define INTERVAL_HPP
#include<cfenv>
#include<stdexcept>
#include<algorithm>
#include<iostream>
namespace ra::math {

	struct indeterminate_result : public std::runtime_error{
		using std::runtime_error::runtime_error;
	};

	class rounding_mode_saver {
		public:
			rounding_mode_saver() : current_round_mode(std::fegetround()) {}
			~rounding_mode_saver() {
				std::fesetround(current_round_mode);
			}
			rounding_mode_saver(rounding_mode_saver&&) = delete;
			rounding_mode_saver(const rounding_mode_saver&) = delete;
			rounding_mode_saver& operator=(rounding_mode_saver&&) = delete;
			rounding_mode_saver& operator=(const rounding_mode_saver&) = delete;
		private:
			int current_round_mode;
	};

	template<class T>
	class interval{
		public:
			using real_type = T;

			struct statistics{
				// The total number of indeterminate results encountered.
				unsigned long indeterminate_result_count;
				// The total number of interval arithmetic operations.
				unsigned long arithmetic_op_count;
			};

			void keep_order() {
				real_type temp;
				if(lower_>upper_){
					temp = lower_;
					lower_ = upper_;
					upper_ = temp;
				}
			}

			interval(real_type set_value = real_type(0)) : lower_(set_value), upper_(set_value) {}
			interval(real_type lower, real_type upper) : lower_(lower), upper_(upper) { keep_order(); }
			real_type lower() const { return lower_; }
			real_type upper() const { return upper_; }
			
			void incr_indeterminate_result_count() { ++(stat_.indeterminate_result_count); }
			void incr_arithmetic_op_count() { ++(stat_.arithmetic_op_count); }

			interval& operator+=(const interval& obj){
				real_type upper_Temp(0);
				real_type lower_Temp(0);
				rounding_mode_saver rms; // Save the rounding mode that should be restored
				std::fesetround(FE_DOWNWARD);
				lower_Temp = lower_ + obj.lower();
				std::fesetround(FE_UPWARD);
				upper_Temp = upper_ + obj.upper();
				lower_ = lower_Temp;
				upper_ = upper_Temp;
				incr_arithmetic_op_count();
				return *this;
			}
			interval& operator-=(const interval& obj){
				real_type upper_Temp(0);
				real_type lower_Temp(0);
				rounding_mode_saver rms; // Save the rounding mode that should be restored
				std::fesetround(FE_DOWNWARD);
				lower_Temp = lower_ - obj.upper();
				std::fesetround(FE_UPWARD);
				upper_Temp = upper_ - obj.lower();
				lower_ = lower_Temp;
				upper_ = upper_Temp;
				incr_arithmetic_op_count();
				return *this;
			}
			interval& operator*=(const interval& obj){
				real_type upper_Temp(0);
				real_type lower_Temp(0);
				rounding_mode_saver rms; // Save the rounding mode that should be restored
				std::fesetround(FE_DOWNWARD);
				lower_Temp = std::min(std::min((lower_*obj.lower()),(lower_*obj.upper())), std::min((upper_*obj.lower()), (upper_*obj.upper())));
				std::fesetround(FE_UPWARD);
				upper_Temp = std::max(std::max((lower_*obj.lower()),(lower_*obj.upper())), std::max((upper_*obj.lower()), (upper_*obj.upper())));
				lower_ = lower_Temp;
				upper_ = upper_Temp;
				incr_arithmetic_op_count();
				return *this;
			}
			
			bool is_singleton() const { return (lower_==upper_)? true : false; }

			int sign() const {
				if((lower_ < real_type(0)) && (upper_ < real_type(0))){ return -1; }
				else if((lower_ > real_type(0)) && (upper_ > real_type(0))){ return 1; }
				else if((lower_ == real_type(0)) && (upper_ == real_type(0))){ return 0; }
				else{
					throw indeterminate_result("Indeterminate sign of interval");
				}
			}
			int sign() {
				if((lower_ < real_type(0)) && (upper_ < real_type(0))){ return -1; }
				else if((lower_ > real_type(0)) && (upper_ > real_type(0))){ return 1; }
				else if((lower_ == real_type(0)) && (upper_ == real_type(0))){ return 0; }
				else{
					incr_indeterminate_result_count();
					throw indeterminate_result("Indeterminate sign of interval");
				}
			}

			void clear_statistics() {
				stat_.indeterminate_result_count = 0;
				stat_.arithmetic_op_count = 0;
			}

			void get_statistics(statistics& stat) {
				stat = stat_;
			}

		private:
			real_type upper_;
			real_type lower_;
			static statistics stat_;
			
	};
	
	template<class real_type>
	typename interval<real_type>::statistics interval<real_type>::stat_ = {.indeterminate_result_count = 0, .arithmetic_op_count = 0};

	template<class real_type>
	interval<real_type> operator+(const interval<real_type>& obj_A, const interval<real_type>& obj_B){
		real_type upper_Temp(0);
		real_type lower_Temp(0);
		rounding_mode_saver rms; // Save the rounding mode that should be restored
		std::fesetround(FE_DOWNWARD);
		lower_Temp = obj_A.lower() + obj_B.lower();
		std::fesetround(FE_UPWARD);
		upper_Temp = obj_A.upper() + obj_B.upper();
		interval<real_type> result(lower_Temp,upper_Temp);
		result.incr_arithmetic_op_count();
		return result;
	}

	template<class real_type>
	interval<real_type> operator-(const interval<real_type>& obj_A, const interval<real_type>& obj_B){
		real_type upper_Temp(0);
		real_type lower_Temp(0);
		rounding_mode_saver rms; // Save the rounding mode that should be restored
		std::fesetround(FE_DOWNWARD);
		lower_Temp = obj_A.lower() - obj_B.upper();
		std::fesetround(FE_UPWARD);
		upper_Temp = obj_A.upper() - obj_B.lower();
		interval<real_type> result(lower_Temp,upper_Temp);
		result.incr_arithmetic_op_count();
		return result;
	}

	template<class real_type>
	interval<real_type> operator*(const interval<real_type>& obj_A, const interval<real_type>& obj_B){
		real_type upper_Temp(0);
		real_type lower_Temp(0);
		rounding_mode_saver rms; // Save the rounding mode that should be restored
		std::fesetround(FE_DOWNWARD);
		lower_Temp = std::min(std::min((obj_A.lower()*obj_B.lower()),(obj_A.lower()*obj_B.upper())), std::min((obj_A.upper()*obj_B.lower()), (obj_A.upper()*obj_B.upper())));
		std::fesetround(FE_UPWARD);
		upper_Temp = std::max(std::max((obj_A.lower()*obj_B.lower()),(obj_A.lower()*obj_B.upper())), std::max((obj_A.upper()*obj_B.lower()), (obj_A.upper()*obj_B.upper())));
		interval<real_type> result(lower_Temp,upper_Temp);
		result.incr_arithmetic_op_count();
		return result;
	}

	template<class real_type>
	bool operator<(const interval<real_type>& obj_A, const interval<real_type>& obj_B){
		if( (obj_A.upper()) < (obj_B.lower()) ) { return true; }
		else if( (obj_A.lower()) >= (obj_B.upper()) ) { return false; }
		else{
			interval<real_type> temp;
			temp.incr_indeterminate_result_count();
			throw indeterminate_result("Indeterminate less than operator");
		}
	}

	template<class real_type>
	std::ostream& operator<<(std::ostream& outStream, const interval<real_type>& obj){
		outStream<<"["<<(obj.lower())<<","<<(obj.upper())<<"]";
		return outStream;
	}


}
#endif
