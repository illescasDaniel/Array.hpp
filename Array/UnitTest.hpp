//
//  UnitTest.hpp
//
//  Created by Daniel Illescas Romero on 05/12/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

#pragma once

#include <string>
#include <iostream>

namespace evt {
	
	class UnitTest {

		std::string testName = "";
		
	public:
		
		UnitTest(const std::string& testName): testName(testName) { }
		
		void assert(bool condition, const std::string& errorMessage = "") {
			if (!condition) {
				if (errorMessage.empty() && this->testName.empty()) {
					std::cerr << "✗ TEST FAILED" << std::endl;
				}
				else if (errorMessage.empty()) {
					std::cerr << "✗ TEST FAILED: " << this->testName << std::endl;
				} else if (this->testName.empty()) {
					std::cout << "✗ TEST FAILED: \"" << errorMessage << "\"" << std::endl;
				} else {
					std::cout << "✗ TEST FAILED (\"" << this->testName << "\"): \"" << errorMessage << "\"" << std::endl;
				}
				exit(1);
			}
		}
		
		~UnitTest() {
			xTestPassed(this->testName);
		}
		
		static void testPassed(const std::string& testName = "") {
			if (testName.empty()) {
				std::cout << "\n✓ All TESTS PASSED" << std::endl;
			}
			else {
				std::cout << "\n✓ ALL TESTS PASSED [\"" << testName << "\"]\n" << std::endl;
			}
		}
		
		//
		
		static void xAssert(bool condition, const std::string& errorMessage = "", bool displayPassedTest = false) {
			if (!condition) {
				xTestFailed(errorMessage);
				exit(1);
			} else if (displayPassedTest) {
				xTestPassed();
			}
		}
		
		static void xTestPassed(const std::string& testName = "") {
			if (testName.empty()) {
				std::cout << "✓ TEST PASSED" << std::endl;
			} else {
				std::cout << "✓ TEST PASSED: \"" << testName << "\"" << std::endl;
			}
		}
		
		static void xTestFailed(const std::string& errorMessage = "") {
			if (errorMessage.empty()) {
				std::cerr << "✗ TEST FAILED" << std::endl;
			} else {
				std::cout << "✗ TEST FAILED: \"" << errorMessage << "\"" << std::endl;
			}
		}
	};
}
