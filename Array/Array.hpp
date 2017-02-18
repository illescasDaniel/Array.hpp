#pragma once

// make_unique is "safer", but unique_ptr is faster; you decide
#define use_make_unique false
#define cplusplus14 (__cplusplus >= 201400)
#define cplusplus11 (__cplusplus >= 201100)

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
	
	/* Custom "to_string()" function for X class */
	// template <typename MyClass>
	// inline std::string to_string(const MyClass& object) {
	//		return object.value + ... ;
	// }
	
	// MARK: - Array Class
	template <typename Type, std::size_t initialCapacity = 2>
	class Array {
		
		#define sizeOfArrayInMB(number_) (float((24 + (sizeof(Type)*(number_)))) / 1000000.0)
		#define initialCapacity_ ((initialCapacity > 2) ? initialCapacity : 2)
		
		// MARK: - Attributes
		
		#if cplusplus14 && use_make_unique
			std::unique_ptr<Type[]> values { std::make_unique<Type[]>(initialCapacity_) };
		#elif cplusplus11 || !use_make_unique
			std::unique_ptr<Type[]> values { new Type[initialCapacity_] };
		#endif

		std::size_t count_ { 0 };
		std::size_t capacity_ { initialCapacity_ };
		
		// MARK: - Private Functions
		
		inline void assignMemoryForSize(size_t newSize) {
			
			#if cplusplus14 && use_make_unique
				values = std::make_unique<Type[]>(newSize);
			#elif cplusplus11 || !use_make_unique
				values = std::unique_ptr<Type[]> { new Type[newSize] };
			#endif
			
			capacity_ = newSize;
		}
		
		inline void resizeValuesToSize(size_t newSize, bool move = 0) {
			
			#if cplusplus14 && use_make_unique
				std::unique_ptr<Type[]> newValues { std::make_unique<Type[]>(newSize) };
			#elif cplusplus11 || !use_make_unique
				std::unique_ptr<Type[]> newValues { new Type[newSize] };
			#endif
			
			move ? std::move(&values[0], &values[count_], &newValues[0]) : std::copy(&values[0], &values[count_], &newValues[0]);
			values = std::move(newValues);
			
			capacity_ = newSize;
		}
		
		template <typename Container>
		void assignNewElements(const Container& elements) {
			
			count_ = std::distance(std::begin(elements), std::end(elements));
			
			if (count_ > 2) {
				assignMemoryForSize(count_);
			}
			std::copy(std::begin(elements), std::end(elements), &values[0]);
		}
		
		template <typename Container>
		void assignNewElements(Container&& elements) {
			
			count_ = std::distance(std::begin(elements), std::end(elements));
			
			if (count_ > 2) {
				assignMemoryForSize(count_);
			}
			std::move(std::begin(elements), std::end(elements), &values[0]);
		}
		
		template <typename Container>
		Array& appendNewElements(const Container& newElements) {
			
			std::size_t countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				std::copy(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {

				capacity_ = countOfContainer + count_;
				
				while (capacity_ < (count_ + countOfContainer)) {
					capacity_ += 2;
				}
				
				#if cplusplus14 && use_make_unique
					auto newValues = std::make_unique<Type[]>(capacity_);
				#elif cplusplus11 || !use_make_unique
					std::unique_ptr<Type[]> newValues (new Type[capacity_]);
				#endif
				
				std::copy(&values[0], &values[count_], &newValues[0]);
				std::copy(std::begin(newElements), std::end(newElements), &newValues[count_]);
	
				values = std::move(newValues);
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		
		template <typename Container>
		Array& appendNewElementsMOVE(Container&& newElements) {
			
			std::size_t countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				std::move(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {
				
				capacity_ = countOfContainer + count_;
				
				while (capacity_ < (count_ + countOfContainer)) {
					capacity_ += 2;
				}
				
				#if cplusplus14 && use_make_unique
					auto newValues = std::make_unique<Type[]>(capacity_);
				#elif cplusplus11 || !use_make_unique
					std::unique_ptr<Type[]> newValues (new Type[capacity_]);
				#endif

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
		
		inline void checkIfOutOfRange(const std::size_t index) const {
			if (index >= count_) {
				throw std::out_of_range("Index out of range");
			}
		}
		
	public:
		
		// MARK: Constructors
		
		Array() { }
		
		template<typename Container>
		Array(const Container& elements) { assignNewElements(elements); }
		
		template<typename Container>
		Array(Container&& elements) { assignNewElements(elements); }

		Array(const std::initializer_list<Type>& elements) { assignNewElements(elements); }
		Array(std::initializer_list<Type>&& elements) { assignNewElements(elements); }
		
		explicit Array(const Array& otherArray) { (*this) = otherArray; }
		explicit Array(Array&& otherArray) { (*this) = otherArray; }
		
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
				
				#if cplusplus14 && use_make_unique
					auto newValues = std::make_unique<Type[]>(capacity_);
				#elif cplusplus11 || !use_make_unique
					std::unique_ptr<Type[]> newValues (new Type[capacity_]);
				#endif

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
				
				#if cplusplus14 && use_make_unique
					auto newValues = std::make_unique<Type[]>(capacity_);
				#elif cplusplus11 || !use_make_unique
					std::unique_ptr<Type[]> newValues (new Type[capacity_]);
				#endif
				
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
		
		template<typename Container>
		inline void appendElements(const Container& newElements) { (*this) += newElements; }
		
		inline void append(const std::initializer_list<Type>& newElements) { (*this) += newElements; }
		
		template<typename Container>
		inline void appendElements(Container&& newElements) { (*this) += newElements; }
		
		inline void append(std::initializer_list<Type>&& newElements) { (*this) += newElements; }
		
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
			
			std::copy(&values[index + 1], &values[count_], &values[index]);
			
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
			
			for (const auto& elm: (*this)) {
				if (element == elm) { return true; }
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
		
		friend std::ostream& operator<<(std::ostream& os, const evt::Array<Type,initialCapacity>& arr) {
			return os << arr.toString();
		}
		
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
		inline bool operator==(const Container& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		template<typename Container>
		inline bool operator==(Container&& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		inline bool operator==(const std::initializer_list<Type>& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		inline bool operator==(std::initializer_list<Type>&& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		template<typename Container>
		inline bool operator!=(const Container& elements) const {
			return !( (*this) == elements );
		}
		
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
		
		Array& operator=(const Array& otherArray) {
			
			count_ = otherArray.count_;
			capacity_ = otherArray.capacity_;
			
			assignMemoryForSize(capacity_);
			std::copy(otherArray.begin(), otherArray.end(), &values[0]);
			
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
		
		inline Type first() const {
			checkIfEmpty();
			return *(&values[0]);
		}
		
		inline Type last() const {
			checkIfEmpty();
			return *(&values[count_]-1);
		}
	};
}

#undef sizeOfArrayInMB
#undef use_make_unique
#undef cplusplus14
#undef cplusplus11
#undef initialCapacity_
