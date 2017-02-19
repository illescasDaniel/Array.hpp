#if __cplusplus >= 201100
#pragma once
#endif

#ifndef evt_Array_hpp
#define evt_Array_hpp

#define DISTANCE_(iteratorFirst_, iteratorLast_) ((iteratorLast_)-(iteratorFirst_))
#define copy_(first, last, d_first) ( std::memcpy(d_first, first, sizeof(Type) * (last-first)) )
#define move_(first, last, d_first) ( std::memmove(d_first, first, sizeof(Type) * (last-first)) )

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <cstring>
#include <random>

namespace evt {
	
	// Extra functions for the "toString()" method
	inline std::string to_string(const std::string& str) { return str; }
	inline std::string to_string(const char chr) { return std::string(1,chr); }
	
	template <typename Others>
	inline std::string to_string(const Others other) { return std::to_string(other); }
	
	/* Custom "to_string()" function for X class */
	// template <typename MyClass>
	// inline std::string to_string(const MyClass& object) {
	//		return object.value + ... ;
	// }
	
	// MARK: - Array Class
	template <typename Type, std::size_t initialCapacity = 2>
	class Array {
		
		#define sizeOfArrayInMB(number_) (float((24 + (sizeof(Type)*(number_)))) / 1000000.0)
		#define newType(size_) ( static_cast<Type*>(operator new[](sizeof(Type) * (size_))) )
		#define initialCapacity_ ((initialCapacity > 2) ? initialCapacity : 2)
		
		// MARK: - Attributes
		
		#if __cplusplus >= 201100
			Type* values { new Type[initialCapacity_] };
			std::size_t count_ { 0 };
			std::size_t capacity_ { initialCapacity_ };
		#else
			Type* values;
			std::size_t count_;
			std::size_t capacity_;
		#endif
		
		// MARK: - Private functions
		
		inline void initializeArray() {
			#if __cplusplus < 201100
				count_ = 0 ;
				capacity_ = initialCapacity_;
				values = new Type[capacity_];
			#endif
		}
		
		inline void deleteMemory() {
			if (values) {
				delete[] values;
				values = nullptr;
			}
		}
		
		inline void assignMemoryForSize(const size_t newSize) {
			deleteMemory();
			values = new Type[newSize];
		}
		
		inline void resizeValuesToSize(const size_t newSize, bool move = 0) {
			Type* newValues = new Type[newSize];
			move ? std::memmove(newValues, values, sizeof(Type) * count_) : std::memcpy(newValues, values, sizeof(Type) * count_);
			
			deleteMemory();
			values = newValues;
			
			capacity_ = newSize;
		}
		
		template <typename Container>
		void assignNewElements(const Container& elements) {
			
			count_ = DISTANCE_(std::begin(elements), std::end(elements));
			
			if (count_ > 2) {
				capacity_ = count_;
				assignMemoryForSize(capacity_);
			}
			copy_(std::begin(elements), std::end(elements), &values[0]);
		}
		
		#if __cplusplus >= 201100
		template <typename Container>
		void assignNewElements(Container&& elements) {
			
			count_ = DISTANCE_(std::begin(elements), std::end(elements));
			
			if (count_ > 2) {
				
				capacity_ = count_;
				assignMemoryForSize(capacity_);
			}
			std::memmove(&values[0], &elements[0], sizeof(Type) * count_);
		}
		
