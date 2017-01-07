#pragma once

#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

std::vector<int> numbers;
evt::Array<int> numbers2;
constexpr int size = 30000000;
constexpr int insertSize = 150000;

template <typename Function>
float benchmark(const Function& function) {

	auto start = high_resolution_clock::now();
	function();
	auto end = high_resolution_clock::now();
	
	return float(duration_cast<milliseconds>(end - start).count()) / float(1000);
}

float testVectorPushBack() {
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers.push_back(i);
		}
	});
}

float testArrayAppend() {
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers2.append(i);
		}
	});
}

float testVectorInsertAtBeginning() {
	
	vector<int> backup;
	numbers = move(backup); // reset the internal capacity to 0 and removes the elements
	
	return benchmark([](){
		
		for (int i = 0; i < insertSize; ++i) {
			numbers.insert(numbers.begin(), i);
		}
	});
}

float testArrayInsertAtBeginning() {
	
	numbers2.removeAll();
	
	return benchmark([](){
		
		for (int i = 0; i < insertSize; ++i) {
			numbers2.insertAt(numbers2.begin(), i);
		}
	});
}

float testVectorInsertAtEnd() {
	
	vector<int> backup;
	numbers = move(backup);
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers.insert(numbers.end(), i);
		}
	});
}


evt::Array<int> one(size);
evt::Array<int> two(size);

float testArrayAppendElements() {
	
	for (int i = 0; i < size; ++i) {
		one.append(i);
	}
	
	for (int i = 0; i < size; ++i) {
		two.append(size - i);
	}
	
	return benchmark([](){
		one += two;
	});
	
	one.removeAll();
	two.removeAll();
}

float testArrayInsertAtEnd() {
	
	numbers2.removeAll();
	
	return benchmark([](){
		
		for (int i = 0; i < size; ++i) {
			numbers2.insertAt(numbers2.end(), i);
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
