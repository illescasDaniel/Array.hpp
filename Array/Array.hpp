#pragma once

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <iterator>
#include <memory>
#include <string>
#include <random>

// Extra functions for the "toString()" method
namespace std {
	inline std::string to_string(const std::string& str) { return str; }
	inline std::string to_string(const char chr) { return std::string(1,chr); }
	/* Custom to_string()
	 
	 template <typename MyClass>
	 inline std::string to_string(const MyClass& object) {
		return object.value + ... ;
	 }
	 */
}

namespace evt {
	
	// MARK: - Array Class
	template <typename Type>
	class Array {
		
		// MARK: - Attributes
		
		std::unique_ptr<Type[]> values { nullptr };
		size_t count_ { 0 };
		size_t capacity_ { 0 };
		
		// MARK: - Private functions
		
		template <typename Container>
		void assignNewElements(const Container& elements) {
			count_ = std::distance(std::begin(elements), std::end(elements));
			capacity_ = count_;
			
			#if __cplusplus >= 201400
				values = std::make_unique<Type[]>(count_);
			#elif __cplusplus >= 201100
				values = std::unique_ptr<Type[]>(new Type[count_]);
			#endif

			std::move(std::begin(elements), std::end(elements), &values[0]);
		}
		
		template <typename Container>
		Array& appendNewElements(const Container& newElements) {
			
			size_t countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				
				std::move(std::begin(newElements), std::end(newElements), &values[count_]);
				count_ += countOfContainer;
			}
			else {
				
				size_t newCount = (this->isEmpty()) ? (countOfContainer) : (count_ * 2);
				
				while (newCount < (count_ + countOfContainer)) {
					newCount += 2;
				}
				
				#if __cplusplus >= 201400
					auto newValues = std::make_unique<Type[]>(newCount);
				#elif __cplusplus >= 201100
					std::unique_ptr<Type[]> newValues (new Type[newCount]);
				#endif
				
				if (count_ > 0) {
					std::copy(&values[0], &values[count_], &newValues[0]);
				}

				std::move(std::begin(newElements), std::end(newElements), &newValues[count_]);
				values = move(newValues);
				
				capacity_ = newCount;
				count_ += countOfContainer;
			}
			
			return *this;
		}
		
		inline void checkIfEmpty() const {
			if (count_ == 0) {
				throw std::length_error("Array is empty (lenght == 0)");
			}
		}
		
		inline void checkIfOutOfRange(const size_t index) const {
			if (index >= count_) {
				throw std::out_of_range("Index out of range");
			}
		}
		
	public:
		
		// MARK: Constructors
		
		Array() {}
		
		template<typename Container>
		Array(const Container& elements) { assignNewElements(elements); }
		
		Array(const std::initializer_list<Type>& elements) { assignNewElements(elements); }
		
		explicit Array(const Array& otherArray) { (*this) = otherArray; }
		
		explicit Array(const int32_t capacity): capacity_(capacity) { // Type can't be size_t because it intefere with the other constructor
			
			const size_t intialCapacity = (capacity < 0) ? (- capacity) : (capacity);
			
			#if __cplusplus >= 201400
				values = std::make_unique<Type[]>(intialCapacity);
			#elif __cplusplus >= 201100
				values = std::unique_ptr<Type[]>(new Type[intialCapacity]);
			#endif
		}
		
		inline size_t count() const { return count_; }
		inline size_t capacity() const { return capacity_; }
		
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
		
		void insert(const Type& newElement, const size_t index) {
			
			if (index == count_ - 1 || this->isEmpty()) {
				this->append(newElement);
				return;
			}

			checkIfOutOfRange(index);
			
			if (capacity_ == count_) {
				
				capacity_ = count_ * 2;
				
				#if __cplusplus >= 201400
					auto newValues = std::make_unique<Type[]>(capacity_);
				#elif __cplusplus >= 201100
					std::unique_ptr<Type[]> newValues(new Type[capacity_]);
				#endif
				
				std::copy(&values[0], &values[count_], &newValues[0]);
				std::copy(&newValues[index], &newValues[count_], &newValues[index + 1]);
				
				newValues[index] = newElement;
				values = move(newValues);
			}
			else {
				std::copy(&values[index], &values[count_], &values[index + 1]);
				values[index] = newElement;
			}
			
			count_ += 1;
		}
		
		void append(const Type& newElement) {
			
			if (capacity_ == count_) {
				
				const size_t newCount = (this->isEmpty()) ? (2) : (count_ * 2);
				
				#if __cplusplus >= 201400
					auto newValues = std::make_unique<Type[]>(newCount);
				#elif __cplusplus >= 201100
					std::unique_ptr<Type[]> newValues (new Type[newCount]);
				#endif
				
				if (count_ > 0) {
					std::copy(&values[0], &values[count_], &newValues[0]);
				}
				
				newValues[count_] = newElement;
				values = move(newValues);
				
				capacity_ = newCount;
			}
			else {
				values[count_] = newElement;
			}
			
			count_ += 1;
		}
		
		template<typename Container>
		inline void appendElements(const Container& newElements) { (*this) += newElements; }
		inline void append(const std::initializer_list<Type>& newElements) { (*this) += newElements; }
		
