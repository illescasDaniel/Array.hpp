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
	
	Array<int> numbers {1,2,3,4,5};
	const auto& lazyNumbers = numbers.lazyMap<int>([](const int number){
		return number * 2;
	});
	
	cout << lazyNumbers[1] << endl;
	
	//
	
	Array<string> names {"Daniel", "John", "Peter"};
	const auto& lazyNamesSize = names.lazyMap<int>([](const string& name) {
		return name.size();
	});
	
	cout << lazyNamesSize[0] << endl;
	
	
	//
	cout << "-----" << endl;
	testArrayAppend();

	float sortTime = benchmark([&] {
		numbers2.sort([&](const int& lhs, const int& rhs){
			return lhs >= rhs;
		});
	});
	cout << sortTime << endl;
	cout << numbers2[0] << ' ' << numbers2[1] << endl;
	
	cout << "----" << endl;
	
/*
	Array<string> names {"Daniel", "John", "Peter"};
	
	auto sizes = names.map<size_t>([](const string& str) {
		return str.size();
	});
	
	auto arrSize = sizes.reduce<size_t>([](const size_t totalSize, const size_t strSize){
		return totalSize + strSize;
	});
	
	cout << arrSize << endl;
	
	auto totalStrSize = [](const Array<string> strArray) {
		return strArray.map<size_t>([](auto str) {
			return str.size();
		})
		.reduce<size_t>([](auto total, auto strSize) {
			return total + strSize;
		});
	};
	
	Array<string>({"names", "john"})
	    .map<size_t>([](auto str) {
		    return str.size();
	    })
	    .reduce<size_t>([](auto total, auto strSize) {
		    return total + strSize;
	    });
	
	cout << totalStrSize(Array<string>{"hola", "adios"}) << endl;
*/
	/*
	 Array<string> names2_ {"daniel", "test", "lol"};
	 
	 string joint = names2_.joinToString(", ", "->", "END", -1, "...", [](const string& element) {
		return "`" + element + "`";
	});
	
	joint = names2_.joinToString(", ", "->", "END", 2);
	cout << joint << endl;
	
	joint = names2_.joinToString(", ", [](const string& element) {
		return "(" + element + ")";
	});
	
	cout << joint << endl;
	
	joint = names2_.joinToString(" <-> "s);
	cout << joint << endl;
	
	double namesSizeMean = names2_.map<size_t>([](const string& element) {
		return element.length();
	}).mean();
	
	size_t namesSizeSum = names2_.map<size_t>([](const string& element) {
		return element.length();
	}).sum();
	
	size_t namesSizeSum2 = names2_.map<size_t>([](const string& element) {
		return element.length();
	}).reduce<size_t>([&](const size_t result, const size_t strSize) {
		return result + strSize;
	});
	
	Array<string> names22_ = names2_.map([](const string& element) {
		return "(" + element + ")";
	});
	cout << names22_ << endl;
	
	cout << namesSizeMean << endl;
	cout << namesSizeSum << endl;
	cout << namesSizeSum2 << endl;
	
	Array<int> numbers__(6, 3);
	cout << numbers__ << endl;
	
	Array<int> numbers11 {1,2,3,4};
	Array<int> numbers22 ({5,6,7,8}, 90);
	Array<int> numbers3 (100000);
	
	cout << numbers11 << ' ' << numbers11.size() << ' ' << numbers11.capacity() << endl;
	cout << numbers22 << ' ' << numbers22.size() << ' ' << numbers22.capacity() << endl;
	cout << numbers3 << ' ' << numbers3.size() << ' ' << numbers3.capacity() << endl;
	
	vector<int> numbers4 {1,3,4,7};
	Array<int> numbers5 (numbers4, 97);
	
	cout << numbers5 << ' ' << numbers5.count() << ' ' << numbers5.capacity() << endl;
	
	Array<Test2> tests;
	tests.append(Test2(1, 29));
	
	boolalpha(cout);
	
	Array<int> test1000 {1,2,3,4,1};
	Array<int> test2000 {1,2,3,5};
	
	cout << (test1000 < test2000) << endl;
	cout << (test1000.count() < test2000.count()) << endl;
	
	Array<int> test00 {1,2,3,4,54,5};
	cout << test00.count() << " " << test00.capacity() << endl;
	
	Array<int> test22 ({1,2,3,4}, 60);
	cout << test22.count() << " " << test22.capacity() << endl;
	
	test22 = test00;
	cout << test22.count() << " " << test22.capacity() << endl;
	
	// C++17 example
	
	Array<int> numbersArr {1,2,3,4,5};
	if (numbersArr.find(3) != numbersArr.count()) {
		cout << "Found!!" << endl;;
	}
	
	cout << numbersArr.first() << " " << numbersArr.last() << endl;
	
	#if (__cplusplus >= 201406)
	
		cout << numbersArr.filter([](const int& number){
			return number >= 3;
		}) << endl;
	
		auto firstEven = numbersArr.first([](const int& number) {
			return number % 2 == 0;
		});
		cout << firstEven.value_or(0) << endl;
	
		if (const auto firstOdd = numbersArr.first([](const int& n) { return n % 2 == 1; })) { // Optional
			cout << firstOdd.value_or(0) << endl;
		}
	
		if (const auto lastEven = numbersArr.last([](const int& n) { return n % 2 == 0; })) {
			cout << lastEven.value_or(0) << endl;
		}
	 
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
	
	Array<string> strings1 {"one", "two", "three", "four" };
	cout << strings1 << endl;
	Array<string> strings2 {"five", "six" };
	cout << strings1 + strings2 << endl;
	
	Array<int> test0(3);
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
	
	Array<string> someStrings {"daniel", "test", "daniel", "john", "null", "macOS"};
	cout << someStrings.countOf("daniel") << endl;
	cout << someStrings.countOf([](const string& str){ return str.length() > 4; }) << endl;
	
	// Array<int, 1000> aaaa;
	// Array<int> nnnn = vector<int>({1,2,3,4}); // uses the move (&&) operation
	
	// BENCHMARKS
*/
	cout << "\n------- BENCHMARKS -------\n" << endl;

	//cout << "std::vector push_back: " << testVectorPushBack() << "s" << endl;
	//cout << "std::vector emplace_back: " << testVectorEmplaceBack() << "s" << endl;
	cout << "evt::Array Append: " << testArrayAppend() << "s" << endl;
	// Uses less RAM
	cout << "evt::Array Append: " << testArrayAppendEfficiently() << "s" << endl;
	/*
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
	//cout << sizeof(Array<int>) << endl;
	cout << "\n----- TEST Class Benchmark ----\n" << endl;
	*/
	//cout << "std::vector push_back: " << testVectorPushBackTEST() << "s" << endl;
	//cout << "std::vector emplace_back: " << testVectorEmplaceBackTEST() << "s" << endl;
	//cout << "evt::Array Append: " << testArrayAppendTEST() << "s" << endl;
	 
}
