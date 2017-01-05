#pragma once

#include <vector>
#include <chrono>

using namespace chrono;
using namespace std;

std::vector<int> numbers;
evt::Array<int> numbers2;
constexpr int size = 30000000;

template <typename Function>
float benchmark(const Function& function) {

	auto start = high_resolution_clock::now();
	function();
	auto end = high_resolution_clock::now();
	
	return float(duration_cast<milliseconds>(end - start).count()) / float(1000);
}

float testVectorInsertion() {
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers.push_back(i);
		}
	});
}

float testArrayInsertion() {
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers2.append(i);
		}
	});
}

float testVectorRemoval() {
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers.pop_back();
		}
	});
}

float testArrayRemoval() {
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers2.removeLast();
		}
	});
}
