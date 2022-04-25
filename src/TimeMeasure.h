#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H

#include <string>
#include <map>
#include <ostream>
#include <iomanip>
#include <type_traits>
#include <functional>


template <class T>
	struct is_reference_wrapper : std::false_type {};
template <class U>
	struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
template <class T>
	constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;


template<std::size_t loopCount = 1,typename TimeT = std::chrono::milliseconds>
struct measure
{
	measure(){
	}
	template<typename F,typename ...Args>
	auto operator()(F&& func, Args&&... args){
		long long execDuration = 0;
		for(std::size_t i = 0; i < loopCount; ++i){
 			auto start = std::chrono::steady_clock::now();
        		//std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
			std::invoke(std::forward<decltype(func)>(func),std::forward<Args>(args)...);
			auto duration = std::chrono::duration_cast< TimeT>(std::chrono::steady_clock::now() - start);
        		execDuration += duration.count();
		}
		return execDuration/loopCount;

	}	



	template <class T, class Type, class T1, class... Args>
		decltype(auto) INVOKE(const std::string & name, Type T::* f, T1&& t1, Args&&... args)
	{
		long long execDuration = 0;
		for(std::size_t i = 0; i < loopCount; ++i){
			auto start = std::chrono::steady_clock::now();
	    		if constexpr (std::is_member_function_pointer_v<decltype(f)>) {
				if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)		    			
					(std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
				else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
		    			(t1.get().*f)(std::forward<Args>(args)...);
				else
		    			((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
	    		} else {
				static_assert(std::is_member_object_pointer_v<decltype(f)>);
				static_assert(sizeof...(args) == 0);
				if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
		    			std::forward<T1>(t1).*f;
				else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
		    			t1.get().*f;
				else
		    			(*std::forward<T1>(t1)).*f;
	    		}
			auto duration = std::chrono::duration_cast< TimeT>(std::chrono::steady_clock::now() - start);
			execDuration += duration.count();
		}
		execMap[name] = (double)(execDuration/loopCount);
	}

	template<typename F,typename ...Args>
	void method_to_compare(const std::string & name, F&& func, Args&&... args){
		long long execDuration = 0;
		for(std::size_t i = 0; i < loopCount; ++i){
 			auto start = std::chrono::steady_clock::now();
			std::invoke(std::forward<decltype(func)>(func),std::forward<Args>(args)...);
        		//std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        		auto duration = std::chrono::duration_cast< TimeT>(std::chrono::steady_clock::now() - start);
        		execDuration += duration.count();
		}
		execMap[name] = (double)(execDuration/loopCount);
	}

	


	template<typename F, typename ...Args>
	static typename TimeT::rep execution(F&& func, Args&&... args)
 	{
 		auto start = std::chrono::steady_clock::now();
        	std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        	auto duration = std::chrono::duration_cast< TimeT>(std::chrono::steady_clock::now() - start);
        	return duration.count();
    	}
	std::map<std::string,double> get_measured_results()const{
		return this->execMap;
	}
	std::string get_shortcut()const{
		if constexpr (std::is_same<TimeT,std::chrono::hours>::value){
			return "h";
		}
		if constexpr (std::is_same<TimeT,std::chrono::minutes>::value){
			return "min";
		}
		if constexpr (std::is_same<TimeT,std::chrono::seconds>::value){
			return "s";
		}
		if constexpr (std::is_same<TimeT,std::chrono::milliseconds>::value){
			return "ms";
		}
		if constexpr (std::is_same<TimeT,std::chrono::microseconds>::value){
			return "mus" ;
		}
		if constexpr (std::is_same<TimeT,std::chrono::nanoseconds>::value){
			return "ns";
		}
		
	}
private:
	std::map<std::string,double> execMap;
};





template<std::size_t loopCount = 1,typename TimeT = std::chrono::milliseconds>
std::ostream& operator<<(std::ostream & stream,const measure<loopCount,TimeT> & measure){
	stream<<std::left<<std::setw(25)<<"Method"<<"Elapsed Time" <<"\n";
	stream<<"========================================================\n";
	auto execMap = measure.get_measured_results();
	for(auto const & item : execMap){
		stream<<std::left<<std::setw(25)<<item.first<<item.second<<measure.get_shortcut()<<"\n";
	}

	return stream;
}



#endif
