#pragma once

// make_unique is "safer" and only for c++14 and above, unique_ptr is faster and available since c++11; you decide
#define use_make_unique false

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <random>

namespace evt {
	
	// Extra functions for the "toString()" method
	inline std::string to_string(const std::string& str) { return str; }
	inline std::string to_string(const char chr) { return std::string(1,chr); }
		
	template <typename Others>
	inline std::string to_string(const Others other) { return std::to_string(other); }
	/* Place your custom "to_string()" function/s here for other classes. Use templates if you want. */
	
	// MARK: - Array Class

	template <typename Type, std::size_t initialCapacity = 2>
	class Array {
		
		// Types and macros
		typedef std::size_t sizeType;
		typedef std::unique_ptr<Type[]> Pointer;
		typedef std::initializer_list<Type> InitializerList;
		#define initialCapacity_ ((initialCapacity > 2) ? initialCapacity : 2)
		
		// MARK: - Attributes
		
		Pointer values { new Type[initialCapacity_] };
		sizeType count_ { 0 };
		sizeType capacity_ { initialCapacity_ };
		
		// MARK: - Private Functions
		
		inline double sizeOfArrayInMB(double currentCapacity) {
			return (sizeof(Type)*(currentCapacity)) / 1000000;
		}

		/// Assigns new memory, also updates the new capacity.
		inline void assignMemoryAndCapacityForSize(sizeType newSize) {
			
			#if (__cplusplus >= 201400) && use_make_unique
				values = std::make_unique<Type[]>(newSize);
			#elif cplusplus11 || !use_make_unique
				values = Pointer { new Type[newSize] };
			#endif
			
			capacity_ = newSize;
		}
		
		inline auto newArrayOfSize(sizeType newSize) {
			
			#if (__cplusplus >= 201400) && use_make_unique
				auto newValues { std::make_unique<Type[]>(newSize) };
			#elif (__cplusplus >= 201100) || !use_make_unique
				Pointer newValues { new Type[newSize] };
			#endif
			
			return newValues;
		}
		
		/// Resizes the array to a given size
		inline void resizeValuesToSize(sizeType newSize, bool move = 0) {
	
			auto newValues = newArrayOfSize(newSize);
			
			move ? std::move(&values[0], &values[count_], &newValues[0]) : std::copy(&values[0], &values[count_], &newValues[0]);
			values = std::move(newValues);
			
			capacity_ = newSize;
		}
		
		/// Replaces the content of the array with other elements
		template <typename Container>
		void assignNewElements(const Container& elements) {
			
			count_ = std::distance(std::begin(elements), std::end(elements));
			
			if (count_ > 2) {
				assignMemoryAndCapacityForSize(count_);
			}
			std::copy(std::begin(elements), std::end(elements), &values[0]);
		}
		
		template <typename Container>
		void assignNewElementsMOVE(Container&& elements) {
			
			count_ = std::distance(std::begin(elements), std::end(elements));
			
			if (count_ > 2) {
				assignMemoryAndCapacityForSize(count_);
			}
			std::move(std::begin(elements), std::end(elements), &values[0]);
		}
		
		template <typename Container>
		Array& removeElements(const Container& newElements) {
			
			sizeType elementsFound = 0;
			sizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			std::unique_ptr<sizeType[]> elementsPosition(new sizeType[countOfContainer]);
			
			auto newElement = std::begin(newElements);
			
			do {
				elementsFound = 0;
				newElement = std::begin(newElements);
				
				for (sizeType index = 0; index < count_; ++index) {
					
					if (values[index] == *newElement) {
						elementsPosition[elementsFound] = index;
						elementsFound += 1;
						++newElement;
					}
					else if (elementsFound != newElements.size()) {
						elementsFound = 0;
						newElement = std::begin(newElements);
					} else { break; }
				}
				
				if (elementsFound == newElements.size()) {
					for (sizeType i = 0; i < newElements.size(); ++i) {
						this->removeAt(elementsPosition[0]);
					}
				}
			} while(elementsFound == newElements.size());
			
			return *this;
		}
		