		void resize(const size_t newSize) {
			
			if (newSize == 0 && count_ > 0) {
				removeAll();
			}
			
			if (newSize < capacity_) {
				count_ = newSize;
			}
			else if (newSize > capacity_) {
				
				#if __cplusplus >= 201400
					auto newValues = std::make_unique<Type[]>(newSize);
				#elif __cplusplus >= 201100
					std::unique_ptr<Type[]> newValues(new Type[newSize]);
				#endif
				
				if (!this->isEmpty()) {
					std::copy(&values[0], &values[count_], &newValues[0]);
				}
				
				values = move(newValues);
				capacity_ = newSize;
			}
		}
		
		bool shrink() {
			
			if (capacity_ > count_) {
				
				#if __cplusplus >= 201400
					auto newValues = std::make_unique<Type[]>(count_);
				#elif __cplusplus >= 201100
					std::unique_ptr<Type[]> newValues (new Type[count_]);
				#endif
				
				if (count_ > 0) {
					std::copy(&values[0], &values[count_], &newValues[0]);
				}
				
				values = move(newValues);
				
				capacity_ = count_;
				
				return true;
			}
			
			return false;
		}
		
		inline void clear() {
			removeAll(true);
		}
		
		void removeAll(const bool keepCapacity = false) {
			
			if (!keepCapacity && values != nullptr) {
				
				#if __cplusplus >= 201400
					values = std::make_unique<Type[]>(0);
				#elif __cplusplus >= 201100
					values = std::unique_ptr<Type[]>(new Type[0]);
				#endif

				capacity_ = 0;
			}
			
			count_ = 0;
		}
		
		void removeAt(const size_t index, const bool shrinkIfEmpty = true) {
			
			if (count_ == 2 && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			checkIfOutOfRange(index);
			
			std::copy(&values[index + 1], &values[count_], &values[index]);
			
			count_ -= 1;
		}
		
		void removeLast(const bool shrinkIfEmpty = true) {
			
			if (count_ == 2 && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			
			count_ -= 1;
		}
		
		inline void removeFirst() {
			removeAt(0);
		}

		bool contains(const Type& element) const {
			checkIfEmpty();
			return (std::find(&values[0], &values[count_], element)) != &values[count_];
		}
		
		inline bool isEmpty() const {
			return (count_ == 0);
		}
		
		std::string toString() const {
		
			if (this->isEmpty()) {
				return "[]";
			}
			
			std::string output = "[";
			
			size_t position = 0;
			for (const auto& value: *this) {
				
				output += [&](){
					if (typeid(value) == typeid(std::string)) {
						return ("\"" + std::to_string(value) + "\"");
					} else if (typeid(value) == typeid(char)) {
						return ("\'" + std::to_string(value) + "\'");
					} else {
						return std::to_string(value);
					}
				}();
				
				if (position+1 < count_) {
					output += ", ";
				}
				
				position += 1;
			}
			
			output += "]";
			
			return output;
		}
		
		// Convert Array to other types
		template <typename Container>
		static Container to(const Array<Type>& elements) {
			Container cont(elements.count());
			std::copy(elements.begin(), elements.end(), std::begin(cont));
			return cont;
		}
		
		// MARK: Operators overload
		
		inline Type& operator[](const size_t index) {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		template<typename Container>
		inline Array& operator+=(const Container& newElements) {
			return appendNewElements(newElements);
		}
		
		inline Array& operator+=(const std::initializer_list<Type>& newElements) {
			return appendNewElements(newElements);
		}
		
		template<typename Container>
		inline bool operator==(const Container& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		inline bool operator==(const std::initializer_list<Type>& elements) const {
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		template<typename Container>
		inline bool operator!=(const Container& elements) const {
			return !( (*this) == elements );
		}
		
		inline bool operator!=(const std::initializer_list<Type>& elements) const {
			return !( (*this) == elements );
		}
		
		Array& operator=(const Array& otherArray) {
			
			count_ = otherArray.count_;
			capacity_ = otherArray.capacity_;
			
			#if __cplusplus >= 201400
				values = std::make_unique<Type[]>(capacity_);
			#elif __cplusplus >= 201100
				values = std::unique_ptr<Type[]> (new Type[capacity_]);
			#endif
			
			std::copy(otherArray.begin(), otherArray.end(), &values[0]);
			
			return *this;
		}
		
		Array& operator=(Array&& otherArray) {
			
			count_ = std::move(otherArray.count_);
			capacity_ = std::move(otherArray.capacity_);
			
			values = move(otherArray.values);
			otherArray.values = nullptr;
			
			otherArray.count_ = 0;
			otherArray.capacity_ = 0;
			
			return *this;
		}
		
		// MARK: Shuffle
		
		void shuffle() {
			
			checkIfEmpty();
			
			#ifdef __APPLE__
				std::mt19937_64 rng(arc4random());
			#else
				std::random_device rd;
				std::mt19937_64 rng(rd());
			#endif

			std::shuffle(&values[0], &values[count_], rng);
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
			std::sort(&values[0], &values[count_]);
		}
		
		template <typename Function>
		void sort(const Function& compareFunction) {
			checkIfEmpty();
			std::sort(&values[0], &values[count_], compareFunction);
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
