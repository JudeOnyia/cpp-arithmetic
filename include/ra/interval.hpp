#ifndef INTERVAL_HPP
#define INTERVAL_HPP
#include<cfenv>
#include<stdexcept>
#include<algorithm>
namespace ra::math {
	#pragma STDC FENV_ACCESS ON
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
				unsigned long indeterminate_result_count = 0;
				// The total number of interval arithmetic operations.
				unsigned long arithmetic_op_count = 0;
			};

			interval(real_type set_value = real_type(0)) : lower_(set_value), upper_(set_value) {}
			interval(real_type lower, real_type upper) : lower_(lower), upper_(upper) {
				if(lower > upper){ lower_=upper; upper_=lower; }
			}
			real_type lower() const { return lower_; }
			real_type upper() const { return upper_; }
			interval& operator+=(const interval& obj){
				real_type upper_Temp(0);
				real_type lower_Temp(0);
				++(stat_.arithmetic_op_count);
				rounding_mode_saver rms; // Save the rounding mode that should be restored
				std::fesetround(FE_DOWNWARD);
				lower_Temp = lower_ + obj.lower();
				std::fesetround(FE_UPWARD);
				upper_Temp = upper_ + obj.upper();
				lower_ = lower_Temp;
				upper_ = upper_Temp;
				return *this;
			}
			interval& operator-=(const interval& obj){
				real_type upper_Temp(0);
				real_type lower_Temp(0);
				++(stat_.arithmetic_op_count);
				rounding_mode_saver rms; // Save the rounding mode that should be restored
				std::fesetround(FE_DOWNWARD);
				lower_Temp = lower_ - obj.upper();
				std::fesetround(FE_UPWARD);
				upper_Temp = upper_ - obj.lower();
				lower_ = lower_Temp;
				upper_ = upper_Temp;
				return *this;
			}
			interval& operator*=(const interval& obj){
				real_type upper_Temp(0);
				real_type lower_Temp(0);
				++(stat_.arithmetic_op_count);
				rounding_mode_saver rms; // Save the rounding mode that should be restored
				std::fesetround(FE_DOWNWARD);
				lower_Temp = std::min(std::min((lower_*obj.lower()),(lower_*obj.upper())), std::min((upper_*obj.lower()), (upper_*obj.upper())));
				std::fesetround(FE_UPWARD);
				upper_Temp = std::max(std::max((lower_*obj.lower()),(lower_*obj.upper())), std::max((upper_*obj.lower()), (upper_*obj.upper())));
				lower_ = lower_Temp;
				upper_ = upper_Temp;
				return *this;
			}
			
			bool is_singleton() const { return (lower_==upper_)? true : false; }

			int sign() const {
				if((lower_ < real_type(0)) && (upper_ < real_type(0))){
					return -1;
				}
				else if((lower_ > real_type(0)) && (upper_ > real_type(0))){
					return 1;
				}
				else if((lower_ == real_type(0)) && (upper_ == real_type(0))){
					return 0;
				}
				else{
					throw indeterminate_result("Indeterminate result");
				}
			}
		private:
			real_type upper_;
			real_type lower_;
			statistics stat_;
			
	};


}
#endif
