#pragma once

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <memory>
#include <string>
#include <random>

using namespace std;

namespace evt {
	
	template <typename Type>
	class Array {
		
		// MARK: - Attributes
		
		unique_ptr<Type[]> values = nullptr;
		size_t count_ = 0;
		size_t capacity_ = 0;
		
		// MARK: - Private functions
		
		template <typename Container>
		void assignNewElements(const Container& elements) {
			count_ = distance(std::begin(elements), std::end(elements));
			capacity_ = count_;
			
			values = unique_ptr<Type[]>(new Type[count_]);
			
			size_t index = 0;
			for (const auto& element: elements) {
				values[index] = element;
				++index;
			}
		}
		
		template <typename Container>
		Array& appendNewElements(const Container& newElements) {
			
			size_t countOfContainer = distance(std::begin(newElements), std::end(newElements));
			
			if (capacity() >= (count() + countOfContainer)) {
				
				size_t index = count();
				for (const auto& newElement: newElements) {
					values[index] = newElement;
					++index;
				}
				
				count_ += countOfContainer;
			}
			else {
				
				size_t newcount = (count() == 0) ? (countOfContainer) : (count() * 2);
				
				while (newcount < (count() + countOfContainer)) {
					newcount += 2;
				}
				
				unique_ptr<Type[]> newValues (new Type[newcount]);
				
				if (count() > 0) {
					copy(this->begin(), this->end(), &newValues[0]);
				}
				
				size_t index = count();
				for (const auto& element: newElements) {
					newValues[index] = element;
					++index;
				}
				
				values = move(newValues);
				
				capacity_ = newcount;
				count_ = count() + countOfContainer;
			}
			
			return *this;
		}
		
		// TRUE if elements are the same
		template<typename Container>
		bool compareElements(const Container& elements) const {
			
			size_t index = 0;
			for (const auto& element: elements) {
				if (values[index] != element) {
					return false;
				}
				++index;
			}
			
			return true;
		}
		
		void checkIfEmpty() const {
			if (count() == 0) {
				throw length_error("Array is empty (lenght == 0)");
			}
		}
		
		void checkIfOutOfRange(const size_t& index) const {
			if (index >= count()) {
				throw out_of_range("Index out of range");
			}
		}
		
	public:
		
		// MARK: Constructors
		
		Array() {}
		
		template<typename Container>
		Array(const Container& elements) { assignNewElements(elements); }
		Array(const initializer_list<Type>& elements) { assignNewElements(elements); }
		Array(const Array& otherArray) { (*this) = otherArray; }
		
		Array(const int32_t& capacity) { // Type can't be size_t because it intefere with the other constructor
			
			if (values == nullptr) {
				values = unique_ptr<Type[]>(new Type[size_t(capacity)]);
				capacity_ = capacity;
			}
		}
		
		size_t count() const { return count_; }
		size_t capacity() const { return capacity_; }
		
		// MARK: Manage elements
		
		void insert(const Type& newElement, const size_t& index) {
			
			checkIfOutOfRange(index);
			
			unique_ptr<Type[]> newValues = nullptr;
			
			if (capacity() == count()) {
				
				size_t newcount = (count() == 0) ? (1) : (count() * 2);
				
				newValues = unique_ptr<Type[]>(new Type[newcount]);
				
				copy(this->begin(), this->end(), &newValues[0]);
				copy(&newValues[index], &newValues[count_ + 1], &newValues[index + 1]);
				
				capacity_ = newcount;
			}
			else {
				
				newValues = unique_ptr<Type[]>(new Type[count()]);
				
				copy(this->begin(), this->end(), &newValues[0]);
				copy(&newValues[index], &newValues[count()], &newValues[index + 1]);
			}
			
			newValues[index] = newElement;
			
			values = move(newValues);
			
			count_ += 1;
		}
		
		void append(const Type& newElement) {
			
			if (capacity() == count()) {
				
				size_t newCount = (count() == 0) ? (1) : (count() * 2);
				
				unique_ptr<Type[]> newValues (new Type[newCount]);
				
				if (count() > 0) {
					copy(this->begin(), this->end(), &newValues[0]);
				}
				
				newValues[count()] = newElement;
				
				values = move(newValues);
				
				capacity_ = newCount;
			}
			else if (count() < capacity()) {
				values[count()] = newElement;
			}
			
			count_ += 1;
		}
		
		template<typename Container>
		void append(const Container& newElements) { (*this) += newElements; }
		void append(const initializer_list<Type>& newElements) { (*this) += newElements; }
		
