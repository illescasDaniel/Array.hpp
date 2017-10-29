/*
 The MIT License (MIT)
 
 Copyright (c) 2017 Daniel Illescas Romero <https://github.com/illescasDaniel>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <random>
#include <functional>
#include <iterator>

#if (__cplusplus >= 201406)
	#include <experimental/optional>
#endif

#if (__cplusplus > 201103)
	#define CONSTEXPR constexpr
#else
	#define CONSTEXPR inline
#endif

namespace evt {
	
	namespace internalArrayPrintEVT {
		
		// Extra functions for the "toString()" method
		inline std::string to_string(const std::string& str) { return str; }
		inline std::string to_string(const char chr) { return std::string(1,chr); }
		
		template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value,bool>::type>
		inline std::string to_string(const Arithmetic& arithmeticValue) {
			
			if (std::is_same<Arithmetic, bool>::value) {
				return arithmeticValue ? "true" : "false";
			}
			
			return std::to_string(arithmeticValue);
		}
		/* Place your custom "to_string()" function/s here for other classes. Use templates if you want. */
	}
	
	// MARK: - Array Class
	
	template <typename Type>
	class Array {
		
		// Types and macros
		typedef std::size_t SizeType;
		typedef std::unique_ptr<Type[]> Pointer;
		typedef std::initializer_list<Type> InitializerList;
		
		// MARK: - Attributes
		
		Pointer values { new Type[2] };
		SizeType count_ { 0 };
		SizeType capacity_ { 2 };
		
		// MARK: - Private Functions
	
		/// Assigns new memory, also updates the new capacity.
		inline void assignMemoryAndCapacityForSize(SizeType newSize, bool forceResize = false) {
			if (forceResize or capacity_ < newSize) {
				values = std::move(Pointer { new Type[newSize] });
				capacity_ = newSize;
			}
		}
		
		/// Resizes the array to a given size
		void resizeValuesToSize(const SizeType newSize) {
			
			Pointer newValues { new Type[newSize] };
			std::move(&values[0], &values[count_], &newValues[0]);
			values = std::move(newValues);
			
			capacity_ = newSize;
		}
		
		template <typename MagicContainer>
		void assignNewMagicElements(MagicContainer&& elements, const std::size_t initialCapacity = 2) {
			count_ = std::distance(std::begin(elements), std::end(elements));
			assignMemoryAndCapacityForSize((count_ > initialCapacity) ? count_ : initialCapacity);
			assignNewElements(std::forward<MagicContainer>(elements));
		}
		
		/// Replaces the content of the array with other elements
		template <typename Container>
		void assignNewElements(const Container& elements) {
			std::copy(std::begin(elements), std::end(elements), &values[0]);
		}
		
		template <typename Container>
		void assignNewElements(Container&& elements) {
			std::move(std::begin(elements), std::end(elements), &values[0]);
		}
		
		template <typename Container>
		Array& appendNewElements(const Container& newElements) {
			
			SizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				std::copy(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {
				
				capacity_ = countOfContainer + count_;
				
				Pointer newValues { new Type[capacity_] };
				
				std::copy(&values[0], &values[count_], &newValues[0]);
				std::copy(std::begin(newElements), std::end(newElements), &newValues[count_]);
				
				values = std::move(newValues);
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		
		template <typename Container>
		Array& appendNewElementsMOVE(Container&& newElements) {
			
			SizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (capacity_ >= (count_ + countOfContainer)) {
				std::move(std::begin(newElements), std::end(newElements), &values[count_]);
			}
			else if (countOfContainer > 0) {
				
				capacity_ = countOfContainer + count_;
				
				Pointer newValues { new Type[capacity_] };
				
				std::move(&values[0], &values[count_], &newValues[0]);
				std::move(std::begin(newElements), std::end(newElements), &newValues[count_]);
				
				values = std::move(newValues);
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		
		template <typename Container>
		Array& removeElementsFromContainer(const Container& newElements, bool onlyFirstOcurrence = false) {
			
			SizeType elementsFound = 0;
			SizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			std::unique_ptr<SizeType[]> elementsPosition(new SizeType[countOfContainer]);
			
			auto newElement = std::begin(newElements);
			
			do {
				elementsFound = 0;
				newElement = std::begin(newElements);
				
				for (SizeType index = 0; index < count_; ++index) {
					
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
					for (SizeType i = 0; i < newElements.size(); ++i) {
						this->removeAt(elementsPosition[0]);
					}
				}
			} while((elementsFound == newElements.size()) && !onlyFirstOcurrence);
			
			return *this;
		}
		
		inline void checkIfEmpty() const {
			if (count_ == 0) {
				throw std::length_error("Array is empty (lenght == 0)");
			}
		}
		
		inline void checkIfOutOfRange(const SizeType index) const {
			if (index >= count_) {
				throw std::out_of_range("Index out of range");
			}
		}
		
		void assignArrayWithOptionalInitialCapacity(const Array& otherArray, const size_t initialCapacity = 2) {
			if (this != &otherArray) {
				count_ = otherArray.count();
				SizeType capacity = (otherArray.count() > initialCapacity) ? otherArray.count() : initialCapacity;
				assignMemoryAndCapacityForSize(capacity);
				std::copy(otherArray.begin(), otherArray.end(), &values[0]);
			}
		}
		
		void assignArrayWithOptionalInitialCapacity(Array&& otherArray, const size_t initialCapacity = 2) {
			if (this != &otherArray) {
				count_ = otherArray.count();
				SizeType capacity = (otherArray.count() > initialCapacity) ? otherArray.count() : initialCapacity;
				assignMemoryAndCapacityForSize(capacity);
				std::move(otherArray.begin(), otherArray.end(), &values[0]);
			}
		}
		
	public:
		
		// MARK: Constructors
		
		Array() { }
		Array(const int initialCapacity) { assignMemoryAndCapacityForSize((initialCapacity > 0) ? initialCapacity : 2); }
		Array(const SizeType initialCapacity) { assignMemoryAndCapacityForSize(initialCapacity); }
		Array(InitializerList&& elements, SizeType initialCapacity = 2) { assignNewMagicElements(elements, initialCapacity); }
		Array(const Array& otherArray, SizeType initialCapacity = 2) { assignArrayWithOptionalInitialCapacity(otherArray, initialCapacity); }
		Array(Array&& otherArray, SizeType initialCapacity = 2) { assignArrayWithOptionalInitialCapacity(std::move(otherArray), initialCapacity); }
		Array(const SizeType count, const Type& initialValue) {
			assignMemoryAndCapacityForSize(count);
			this->count_ = count;
			Type n {initialValue};
			std::generate(this->begin(), this->end(), [&n]{ return n++; });
		}
		
		template <typename Container, typename = typename std::enable_if<
		!std::is_same<Container,Array>::value &&
		!std::is_same<Container,Type>::value &&
		!std::is_arithmetic<Container>::value>::type>
		Array(Container&& elements, SizeType initialCapacity = 2) { assignNewMagicElements(elements, initialCapacity); }
		
		// MARK: Capacity
		
		inline SizeType size() const  { return count_; }
		inline SizeType count() const { return count_; }
		inline SizeType capacity() const { return capacity_; }
		
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
				this->append(std::move(newElement));
			}
			else if (position == &values[0]) {
				this->insert(std::move(newElement), 0);
			}
			else if (position > &values[0] && position < &values[count_]) {
				this->insert(std::move(newElement), position - &values[0]);
			}
			else {
				throw std::out_of_range("Index out of range");
			}
		}
		
		void insert(const Type& newElement, const SizeType index, const SizeType capacityResizeFactor = 4) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (capacity_ == count_) {
				
				capacity_ *= capacityResizeFactor;
				
				Pointer newValues { new Type[capacity_] };
				
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
		
		void insert(Type&& newElement, const SizeType index, const SizeType capacityResizeFactor = 4) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(std::move(newElement));
				return;
			}
			
			if (capacity_ == count_) {
				
				capacity_ *= capacityResizeFactor;
				
				Pointer newValues { new Type[capacity_] };
				
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
		void assignRange(SizeType first, SizeType last) {
			
			if (first > last) {
				checkIfOutOfRange(first);
				SizeType auxFirst = last;
				last = first;
				first = auxFirst;
			}
			checkIfOutOfRange(last);
			
			std::copy(&values[first], &values[last+1], &values[0]);
			
			SizeType newCount = last-first + 1;
			
			if (newCount < (count_ / 4.0)) {
				resize(newCount);
			} else {
				count_ = newCount;
			}
		}
		
		void append(const Type& newElement, const SizeType capacityResizeFactor = 4) {

			if (capacity_ == count_) {
				resizeValuesToSize(capacity_ * capacityResizeFactor);
			}
			
			values[count_] = newElement;
			count_ += 1;
		}
		
		void append(Type&& newElement, const SizeType capacityResizeFactor = 4) {

			if (capacity_ == count_) {
				resizeValuesToSize(capacity_ * capacityResizeFactor);
			}
			
			values[count_] = std::move(newElement);
			count_ += 1;
		}
		
		inline void append(InitializerList&& newElements) { appendNewElements(newElements); }
		inline void appendElements(InitializerList&& newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		inline void appendElements(const Container& newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		inline void appendElements(Container&& newElements) { appendNewElementsMOVE(std::move(newElements)); }
		
		/// Only reserves new memory if the new size if bigger than the array capacity
		void reserve(const SizeType newSize) {
			
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
		void resize(const SizeType newSize) {
			
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
		CONSTEXPR void removeAll(const bool keepCapacity = false) {
			if (!keepCapacity) {
				assignMemoryAndCapacityForSize(2, true);
			}
			count_ = 0;
		}
		
		inline void removeAt(const SizeType index, const bool shrinkIfEmpty = true) {
			
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
		
		void removeSubrange(const SizeType startPosition, const SizeType endPosition, bool lessEqual = true) {
			for (SizeType i = startPosition; lessEqual ? (i <= endPosition) : (i < endPosition); ++i) {
				this->removeAt(startPosition);
			}
		}
		
		void removeSubrange(std::initializer_list<int> position, bool lessEqual = true) {
			
			SizeType startPosition = *std::begin(position);
			SizeType endPosition = *(std::end(position)-1);
			
			for (SizeType i = startPosition; lessEqual ? (i <= endPosition) : (i < endPosition); ++i) {
				this->removeAt(startPosition);
			}
		}
		
		void swap(Array& otherArray) {
			
			Pointer auxValues = std::move(this->values);
			this->values = std::move(otherArray.values);
			otherArray.values = std::move(auxValues);
			
			std::swap(this->count_, otherArray.count_);
			std::swap(this->capacity_, otherArray.count_);
		}
		
		template <typename Container>
		Array& removeElements(const Container& newElements, bool onlyFirstOcurrence = false) {
			return removeElementsFromContainer(newElements, onlyFirstOcurrence);
		}
		
		Array& removeElements(InitializerList newElements, bool onlyFirstOcurrence = false) {
			return removeElementsFromContainer(newElements, onlyFirstOcurrence);
		}
		
		template <typename Container>
		void swap(Container& container) {
			Array otherArray(container);
			this->swap(otherArray);
			container = Array::to<Container>(otherArray);
		}
		
		inline bool contains(const Type& element) const {
			for (const Type& elm: (*this)) {
				if (element == elm) { return true; }
			}
			/*for (size_t i = 0; i < count_; i++) {
				if (values[i] == element) { return true; }
			}*/
			return false;
		}
		
		/// Returns the index of the first ocurrence of the element. Last position if the element isn't found
		SizeType find(const Type& element) const {
			return (std::find(&values[0], &values[count_], element) - &values[0]);
		}
		
		SizeType findIf(std::function<bool(const Type&)> findFunction) const {
			return (std::find_if(&values[0], &values[count_], findFunction) - &values[0]);
		}
		
		/// Returns an Array with all the ocurrences of the element
		Array<SizeType> findAll(const Type& element) const {
			
			Array<SizeType> positions;
			SizeType position = 0;
			
			for (const auto& value: (*this)) {
				if (element == value) {
					positions.append(position);
				}
				position += 1;
			}
			
			return positions;
		}
		
		std::string toString() const {
			
			if (this->isEmpty()) {
				return "[]";
			}
			
			std::string output = "[";
			SizeType position = 0;
			
			for (const auto& value: *this) {
				output += [&] {
					if (typeid(value) == typeid(std::string)) {
						return ("\"" + evt::internalArrayPrintEVT::to_string(value) + "\"");
					} else if (typeid(value) == typeid(char)) {
						return ("\'" + evt::internalArrayPrintEVT::to_string(value) + "\'");
					}
					return evt::internalArrayPrintEVT::to_string(value);
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
		friend std::ostream& operator<<(std::ostream& os, const evt::Array<Type>& arr) {
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
		
		bool equal(InitializerList&& elements) const {
			return this->operator==(elements);
		}
		
		Array filter(std::function<bool(const Type&)> filterFunction) const {
			Array filteredArray;
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					filteredArray.append(element);
				}
			}
			return filteredArray;
		}
		
		template <typename MapType>
		Array<MapType> map(std::function<MapType(const Type&)> mapFunctor) const {
			Array<MapType> mappedArray(this->count());
			for (const auto& element: *this) {
				mappedArray.append(mapFunctor(element));
			}
			return mappedArray;
		}
		
		template <typename ReduceType>
		ReduceType reduce(std::function<ReduceType(const ReduceType result, const Type&)> reduceFunctor, ReduceType initialValue = ReduceType()) const {
			ReduceType reducedArrayValue{initialValue};
			for (const auto& element: *this) {
				reducedArrayValue = reduceFunctor(reducedArrayValue, element);
			}
			return reducedArrayValue;
		}
		
		inline size_t sum() const {
			return this->reduce<size_t>([](const size_t result, const Type& value){ return result + value; });
		}
		
		inline double mean() const {
			return double(this->sum()) / double(this->count());
		}
		
		SizeType countOf(std::function<bool(const Type&)> countOfFunction) const {
			return std::count_if(this->begin(), this->end(), countOfFunction);
		}
		
		SizeType countOf(const Type& value) const {
			return std::count(this->begin(), this->end(), value);
		}
		
#if (__cplusplus >= 201406)
		
		inline std::experimental::optional<Type> first(std::function<bool(const Type&)> filterFunction) const {
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					return element;
				}
			}
			return std::experimental::nullopt;
		}
		
		inline std::experimental::optional<Type> last(std::function<bool(const Type&)> filterFunction) const {
			
			std::experimental::optional<Type> optElement;
			
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					optElement = element;
				}
			}
			return optElement;
		}
		
		inline std::experimental::optional<Type> at(const SizeType index) const {
			if (index >= count_) {
				return std::experimental::nullopt;
			} else {
				return values[index];
			}
		}
#endif
		
		// MARK: Operators overload
		
		inline Type& operator[](const SizeType index) {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		inline const Type& operator[](const SizeType index) const {
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
			return appendNewElementsMOVE(std::move(newElements));
		}
		
		inline Array& operator+=(InitializerList newElements) {
			return appendNewElements(newElements);
		}
		
		template <typename Container>
		inline bool operator==(const Container& elements) const {
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			if (count_ != countOfContainer) { return false; }
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		template <typename Container>
		inline bool operator!=(const Container& elements) const {
			return !( (*this) == elements );
		}
		
		/// Returns true if the contents of the array are lexicographically less than the contents of the container
		template <typename Container>
		inline bool operator<(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement < *containerElement;
				}
			}
			
			return count_ < countOfContainer;
		}
		
		template <typename Container>
		inline bool operator<=(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement < *containerElement;
				}
			}
			
			return count_ <= countOfContainer;
		}
		
		/// Returns true if the contents of the array are lexicographically greater than the contents of the container
		template <typename Container>
		inline bool operator>(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement > *containerElement;
				}
			}
			
			return count_ > countOfContainer;
		}
		
		template <typename Container>
		inline bool operator>=(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement > *containerElement;
				}
			}
			
			return count_ >= countOfContainer;
		}
		
		Array& operator=(const Array& otherArray) {
			
			if (this != &otherArray) {
				count_ = otherArray.count_;
				
				assignMemoryAndCapacityForSize(otherArray.capacity_);
				std::copy(otherArray.begin(), otherArray.end(), &values[0]);
			}
			
			return *this;
		}
		
		Array& operator=(Array&& otherArray) {
			
			if (this != &otherArray) {
				count_ = otherArray.count_;
				
				assignMemoryAndCapacityForSize(otherArray.capacity_);
				std::move(otherArray.begin(), otherArray.end(), &values[0]);
			}
			
			return *this;
		}
		
		void moveFrom(Array&& otherArray) {
			
			if (this != &otherArray) {
				values = std::move(otherArray.values);
				capacity_ = otherArray.capacity_;
				count_ = otherArray.count_;
				
				otherArray.capacity_ = 0;
				otherArray.count_ = 0;
			}
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

#undef CONSTEXPR