		template <typename Container>
		Array& appendNewElements(const Container& newElements) {
			
			sizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				std::copy(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {

				capacity_ = countOfContainer + count_;
				
				auto newValues = newArrayOfSize(capacity_);
				
				std::copy(&values[0], &values[count_], &newValues[0]);
				std::copy(std::begin(newElements), std::end(newElements), &newValues[count_]);
	
				values = std::move(newValues);
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		
		template <typename Container>
		Array& appendNewElementsMOVE(Container&& newElements) {
			
			sizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				std::move(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {
				
				capacity_ = countOfContainer + count_;
				
				auto newValues = newArrayOfSize(capacity_);

				std::move(&values[0], &values[count_], &newValues[0]);
				std::move(std::begin(newElements), std::end(newElements), &newValues[count_]);
				
				values = std::move(newValues);
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		
		inline void checkIfEmpty() const {
			if (count_ == 0) {
				throw std::length_error("Array is empty (lenght == 0)");
			}
		}
		
		inline void checkIfOutOfRange(const sizeType index) const {
			if (index >= count_) {
				throw std::out_of_range("Index out of range");
			}
		}
		
	public:
		
		// MARK: Constructors
		
		Array() { }
		Array(InitializerList elements) { assignNewElements(elements); }
		Array(const Array& otherArray) { (*this) = otherArray; }
		Array(Array&& otherArray) { (*this) = otherArray; }
		
		template<typename Container>
		Array(const Container& elements) { assignNewElements(elements); }
		
		template<typename Container>
		Array(Container&& elements) { assignNewElementsMOVE(elements); }

		// MARK: Capacity
		
		inline sizeType size() const  { return count_; }
		inline sizeType count() const { return count_; }
		inline sizeType capacity() const { return capacity_; }
		
		inline bool isEmpty() const { return (count_ == 0); }
		
		// MARK: Manage elements

		void insertAt(const Type* position, const Type& newElement) {
			
			if (position == &values[count_]) {
				this->append(newElement);
			}
			else if (position == &values[0]) {
				this->insert(newElement, 0);
			}
			else if (position > &values[0] && position < &values[count_]) {
				this->insert(newElement, position - &values[0]);
			}
			else {
				throw std::out_of_range("Index out of range");
			}
		}
		
		void insertAt(const Type* position, Type&& newElement) {
			
			if (position == &values[count_]) {
				this->append(newElement);
			}
			else if (position == &values[0]) {
				this->insert(newElement, 0);
			}
			else if (position > &values[0] && position < &values[count_]) {
				this->insert(newElement, position - &values[0]);
			}
			else {
				throw std::out_of_range("Index out of range");
			}
		}
		
		void insert(const Type& newElement, const sizeType index) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (capacity_ == count_) {
				
				capacity_ = (sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1);

				auto newValues = newArrayOfSize(capacity_);

				std::copy(&values[0], &values[index], &newValues[0]);
				std::copy(&values[index], &values[count_], &newValues[index+1]);
				
				values = std::move(newValues);
			}
			else {
				std::copy(&values[index], &values[count_], &values[index + 1]);
			}
			
			values[index] = newElement;
			count_ += 1;
		}
		
		void insert(Type&& newElement, const sizeType index) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (capacity_ == count_) {
				
				capacity_ = (sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1);
				
				auto newValues = newArrayOfSize(capacity_);
				
				std::move(&values[0], &values[index], &newValues[0]);
				std::move(&values[index], &values[count_], &newValues[index+1]);
				
				values = std::move(newValues);
			}
			else {
				std::move(&values[index], &values[count_], &values[index + 1]);
			}
			
			values[index] = std::move(newElement);
			count_ += 1;
		}
		
		/// Replaces the contents with copies of those in the range
		void assignRange(sizeType first, sizeType last) {
			
			if (first > last) {
				checkIfOutOfRange(first);
				sizeType auxFirst = last;
				last = first;
				first = auxFirst;
			}
			checkIfOutOfRange(last);
			
			std::copy(&values[first], &values[last+1], &values[0]);
			
			sizeType newCount = last-first + 1;
			
			if (newCount < (count_ / 4.0)) {
				resize(newCount);
			} else {
				count_ = newCount;
			}
		}
		
		void append(const Type& newElement) {
			
			if (capacity_ == count_) {
				resizeValuesToSize((sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1));
			}
			values[count_] = newElement;
			count_ += 1;
		}
		
		void append(Type&& newElement) {
	
			if (capacity_ == count_) {
				resizeValuesToSize((sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1), 1);
			}
			values[count_] = std::move(newElement);
			count_ += 1;
		}
		
		inline void append(InitializerList newElements) { appendNewElements(newElements); }
		inline void appendElements(InitializerList newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		inline void appendElements(const Container& newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		inline void appendElements(Container&& newElements) { appendNewElementsMOVE(newElements); }
		
		/// Only reserves new memory if the new size if bigger than the array capacity
		void reserve(const sizeType newSize) {
			
			if (newSize == 0 && count_ > 0) {
				removeAll();
				return;
			}
			
			if (newSize < count_) {
				count_ = newSize;
			}
			if (newSize > capacity_) {
				resizeValuesToSize(newSize);
			}
		}
		
		/// Only reserves new memory if the new size if smaller than the array count
		void resize(sizeType newSize) {
			
			if (newSize == 0 && count_ > 0) {
				removeAll();
				return;
			}
			
			if (newSize < count_) {
				count_ = newSize;
			}
			resizeValuesToSize(newSize);
		}
		
		/// Requests the removal of unused capacity. The new capacity will be the number of elements (count)
		bool shrink() {
			
			if (capacity_ > count_) {
				resizeValuesToSize(count_);
				return true;
			}
			return false;
		}
		
		/// Removes all elements in array, count will be 0, capacity won't be affected
		inline void clear() {
			removeAll(true);
		}
		
		/// Removes all elements in array, capacity will be 1 if desired
		void removeAll(const bool keepCapacity = false) {
			
			if (!keepCapacity) {
				assignMemoryAndCapacityForSize(1);
			}
			count_ = 0;
		}
		
		inline void removeAt(const sizeType index, const bool shrinkIfEmpty = true) {
		
			if (index == count_ - 1) {
				removeLast();
				return;
			}
			
			if (count_ == 2 && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			checkIfOutOfRange(index);
			
			std::copy(&values[index + 1], &values[count_], &values[index]);
			
			if (count_ - 1 < (capacity_ / 4.0)) {
				resize(count_ - 1);
			}
			else {
				count_ -= 1;
			}
		}
		
		void removeLast(const bool shrinkIfEmpty = true) {
			
			if (count_ == 2 && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			
			if (count_ - 1 < (capacity_ / 4.0)) {
				resize(count_ - 1);
			}
			else {
				count_ -= 1;
			}
		}
		
		inline void removeFirst(const bool shrinkIfEmpty = true) {
			removeAt(0, shrinkIfEmpty);
		}
		
		void removeSubrange(const sizeType startPosition, const sizeType endPosition, bool lessEqual = true) {
			for (sizeType i = startPosition; lessEqual ? (i <= endPosition) : (i < endPosition); ++i) {
				this->removeAt(startPosition);
			}
		}
		
		void removeSubrange(std::initializer_list<int> position, bool lessEqual = true) {
			
			sizeType startPosition = *std::begin(position);
			sizeType endPosition = *(std::end(position)-1);
			
			for (sizeType i = startPosition; lessEqual ? (i <= endPosition) : (i < endPosition); ++i) {
				this->removeAt(startPosition);
			}
		}
		
		void swap(Array& otherArray) {
			
			Pointer auxValues = std::move(this->values);
			sizeType auxCount = this->count_;
			sizeType auxCapacity = this->capacity_;
			
			this->values = std::move(otherArray.values);
			this->count_ = otherArray.count_;
			this->capacity_ = otherArray.capacity_;
			
			otherArray.values = std::move(auxValues);
			otherArray.count_ = auxCount;
			otherArray.capacity_ = auxCapacity;
		}
		
		template <typename Container>
		void swap(Container& container) {
			Array otherArray(container);
			swap(otherArray);
			container = Array::to<Container>(otherArray);
		}

		inline bool contains(const Type& element) const {
			for (const auto& elm: (*this)) {
				if (element == elm) { return true; }
			} 
			return false;
		}
		
		std::string toString() const {
		
			if (this->isEmpty()) {
				return "[]";
			}
			
			std::string output = "[";
			sizeType position = 0;
			
			for (const auto& value: *this) {
					output += [&] {
						if (typeid(value) == typeid(std::string)) {
							return ("\"" + evt::to_string(value) + "\"");
						} else if (typeid(value) == typeid(char)) {
							return ("\'" + evt::to_string(value) + "\'");
						}
						return evt::to_string(value);
					}();
				
				if (position+1 < count_) {
					output += ", ";
				}
				
				position += 1;
			}
			
			output += "]";
			
			return output;
		}
		
		/// Returns an ostream that contains the array elements
		friend std::ostream& operator<<(std::ostream& os, const evt::Array<Type,initialCapacity>& arr) {
			return os << arr.toString();
		}
		
		/// Converts Array to other types
		template <typename Container>
		static Container to(const Array& elements) {
			Container cont(elements.count());
			std::copy(std::begin(elements), std::end(elements), std::begin(cont));
			return cont;
		}
		
		template <typename Container>
		static Container to(Array&& elements) {
			Container cont(elements.count());
			std::move(std::begin(elements), std::end(elements), std::begin(cont));
			return cont;
		}
		
		template <typename Container>
		bool equal(const Container& elements) const {
			return (*this == elements);
		}
		
		bool equal(InitializerList elements) const {
			return this->operator==(elements);
		}
		
		// MARK: Operators overload
		
		/// Returns the pointer of the array
		inline Type* operator*() const {
			return values.get();
		}
		
		inline Type& operator[](const sizeType index) {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		inline const Type& operator[](const sizeType index) const {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		/// Removes a subrange of elements
		template <typename Container>
		inline Array& operator-=(const Container& newElements) {
			return removeElements(newElements);
		}
		
		inline Array& operator-=(InitializerList newElements) {
			return removeElements(newElements);
		}

		/// Returns a subrange of elements
		template <typename Container>
		inline Array operator-(const Container& newElements) const {
			Array otherArray(*this);
			otherArray -= newElements;
			return otherArray;
		}
		
		template <typename Container>
		inline Array operator+(const Container& newElements) const {
			Array otherArray(*this);
			otherArray += newElements;
			return otherArray;
		}
		
		template <typename Container>
		inline Array& operator+=(const Container& newElements) {
			return appendNewElements(newElements);
		}
		
		template <typename Container>
		inline Array& operator+=(Container&& newElements) {
			return appendNewElementsMOVE(newElements);
		}
		
		inline Array& operator+=(InitializerList newElements) {
			return appendNewElements(newElements);
		}
		
		template <typename Container>
		inline bool operator==(const Container& elements) const {
			sizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			if (count_ != countOfContainer) { return false; }
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}

		template <typename Container>
		inline bool operator!=(const Container& elements) const {
			return !( (*this) == elements );
		}
		
		// Returns true if the contents of the array are lexicographically less than the contents of the container
		template <typename Container>
		inline bool operator<(const Container& elements) {
			
			sizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			sizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (sizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (arrayElement != containerElement) {
					return arrayElement < containerElement;
				}
			}
			
			return count_ < countOfContainer;
		}
		
		template <typename Container>
		inline bool operator<=(const Container& elements) {
			
			sizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			sizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (sizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (arrayElement != containerElement) {
					return arrayElement < containerElement;
				}
			}
			
			return count_ <= countOfContainer;
		}
		
		// Returns true if the contents of the array are lexicographically greater than the contents of the container
		template <typename Container>
		inline bool operator>(const Container& elements) {
			
			sizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			sizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (sizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (arrayElement != containerElement) {
					return arrayElement > containerElement;
				}
			}
			
			return count_ > countOfContainer;
		}
		
		template <typename Container>
		inline bool operator>=(const Container& elements) {
			
			sizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			sizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (sizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (arrayElement != containerElement) {
					return arrayElement > containerElement;
				}
			}
			
			return count_ >= countOfContainer;
		}
		
		Array& operator=(const Array& otherArray) {
			
			if (this != &otherArray) {
				count_ = otherArray.count_;
				capacity_ = otherArray.capacity_;
				
				assignMemoryAndCapacityForSize(capacity_);
				std::copy(otherArray.begin(), otherArray.end(), &values[0]);
			}
			
			return *this;
		}
		
		Array& operator=(Array&& otherArray) {
			
			if (this != &otherArray) {
				values = std::move(otherArray.values);
				capacity_ = otherArray.capacity_;
				count_ = otherArray.count_;
				
				otherArray.capacity_ = 0;
				otherArray.count_ = 0;
			}
			
			return *this;
		}
		
		// MARK: Shuffle
		
		void shuffle() {
			
			if (this->isEmpty()) { return; }
			
			#ifdef __APPLE__
				std::mt19937_64 rng(arc4random());
			#else
				std::random_device rd;
				std::mt19937_64 rng(rd());
			#endif

			std::shuffle(&values[0], &values[count_], rng);
		}
		
		Array shuffled() const {
			
			if (this->isEmpty()) {
				return *this;
			}
			
			Array otherArray(*this);
			otherArray.shuffle();
			
			return otherArray;
		}
		
		// MARK: Sort
	
		void sort(std::function<bool(Type&,Type&)> compareFunction = std::less_equal<Type>()) {
			std::sort(&values[0], &values[count_], compareFunction);
		}
		
		Array sorted(std::function<bool(Type&,Type&)> compareFunction = std::less_equal<Type>()) const {
			
			if (this->isEmpty()) {
				return *this;
			}
			
			Array otherArray(*this);
			otherArray.sort(compareFunction);
			
			return otherArray;
		}
		
		// MARK: Positions
		
		inline Type* begin() const {
			return &values[0];
		}
		
		inline Type* end() const {
			return &values[count_];
		}
		
		inline Type& first() {
			checkIfEmpty();
			return *(&values[0]);
		}
		
		inline Type& last() {
			checkIfEmpty();
			return *(&values[count_]-1);
		}
		
		inline const Type& first() const {
			checkIfEmpty();
			return *(&values[0]);
		}
		
		inline const Type& last() const {
			checkIfEmpty();
			return *(&values[count_]-1);
		}
	};
}

#undef use_make_unique
#undef initialCapacity_