		void resize(const size_t& newSize) {
			
			if (newSize == 0 && count() > 0 && capacity() > 0 && values != nullptr) {
				removeAll();
			}
			
			checkIfEmpty();
			
			if (newSize < capacity()) {
				for_each(&values[newSize], this->end(), [](Type& value){ value = Type{}; });
				count_ = newSize;
			}
			else if (newSize > capacity()) {
				
				unique_ptr<Type[]> newValues(new Type[newSize]);
				copy(this->begin(), this->end(), &newValues[0]);
				
				values = move(newValues);
				capacity_ = newSize;
			}
		}
		
		bool shrink() {
			
			if (capacity() > count()) {
				
				unique_ptr<Type[]> newValues (new Type[count()]);
				
				if (count() > 0) {
					copy(this->begin(), this->end(), &newValues[0]);
				}
				
				values = move(newValues);
				
				capacity_ = count();
				
				return true;
			}
			
			return false;
		}
		
		void clear() {
			removeAll(true);
		}
		
		void removeAll(const bool& keepCapacity = false) {
			
			if (keepCapacity) {
				if (values != nullptr) {
					for_each(this->begin(), this->end(), [](Type& value){ value = Type{}; });
					count_ = 0;
				}
			}
			else {
				values = unique_ptr<Type[]>(new Type[0]);
				count_ = 0;
				capacity_ = 0;
			}
		}
		
		void removeAt(const size_t& index, const bool& shrinkIfEmpty = true) {
			
			if (count() == 1 && capacity() > 1 && values != nullptr && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			checkIfOutOfRange(index);
			
			copy(&values[index + 1], this->end(), &values[index]);
			values[count() - 1] = Type{};
			
			count_ -= 1;
		}
		
		void removeLast(const bool& shrinkIfEmpty = true) {
			
			if (count() == 1 && capacity() > 1 && values != nullptr && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			
			values[count()] = Type{};
			count_ -= 1;
		}
		
		void removeFirst() {
			removeAt(0);
		}
	
		bool contains(const Type& element) const {
			checkIfEmpty();
			return (find(this->begin(), this->end(), element)) != this->end();
		}
		
		bool isEmpty() const {
			return (count() == 0);
		}
		
		string toString() const {
		
			string output = "[";
			
			size_t position = 0;
			for (const auto& value: *this) {
				
				output += to_string(value);
				
				if (position+1 < count()) {
					output += ", ";
				}
				
				++position;
			}
			
			output += "]";
			
			return output;
		}
		
		// MARK: Operators overload
		
		Type& operator[](const size_t& index) {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		template<typename Container>
		Array& operator+=(const Container& newElements) {
			return appendNewElements(newElements);
		}
		
		Array& operator+=(const initializer_list<Type>& newElements) {
			return appendNewElements(newElements);
		}
		
		template<typename Container>
		bool operator==(const Container& elements) const {
			return compareElements(elements);
		}
		
		bool operator==(const initializer_list<Type>& elements) const {
			return compareElements(elements);
		}
		
		template<typename Container>
		bool operator!=(const Container& elements) const {
			return !( (*this) == elements );
		}
		
		bool operator!=(const initializer_list<Type>& elements) const {
			return !( (*this) == elements );
		}
		
		Array& operator=(const Array& otherArray) {
			
			count_ = otherArray.count();
			capacity_ = otherArray.capacity();
			
			values = unique_ptr<Type[]> (new Type[capacity()]);
			
			copy(otherArray.begin(), otherArray.end(), this->begin());
			
			return *this;
		}
		
		// MARK: Shuffle
		
		void shuffle() {
			
			checkIfEmpty();
			
			#ifdef __APPLE__
				mt19937_64 rng(arc4random());
			#else
				random_device rd;
				mt19937_64 rng(rd());
			#endif

			std::shuffle(this->begin(), this->end(), rng);
		}
		
		Array shuffled() const {
			
			checkIfEmpty();
			
			Array otherArray(*this);
			otherArray.shuffle();
			
			return otherArray;
		}
		
		// MARK: Sort
		
		void sort() {
			checkIfEmpty();
			std::sort(this->begin(), this->end());
		}
		
		template <typename Function>
		void sort(const Function& compareFunction) {
			checkIfEmpty();
			std::sort(this->begin(), this->end(), compareFunction);
		}
		
		template <typename Function>
		Array sorted(const Function& compareFunction) const {
			
			checkIfEmpty();
			
			Array otherArray(*this);
			otherArray.sort(compareFunction);
			
			return otherArray;
		}
		
		Array sorted() const {
			
			checkIfEmpty();
			
			Array otherArray(*this);
			otherArray.sort();
			
			return otherArray;
		}
		
		// MARK: Positions
		
		Type* begin() const {
			checkIfEmpty();
			return &values[0];
		}
		
		Type* end() const {
			checkIfEmpty();
			return &values[count()];
		}
		
		Type first() const {
			checkIfEmpty();
			return values[0];
		}
		
		Type last() const {
			checkIfEmpty();
			return values[count() - 1];
		}
	};
}
