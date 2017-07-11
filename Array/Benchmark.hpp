#pragma once

#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

std::vector<int> numbers;
evt::Array<int> numbers2;
constexpr int aSize = 30000000;
constexpr int insertSize = 150000;

template <typename Function>
float benchmark(const Function& function, size_t iterations = 1) {
	
	float averageTime = 0.0;
	
	for (size_t i = 0; i < iterations; ++i) {
		
		auto start = std::chrono::high_resolution_clock::now();
		function();
		auto end = std::chrono::high_resolution_clock::now();
		
		averageTime += float(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / float(1000000000);
	}
	
	return averageTime / float(iterations);
}

float testVectorPushBack() {
	
	return benchmark([] {
		
		vector<int> backup;
		numbers = move(backup);
	
		for (int i = 0; i < aSize; ++i) {
			numbers.push_back(i);
		}
	}, 20);
}

float testVectorEmplaceBack() {
	
	return benchmark([] {
		
		vector<int> backup;
		numbers = move(backup);
		
		for (int i = 0; i < aSize; ++i) {
			numbers.emplace_back(i);
		}
	}, 20);
}

float testArrayAppend() {
	
	return benchmark([] {
		
		numbers2.removeAll();
		
		for (int i = 0; i < aSize; ++i) {
			numbers2.append(i);
		}
	}, 20);
}

float testVectorInsertAtBeginning() {

	return benchmark([] {
		
		vector<int> backup;
		numbers = move(backup);
		
		for (int i = 0; i < insertSize; ++i) {
			numbers.insert(numbers.begin(), i);
		}
	}, 5);
}

float testArrayInsertAtBeginning() {
	
	return benchmark([] {
		
		numbers2.removeAll();
		
		for (int i = 0; i < insertSize; ++i) {
			numbers2.insertAt(numbers2.begin(), i);
		}
	}, 5);
}

float testVectorInsertAtEnd() {
	
	return benchmark([] {
		
		vector<int> backup;
		numbers = move(backup);
		
		for (int i = 0; i < aSize; ++i) {
			numbers.insert(numbers.end(), i);
		}
	}, 10);
}

float testArrayInsertAtEnd() {
	
	return benchmark([] {
		
		numbers2.removeAll();
		
		for (int i = 0; i < aSize; ++i) {
			numbers2.insertAt(numbers2.end(), i);
		}
	}, 10);
}

constexpr int middleSize = 250000;

float testVectorInsertInMiddle() {
	
	return benchmark([] {
		
		vector<int> backup;
		numbers = move(backup);
		
		for (int i = 0; i < middleSize; ++i) {
			auto index = numbers.begin() + (numbers.size() / 2);
			numbers.insert(index, i);
		}
	}, 5);
}

float testArrayInsertInMiddle() {
	
	return benchmark([] {
		
		numbers2.removeAll();
		
		for (int i = 0; i < middleSize; ++i) {
			size_t index = numbers2.count() / 2;
			numbers2.insert(i, index);
		}
	}, 5);
}

evt::Array<int,aSize> one;
evt::Array<int,aSize> two;

float testArrayAppendElements() {
	
	for (int i = 0; i < aSize; ++i) {
		one.append(i);
	}
	
	for (int i = 0; i < aSize; ++i) {
		two.append(aSize - i);
	}
	
	return benchmark([] {
		one += two;
	});
	
	one.removeAll();
	two.removeAll();
}

float testVectorRemoval() {
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers.pop_back();
		}
	});
}

float testArrayRemoval() {
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers2.removeLast();
		}
	});
}

float testArrayRemoveFirst() {
	
	return benchmark([] {
		
		for (size_t i = 0; i < 1000; ++i) {
			numbers2.removeFirst();
		}
	});
}

float testElementAccess() {
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers2[i];
			//(*numbers2 + i);
		}
	}, 10);
}

float testFindElement() {
	
	return benchmark([] {
		
		for (int i = 0; i < 30000; ++i) {
			numbers2.contains(i);
		}
	}, 10);
}

// TEST CLASS

class Test {
public:
	int number1;
	double number2;
	string str;
	
	Test() {}
	Test(int n1_, double n2_, string str_): number1(n1_), number2(n2_), str(str_) {}
};

std::vector<Test> test1;
evt::Array<Test> test2;

float testVectorPushBackTEST() {
	
	return benchmark([] {
		
		vector<Test> backup;
		test1 = move(backup);
		
		for (int i = 0; i < aSize; ++i) {
			test1.push_back(Test(i, double(i), std::to_string(i)));
		}
	}, 5);
}

float testVectorEmplaceBackTEST() {
	
	return benchmark([] {
		
		vector<Test> backup;
		test1 = move(backup);
		
		for (int i = 0; i < aSize; ++i) {
			test1.emplace_back(Test(i, double(i), std::to_string(i)));
		}
	}, 5);
}

float testArrayAppendTEST() {
	
	return benchmark([] {
		
		test2.removeAll();
		
		for (int i = 0; i < aSize; ++i) {
			test2.append(Test(i, double(i), std::to_string(i)));
		}
	}, 5);
}
