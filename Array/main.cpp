#include <iostream>
#include "Array.hpp"
#include "Benchmark.hpp"

using namespace std;
using namespace evt;

#if (__cplusplus >= 201406)
	#include <experimental/optional>
	using namespace experimental;
#endif

class Test2 {
public:
	int a = 0, b = 0;
	Test2() {}
	Test2(int a, int b): a(a), b(b) {}
};

int main() {
	
	Array<Test2> tests;
	tests.append(Test2(1, 29));
	
	boolalpha(cout);
	
	Array<int> test1000 {1,2,3,4,1};
	Array<int> test2000 {1,2,3,5};
	
	cout << (test1000 < test2000) << endl;
	cout << (test1000.count() < test2000.count()) << endl;
	
	Array<int> test00 {1,2,3,4,54,5};
	cout << test00.count() << " " << test00.capacity() << endl;
	
	Array<int,60> test22 {1,2,3,4};
	cout << test22.count() << " " << test22.capacity() << endl;
	
	test22 = test00;
	cout << test22.count() << " " << test22.capacity() << endl;
	
	// C++17 example
	
	#if (__cplusplus >= 201406)
		cout << "C++17" << endl;
		cout << test22.at(10).value_or(0) << endl;
		cout << test22.at(3).value_or(0) << endl;
	#endif
	//
	
	Array<int> testArray1 {1,2,3,4,5,6,7,8,9,10,1,12,3,4};
	
	testArray1.removeElements({3,4}, true);
	
	cout << testArray1 << endl;
	
	cout << testArray1.find(13) << endl;
	cout << testArray1.findAll(13) << endl;

	cout << testArray1 << ' ' << testArray1.count() << ' ' << testArray1.capacity() << endl;
	
	testArray1.assignRange(0,3);
	testArray1.removeAt(1);
	cout << testArray1 << ' ' << testArray1.count() << ' ' << testArray1.capacity() << endl;
	
	testArray1.assignRange(0,1);
	
	cout << testArray1 << ' ' << testArray1.count() << ' ' << testArray1.capacity() << endl;

	//
	testArray1.last() = 10;
	cout << testArray1.last() << endl;
	
	Array<int> testBlabla = {3,4};
	cout << (testArray1 - testBlabla) << endl;
	//testArray1.appendElements(testBlabla);
	
	cout << "A: " << testArray1 << ' ' << testArray1.count() << ' ' << testArray1.capacity() << endl;
	cout << "B: " << testBlabla << ' ' << testBlabla.count() << ' ' << testBlabla.capacity() << endl;
	testArray1.swap(testBlabla);
	cout << "A: " << testArray1 << ' ' << testArray1.count() << ' ' << testArray1.capacity() << endl;
	cout << "B: " << testBlabla << ' ' << testBlabla.count() << ' ' << testBlabla.capacity() << endl;
	
	testArray1.append(100);
	testArray1.appendElements({1,2,3,4});
	
	testArray1.removeLast();
	
	cout << testArray1 << endl;
	std::swap(testArray1, testBlabla);
	
	cout << "A: " << testArray1 << ' ' << testArray1.count() << ' ' << testArray1.capacity() << endl;
	cout << "B: " << testBlabla << ' ' << testBlabla.count() << ' ' << testBlabla.capacity() << endl;
	
	testArray1.append(9999);
	
	cout << "A: " << testArray1 << ' ' << testArray1.count() << ' ' << testArray1.capacity() << endl;
	
	vector<int> testBlabla2 {1,2,3,4,5};
	
	cout << "C: " << Array<int>(testBlabla2).toString() << ' ' << testBlabla2.size() << ' ' << testBlabla2.capacity() << endl;
	cout << "B: " << testBlabla << ' ' << testBlabla.count() << ' ' << testBlabla.capacity() << endl;
	
	testBlabla.swap(testBlabla2);
	
	cout << "C: " << Array<int>(testBlabla2).toString() << ' ' << testBlabla2.size() << ' ' << testBlabla2.capacity() << endl;
	cout << "B: " << testBlabla << ' ' << testBlabla.count() << ' ' << testBlabla.capacity() << endl;
	
	cout << (testBlabla < testArray1) << endl;
	//testArray1 -= toRemove;
	cout << testArray1 << endl;
	//
	//testArray1.removeSubrange(2, 4);
	testArray1 = {1,2,3,4,5,6,7,8,9};
	
	testArray1.removeSubrange({2,4});
	cout << testArray1 << endl;
	
	Array<string, 4> strings1 {"one", "two", "three", "four" };
	cout << strings1 << endl;
	Array<string, 2> strings2 {"five", "six" };
	cout << strings1 + strings2 << endl;
	
	Array<int, 3> test0;
	cout << test0.count() << ' ' << test0.capacity() << endl;
	
	test0.append(10);
	cout << test0.toString() << endl;
	cout << test0 << endl;
	
	cout << test0.count() << ' ' << test0.capacity() << endl;
	
	//cin.ignore();
	
	Array<int> test1 = {1,2,4,3};
	cout << test1.sorted(greater_equal<int>()) << endl;
	cout << test1.contains(1) << endl;
	
	test1.contains(1000);
	int asdfasdf = 10000;
	test1.contains(move(asdfasdf));
	
	cout << test1.toString() << endl;
	test1.append(12);
	cout << test1 << endl;
	
	int numbers1[3] = {1,2,3};
	
	Array<int> numbers = numbers1; //{1,2,3};
	cout << *(numbers.end()-1) << endl;

	for (const auto& number: numbers) {
		cout << number << ' ';
	}
	
	cout << "\n" << numbers.first() << ' ' << numbers.last() << endl;
	
	numbers.append(900);
	cout << numbers.last() << endl;
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	numbers.append(901);
	numbers.append(902);
	numbers.append(903);
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	numbers.append(904);
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	cout << numbers.toString() << endl;
	
	numbers.removeAll();
	cout << "(Removed) " << numbers.count() << ' ' << numbers.capacity() << endl;
	
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	numbers.append(10);
	cout << numbers.first() << endl;
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	numbers.append(11);
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	numbers.append(12);
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	numbers.append(13);
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	numbers.append(14);
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	cout << numbers.toString() << endl;
	
	numbers.removeAt(3);
	
	cout << numbers.toString() << endl;
	
	cout << numbers.capacity() << endl;
	numbers.append(16);
	numbers.append(17);
	numbers.append(18);
	
	numbers.insert(15, 3);
	
	cout << numbers.toString() << endl;
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	numbers.insert(15, 1);
	
	cout << numbers.toString() << endl;
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	// ------
	
	Array<int> otherNumbers = {90, 91, 92};
	numbers += otherNumbers;
	
	cout << numbers.toString() << endl;
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	//
	
	Array<int> otherNumbers2 = {93, 94, 95, 96, 97, 98};
	numbers += otherNumbers2;
	
	cout << numbers.toString() << endl;
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	numbers.append(99);
	
	cout << numbers.toString() << endl;
	
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	numbers += {100,101};
	
	cout << numbers.toString() << endl;
	
	cout << numbers.count() << ' ' << numbers.capacity() << endl;
	
	//
	
	Array<int> moreNumbers;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	moreNumbers += {10};
	cout << moreNumbers[0] << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	cout << boolalpha << moreNumbers.isEmpty() << endl;
	
	cout << numbers.contains(99) << endl;
	
	numbers.sort();
	
	cout << numbers.toString() << endl;
	
	moreNumbers = numbers.sorted([](int a, int b){ return a >= b; });
	cout << (moreNumbers == numbers) << endl;
	
	cout << moreNumbers.toString() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	moreNumbers.removeLast();
	
	cout << moreNumbers.toString() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	
	cout << moreNumbers.shrink() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	
	moreNumbers.shuffle();
	
	cout << moreNumbers.toString() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	moreNumbers.reserve(30);
	
	cout << moreNumbers.toString() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	
	moreNumbers.append(999);
	
	cout << moreNumbers.toString() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	
	const char* name1 = "Daniel";
	Array<string> names = {name1, "Sophie", "Michael"}; //vector<string>({"Daniel", "Sophie", "Michael"});
	names.append("lol");
	cout << names.toString() << endl;
	
	Array<float> floats;
	floats.append(10.1);
	cout << floats.count() << ' ' << floats.capacity() << endl;
	
	cout << Array<int>({1,2,3}).toString() << endl;
	
	Array<string> names2(names);
	cout << names2.toString() << endl;
	
	cout << names2.count() << ' ' << names2.capacity() << endl;
	names2.clear();
	
	cout << names2.toString() << endl;
	cout << names2.count() << ' ' << names2.capacity() << endl;

	// Move assignment operator
	
	Array<int> cosa, cosa2 {1,2,3};
	cosa = move(cosa2);
	cout << cosa[0] << endl;
	cout << cosa2.isEmpty() << endl;
	
	try {
		cout << cosa2.first() << endl;
	} catch (const length_error& exception) {
		cerr << "Error: " << exception.what() << endl;
	}
	
	Array<double> test {10.1, 20.2};
	test.insert(15.9, 1);
	cout << test.toString() << endl;
	cout << test.count() << ' ' << test.capacity() << endl;
	
	test.insert(100.9, 0);
	cout << test.toString() << endl;
	cout << test.count() << ' ' << test.capacity() << endl;
	
	test.insert(20.90, 0);
	cout << test.toString() << endl;
	cout << test.count() << ' ' << test.capacity() << endl;
	
	test.insert(280.91, 0);
	cout << test.toString() << endl;
	cout << test.count() << ' ' << test.capacity() << endl;
	
	Array<float> test2;
	cout << "?: " << test2.end() << endl;
	test2.insertAt(test2.begin(), 100);
	cout << test2.toString() << endl;
	cout << test2.count() << ' ' << test2.capacity() << endl;
	
	test2.append(200);
	
	test2.insertAt(test2.end(), 300);
	cout << test2.toString() << endl;
	cout << test2.count() << ' ' << test2.capacity() << endl;
	
	test2.removeLast();
	cout << test2.toString() << endl;
	cout << test2.count() << ' ' << test2.capacity() << endl;
	
	test2.removeLast();
	cout << test2.toString() << endl;
	cout << test2.count() << ' ' << test2.capacity() << endl;
	
	test2.removeLast();
	cout << test2.toString() << endl;
	cout << test2.count() << ' ' << test2.capacity() << endl;
	
	test2.removeAll();
	cout << test2.count() << ' ' << test2.capacity() << endl;
	test2.reserve(10);
	cout << test2.count() << ' ' << test2.capacity() << endl;
	
	test2.reserve(0);
	cout << test2.count() << ' ' << test2.capacity() << endl;
	
	Array<int> numbers2 {1,2,3,4,5};
	
	numbers2.append(6);
	cout << numbers2.toString() << endl;
	cout << numbers2.count() << ' ' << numbers2.capacity() << endl;
	
	numbers2.reserve(7);
	cout << numbers2.toString() << endl;
	cout << numbers2.count() << ' ' << numbers2.capacity() << endl;
	
	numbers2.resize(7);
	cout << numbers2.toString() << endl;
	cout << numbers2.count() << ' ' << numbers2.capacity() << endl;
	
	numbers2.clear();
	cout << numbers2.count() << ' ' << numbers2.capacity() << endl;
	
	numbers2.append(10);
	cout << numbers2[0]<< endl;
	
	numbers2 = vector<int>({1,2,3,4,5,6});
	cout << numbers2.toString() << endl;
	cout << "count: " << numbers2.count() << ' ' << numbers2.capacity() << endl;
	
	numbers2.removeLast();
	cout << numbers2.count() << ' ' << numbers2.capacity() << endl;
	
	vector<int> a = Array<int>::to<vector<int>>(numbers2);
	cout << "a size: " << a.size() << endl;

	for (const auto& element: a) {
		cout << element << ' ';
	}cout << endl;
	
	// Array<int, 1000> aaaa;
	// Array<int> nnnn = vector<int>({1,2,3,4}); // uses the move (&&) operation
	
	// BENCHMARKS

	cout << "\n------- BENCHMARKS -------\n" << endl;
	
	cout << "std::vector push_back: " << testVectorPushBack() << "s" << endl;
	cout << "std::vector emplace_back: " << testVectorEmplaceBack() << "s" << endl;
	cout << "evt::Array Append: " << testArrayAppend() << "s" << endl;
	
	cout << "std::vector Removal: " << testVectorRemoval() << "s" << endl;
	cout << "evt::Array Removal: " << testArrayRemoval() << "s" << endl;
	
	cout << "std::vector Insert at beginning: " << testVectorInsertAtBeginning() << "s" << endl;
	cout << "evt::Array Insert at beginning: " << testArrayInsertAtBeginning() << "s" << endl;

	cout << "std::vector Insert in the middle: " << testVectorInsertInMiddle() << "s" << endl;
	cout << "evt::Array Insert in the middle: " << testArrayInsertInMiddle() << "s" << endl;
	
	cout << "std::vector Insert at end: " << testVectorInsertAtEnd() << "s" << endl;
	cout << "evt::Array Insert at end: " << testArrayInsertAtEnd() << "s" << endl;
	
	// cout << "evt::Array Remove first: " << testArrayRemoveFirst() << "s" << endl;

	cout << "evt::Array Append elements: " << testArrayAppendElements() << "s" << endl;
	
	cout << "evt::Array Element access: " << testElementAccess() << "s" << endl;
	
	cout << "evt::Array Element find: " << testFindElement() << "s" << endl;
 
	//
	
	cout << "\n----- TEST Class Benchmark ----\n" << endl;
	
	cout << "std::vector push_back: " << testVectorPushBackTEST() << "s" << endl;
	cout << "std::vector emplace_back: " << testVectorEmplaceBackTEST() << "s" << endl;
	cout << "evt::Array Append: " << testArrayAppendTEST() << "s" << endl;
}