		template <typename Container>
		Array& appendNewElementsMOVE(Container&& newElements) {
			
			std::size_t countOfContainer = DISTANCE_(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				move_(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {
				
				capacity_ = countOfContainer + count_;
				
				while (capacity_ < (count_ + countOfContainer)) {
					capacity_ += 2;
				}
				
				Type* newValues = new Type[capacity_];
				
				move_(&values[0], &values[count_], &newValues[0]);
				move_(std::begin(newElements), std::end(newElements), &newValues[count_]);
				
				deleteMemory();
				values = newValues;
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		#endif
		
		template <typename Container>
		Array& appendNewElements(const Container& newElements) {
			
			std::size_t countOfContainer = DISTANCE_(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				copy_(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {
				
				capacity_ = countOfContainer + count_;
				
				while (capacity_ < (count_ + countOfContainer)) {
					capacity_ += 2;
				}
				
				Type* newValues = new Type[capacity_];
				
				copy_(&values[0], &values[count_], &newValues[0]);
				copy_(std::begin(newElements), std::end(newElements), &newValues[count_]);
				
				deleteMemory();
				values = newValues;
			}
			
			count_ += countOfContainer;
			
			return *this;
		}

		inline void checkIfEmpty() const {
			if (count_ == 0) {
				throw std::length_error("Array is empty (lenght == 0)");
			}
		}
		
		inline void checkIfOutOfRange(const std::size_t index) const {
			if (index >= count_) {
				throw std::out_of_range("Index out of range");
			}
		}
		
	public:
		
		// MARK: Constructors
		
		Array() { initializeArray(); }
		
		template<typename Container>
		Array(const Container& elements) {
			assignNewElements(elements);
		}
		
		#if __cplusplus >= 201100
		template<typename Container>
		Array(Container&& elements) { assignNewElements(elements); }
		
		Array(const std::initializer_list<Type>& elements) { assignNewElements(elements); }
		#endif
		
		explicit Array(const Array& otherArray) {
			(*this) = otherArray;
		}
		
		inline std::size_t size() const  { return count_; }
		inline std::size_t count() const { return count_; }
		inline std::size_t capacity() const { return capacity_; }
		
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
		
		#if __cplusplus >= 201100
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
		#endif
		
		void insert(const Type& newElement, const std::size_t index) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (capacity_ == count_) {
				
				capacity_ = (sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1);
				
				Type* newValues = new Type[capacity_];
				
				copy_(&values[0], &values[index], &newValues[0]);
				copy_(&values[index], &values[count_], &newValues[index+1]);
				
				deleteMemory();
				values = newValues;
			}
			else {
				copy_(&values[index], &values[count_], &values[index + 1]);
			}
			
			values[index] = newElement;
			count_ += 1;
		}
		
		#if __cplusplus >= 201100
		void insert(Type&& newElement, const std::size_t index) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (capacity_ == count_) {
				
				capacity_ = (sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1);
				
				Type* newValues = new Type[capacity_];
			
				move_(&values[0], &values[index], &newValues[0]);
				move_(&values[index], &values[count_], &newValues[index+1]);
				
				deleteMemory();
				values = newValues;
			}
			else {
				move_(&values[index], &values[count_], &values[index + 1]);
			}
			
			values[index] = std::move(newElement);
			count_ += 1;
		}
		
		void append(Type&& newElement) {
			
			if (capacity_ == count_) {
				resizeValuesToSize((sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1),1);
			}
			values[count_] = newElement;
			count_ += 1;
		}
		#endif
		
		void append(const Type& newElement) {
			
			if (capacity_ == count_) {
				resizeValuesToSize((sizeOfArrayInMB(capacity_) < 500) ? (capacity_ << 2) : (capacity_ << 1));
			}
			values[count_] = newElement;
			count_ += 1;
		}
		
		template<typename Container>
		inline void appendElements(const Container& newElements) { (*this) += newElements; }
		
		#if __cplusplus >= 201100
		template<typename Container>
		inline void appendElements(Container&& newElements) { (*this) += newElements; }
		
		inline void append(const std::initializer_list<Type>& newElements) { (*this) += newElements; }
		inline void append(std::initializer_list<Type>&& newElements) { (*this) += newElements; }
		#endif
		
		/// Only reserves new memory if the new size if bigger than the array capacity
		void reserve(const std::size_t newSize) {
			
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
		
		/// Only reserves new memory if the new size if bigger than the array count
		void resize(std::size_t newSize) {
			
			if (newSize == 0 && count_ > 0) {
				removeAll();
				return;
			}
			
			if (newSize < count_) {
				count_ = newSize;
			}
			resizeValuesToSize(newSize);
		}
		
		bool shrink() {
			
			if (capacity_ > count_) {
				resizeValuesToSize(count_);
				return true;
			}
			return false;
		}
		
		inline void clear() {
			removeAll(true);
		}
		
		void removeAll(const bool keepCapacity = false) {
			
			if (!keepCapacity) {
				assignMemoryForSize(1);
				capacity_ = 1;
			}
			count_ = 0;
		}
		
		inline void removeAt(const std::size_t index, const bool shrinkIfEmpty = true) {
			
			if (index == count_ - 1) {
				removeLast();
				return;
			}
			
			if (count_ == 2 && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			checkIfOutOfRange(index);
			
			copy_(&values[index + 1], &values[count_], &values[index]);
			
			count_ -= 1;
		}
		
		void removeLast(const bool shrinkIfEmpty = true) {
			(count_ == 2 && shrinkIfEmpty) ?
			(shrink(), count_ -= 1)
			:	((count_ != 0) ?
					(count_ -= 1)
				 :	(throw std::length_error("Array is empty (lenght == 0)")));
		}
		
		inline void removeFirst(const bool shrinkIfEmpty = true) {
			removeAt(0, shrinkIfEmpty);
		}
		
		bool contains(const Type& element) const {
			for (size_t i = 0; i < count_; ++i) {
				if (values[i] == element) { return true; }
			}
			return false;
		}
		
		inline bool isEmpty() const {
			return (count_ == 0);
		}
		
		std::string toString() const {
			
			if (this->isEmpty()) {
				return "[]";
			}
			
			std::string output = "[";
			std::size_t position = 0;
			
			#if __cplusplus >= 201100
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
			#else
			
			for (size_t i = 0; i < count_; ++i) {
				
				if (typeid(values[i]) == typeid(std::string)) {
					output += ("\"" + evt::to_string(values[i]) + "\"");
				} else if (typeid(values[i]) == typeid(char)) {
					output += ("\'" + evt::to_string(values[i]) + "\'");
				} else {
					output += std::to_string(values[i]);
				}
				
				if (position+1 < count_) {
					output += ", ";
				}
				
				position += 1;
			}
			#endif
			
			output += "]";
			
			return output;
		}
		
		friend std::ostream& operator<<(std::ostream& os, const evt::Array<Type, initialCapacity>& arr) {
			return os << arr.toString();
		}
		
		#if __cplusplus >= 201100
		friend std::ostream& operator<<(std::ostream& os, evt::Array<Type, initialCapacity>&& arr) {
			return os << arr.toString();
		}
		#endif
		
		// Convert Array to other types
		template <typename Container>
		static Container to(const Array& elements) {
			Container cont(elements.count());
			std::copy(std::begin(elements), std::end(elements), std::begin(cont));
			return cont;
		}
		
		// MARK: Operators overload
		
		inline Type& operator*() const {
			return *values;
		}
		
		inline Type& operator[](const std::size_t index) {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		template<typename Container>
		inline Array& operator+=(const Container& newElements) {
			return appendNewElements(newElements);
		}
		
		#if __cplusplus >= 201100
		template<typename Container>
		inline Array& operator+=(Container&& newElements) {
			return appendNewElementsMOVE(newElements);
		}
		
		inline Array& operator+=(const std::initializer_list<Type>& newElements) {
			return appendNewElements(newElements);
		}
		
		inline Array& operator+=(std::initializer_list<Type>&& newElements) {
			return appendNewElementsMOVE(newElements);
		}
		
		template<typename Container>
		inline bool operator==(Container&& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}

		inline bool operator==(std::initializer_list<Type>&& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		inline bool operator==(const std::initializer_list<Type>& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		#endif
		
		template<typename Container>
		inline bool operator==(const Container& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		template<typename Container>
		inline bool operator!=(const Container& elements) const {
			return !( (*this) == elements );
		}
		
		#if __cplusplus >= 201100
		template<typename Container>
		inline bool operator!=(Container&& elements) const {
			return !( (*this) == elements );
		}
		
		inline bool operator!=(const std::initializer_list<Type>& elements) const {
			return !( (*this) == elements );
		}
		
		inline bool operator!=(std::initializer_list<Type>&& elements) const {
			return !( (*this) == elements );
		}
		
		Array& operator=(Array&& otherArray) {
			
			count_ = std::move(otherArray.count_);
			capacity_ = std::move(otherArray.capacity_);
			
			deleteMemory();
			values = otherArray.values;
			otherArray.values = nullptr;
			
			otherArray.count_ = 0;
			otherArray.capacity_ = 0;
			
			return *this;
		}
		#endif
		
		Array& operator=(const Array& otherArray) {
			
			count_ = otherArray.count_;
			capacity_ = otherArray.capacity_;
			
			assignMemoryForSize(capacity_);
			
			copy_(otherArray.begin(), otherArray.end(), &values[0]);
			
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
		
		void sort() {
			std::sort(&values[0], &values[count_]);
		}
		
		template <typename Function>
		void sort(const Function& compareFunction) {

			std::sort(&values[0], &values[count_], compareFunction);
		}
		
		template <typename Function>
		Array sorted(const Function& compareFunction) const {
			
			if (this->isEmpty()) {
				return *this;
			}
			
			Array otherArray(*this);
			otherArray.sort(compareFunction);
			
			return otherArray;
		}
		
		Array sorted() const {
			
			if (this->isEmpty()) {
				return *this;
			}
			
			Array otherArray(*this);
			otherArray.sort();
			
			return otherArray;
		}
		
		// MARK: Positions
		
		inline Type* begin() const {
			return &values[0];
		}
		
		inline Type* end() const {
			return &values[count_];
		}
		
		inline Type first() const {
			checkIfEmpty();
			return *(&values[0]);
		}
		
		inline Type last() const {
			checkIfEmpty();
			return *(&values[count_]-1);
		}
		
		~Array() { deleteMemory(); }
	};
}

#undef sizeOfArrayInMB
#undef DISTANCE_
#undef copy_
#undef move_
#undef initialCapacity_

#endif
