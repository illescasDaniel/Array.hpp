//
//  ArrayTest.hpp
//  Array
//
//  Created by Daniel Illescas Romero on 05/12/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

#include "UnitTest.hpp"
#include "Array.hpp"
#include <vector>

class ArrayTest {
	
	// evt::Array<int> commonArray{};
	
	static void testInitialization() {
		
		evt::UnitTest initializationTest("Array - Initialization");
		
		evt::Array<size_t> numbers {0,1,2,3,4};
		
		for (size_t i = 0; i < numbers.size(); i++) {
			initializationTest.assert(numbers.contains(i), "Array doesn't contain expected value");
		}
	}
	
	static void testAppend() {
		
		evt::UnitTest appendTest("Array - Append");
		
		evt::Array<size_t> numbers;
		const size_t sizeArray = 2012344;
		
		for (size_t i = 0; i < sizeArray; i++) {
			numbers.append(i * 2);
		}
		appendTest.assert(numbers.size() == sizeArray, "Size is incorrect");
		
		for (size_t i = 0; i < sizeArray; i++) {
			appendTest.assert(numbers[i] == i*2, "Value is incorrect");
		}
	}
	
	static void testInitializationFromVector() {
		
		evt::UnitTest initializationTest("Array - Initialization from std::vector");
		
		std::vector<int> numbers0 {10,30,-543,9876,3243423};
		evt::Array<int> numbers1(numbers0);
		
		initializationTest.assert(numbers0.size() == numbers1.size(), "Sizes doesn't match");
		
		for (size_t i = 0 ; i < numbers0.size(); i++) {
			initializationTest.assert(numbers0[i] == numbers1[i], "Incorrect values");
		}
	}
	
public:
	
	static void runTests() {
		
		testInitialization();
		testAppend();
		testInitializationFromVector();
		
		evt::UnitTest::testPassed("Array");
	}
};

int main() {
	ArrayTest::runTests();
}
