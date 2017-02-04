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
float benchmark(const Function& function) {

	auto start = high_resolution_clock::now();
	function();
	auto end = high_resolution_clock::now();
	
	return float(duration_cast<milliseconds>(end - start).count()) / float(1000);
}

float testVectorPushBack() {
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers.push_back(i);
		}
	});
}

float testVectorEmplaceBack() {
	
	vector<int> backup;
	numbers = move(backup); // Reset the internal capacity to 0 and remove the elements
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers.emplace_back(i);
		}
	});
}

float testArrayAppend() {
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers2.append(i);
		}
	});
}

float testVectorInsertAtBeginning() {
	
	vector<int> backup;
	numbers = move(backup);
	
	return benchmark([] {
		
		for (int i = 0; i < insertSize; ++i) {
			numbers.insert(numbers.begin(), i);
		}
	});
}

float testArrayInsertAtBeginning() {
	
	numbers2.removeAll();
	
	return benchmark([] {
		
		for (int i = 0; i < insertSize; ++i) {
			numbers2.insertAt(numbers2.begin(), i);
		}
	});
}

float testVectorInsertAtEnd() {
	
	vector<int> backup;
	numbers = move(backup);
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers.insert(numbers.end(), i);
		}
	});
}

float testArrayInsertAtEnd() {
	
	numbers2.removeAll();
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			numbers2.insertAt(numbers2.end(), i);
		}
	});
}

size_t middleSize = 250000;

float testVectorInsertInMiddle() {
	
	vector<int> backup;
	numbers = move(backup);
	
	return benchmark([] {
		
		for (int i = 0; i < middleSize; ++i) {
			auto index = numbers.begin() + (numbers.size() / 2);
			numbers.insert(index, i);
		}
	});
}

float testArrayInsertInMiddle() {
	
	numbers2.removeAll();
	
	return benchmark([] {
		
		for (int i = 0; i < middleSize; ++i) {
			size_t index = numbers2.count() / 2;
			numbers2.insert(i, index);
		}
	});
}

evt::Array<int> one(aSize);
evt::Array<int> two(aSize);

float testArrayAppendElements() {
	
	for (int i = 0; i < aSize; ++i) {
		one.append(i);
	}
	
	for (int i = 0; i < aSize; ++i) {
		two.append(aSize - i);
	}
	
	return benchmark([](){
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
		
		for (int i = 0; i < insertSize; ++i) {
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
	});
}

float testFindElement() {
	
	return benchmark([] {
		
		for (int i = 0; i < 30000; ++i) {
			numbers2.contains(i);
		}
	});
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
		
		for (int i = 0; i < aSize; ++i) {
			test1.push_back(Test(i, double(i), std::to_string(i)));
		}
	});
}

float testVectorEmplaceBackTEST() {
	
	vector<int> backup;
	numbers = move(backup);
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			test1.emplace_back(Test(i, double(i), std::to_string(i)));
		}
	});
}

float testArrayAppendTEST() {
	
	return benchmark([] {
		
		for (int i = 0; i < aSize; ++i) {
			test2.append(Test(i, double(i), std::to_string(i)));
		}
	});
}
