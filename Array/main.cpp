#include <iostream>
#include "Array.hpp"
#include "Benchmark.hpp"

using namespace std;
using namespace evt;

int main() {
	
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
	moreNumbers.resize(30);
	
	cout << moreNumbers.toString() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	
	moreNumbers.append(999);
	
	cout << moreNumbers.toString() << endl;
	cout << moreNumbers.count() << ' ' << moreNumbers.capacity() << endl;
	
	const char* name1 = "Daniel";
	Array<string> names = {name1, "Sophie", "Michael"};
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
	
	/* BENCHMARKS */
	
	cout << "std::vector Insertion: " << testVectorInsertion() << "s" << endl;
	cout << "evt::Array Insertion: " << testArrayInsertion() << "s" << endl;
	
	cout << "std::vector Removal: " << testVectorRemoval() << "s" << endl;
	cout << "evt::Array Removal: " << testArrayRemoval() << "s" << endl;
}
