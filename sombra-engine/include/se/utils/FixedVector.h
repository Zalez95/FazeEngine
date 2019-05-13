#ifndef FIXED_VECTOR_H
#define FIXED_VECTOR_H

#include <iterator>
#include <initializer_list>

namespace se::utils {

	/**
	 * Class FixedVector, a FixedVector is a container with a compile-time
	 * fixed size but which can vary the number of elements stored inside.
	 * Internally the vector stores the elements contiguously so they could be
	 * accessed with pointers and offsets. The FixedVector won't do any
	 * allocation at runtime.
	 */
	template <typename T, std::size_t N>
	class FixedVector
	{
	public:		// Nested types
		using size_type					= std::size_t;
		using difference_type			= std::ptrdiff_t;
		using value_type				= T;
		using pointer					= T*;
		using reference					= T&;
		using iterator					= T*;
		using const_iterator			= const T*;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

	private:	// Attributes
		/** The array where the elements will be stored */
		T mElements[N];

		/** The number of Elements added to the FixedVector */
		size_type mNumElements;

	public:		// Functions
		/** Creates a new FixedVector
		 *
		 * @param	numElements the initial number of elements stored inside
		 *			the FixedVector. If it's larger than the maximum number of
		 *			elements the FixedVector can store, it will be set to 0 */
		FixedVector(size_type numElements = 0) : mNumElements(0)
		{ resize(numElements); };

		/** Creates a new FixedVector
		 *
		 * @param	list the initial elements stored inside the FixedVector.
		 * @note	it will be stored up to N elements */
		FixedVector(std::initializer_list<T> list);

		/** Returns the Element i of the FixedVector
		 *
		 * @param	i the index of the Element
		 * @return	a reference to the Element */
		T& operator[](size_type i) { return mElements[i]; };

		/** Returns the Element i of the FixedVector
		 *
		 * @param	i the index of the Element
		 * @return	a const reference to the Element */
		const T& operator[](size_type i) const { return mElements[i]; };

		/** Compares the given FixedVectors
		 *
		 * @param	fv1 the first FixedVector to compare
		 * @param	fv2 the second FixedVector to compare
		 * @return	true if both FixedVector are equal, false otherwise */
		template <typename U, std::size_t N2>
		friend bool operator==(
			const FixedVector<U, N2>& fv1, const FixedVector<U, N2>& fv2
		);

		/** Compares the given FixedVectors
		 *
		 * @param	fv1 the first FixedVector to compare
		 * @param	fv2 the second FixedVector to compare
		 * @return	true if both FixedVector are different, false otherwise */
		template <typename U, std::size_t N2>
		friend bool operator!=(
			const FixedVector<U, N2>& fv1, const FixedVector<U, N2>& fv2
		);

		/** @return	the initial iterator of the FixedVector */
		iterator begin() { return &mElements[0]; };

		/** @return	the initial iterator of the FixedVector */
		const_iterator begin() const { return &mElements[0]; };

		/** @return	the final iterator of the FixedVector */
		iterator end() { return &mElements[0] + mNumElements; };

		/** @return	the final iterator of the FixedVector */
		const_iterator end() const { return &mElements[0] + mNumElements; };

		/** @return	the initial reverse iterator of the FixedVector */
		reverse_iterator rbegin() { return &mElements[0] + mNumElements; };

		/** @return	the final reverse iterator of the FixedVector */
		const_reverse_iterator rbegin() const
		{ return &mElements[0] + mNumElements; };

		/** @return	the final reverse iterator of the FixedVector */
		reverse_iterator rend() { return &mElements[0]; };

		/** @return	the initial reverse iterator of the FixedVector */
		const_reverse_iterator rend() const { return &mElements[0]; };

		/** @return	the number of Elements in the FixedVector */
		size_type size() const { return mNumElements; };

		/** @return	the maximum number of Elements that can be stored in the
		 *			FixedVector */
		size_type capacity() const { return N; };

		/** @return	true if the FixedVector has no Elements inside, false
		 *			otherwise */
		bool empty() const { return (mNumElements == 0); };

		/** @return	true if the FixedVector has reached its maximum number of
		 *			elements inside */
		bool full() const { return (mNumElements == N); };

		/** @return	a reference to the first element in the FixedVector
		 * @note	if the FixedVector is empty, it causes undefined behavior */
		T& front() { return mElements[0]; };

		/** @return	a reference to the first element in the FixedVector
		 * @note	if the FixedVector is empty, it causes undefined behavior */
		const T& front() const { return mElements[0]; };

		/** @return	a reference to the last element in the FixedVector
		 * @note	if the FixedVector is empty, it causes undefined behavior */
		T& back() { return mElements[mNumElements - 1]; };

		/** @return	a reference to the last element in the FixedVector
		 * @note	if the FixedVector is empty, it causes undefined behavior */
		const T& back() const { return mElements[mNumElements - 1]; };

		/** @return	a pointer to the first element stored in the FixedVector */
		T* data() { return &mElements[0]; };

		/** @return	a pointer to the first element stored in the FixedVector */
		const T* data() const { return &mElements[0]; };

		/** Removes all the elements in the FixedVector */
		void clear();

		/** Changes the number of elements stored in the FixedVector
		 *
		 * @param	numElements the new number of elements stored inside the
		 *			FixedVector
		 * @note	if the new number of elements is larger than the maximum
		 *			number of elements the FixedVector can hold, this function
		 *			will have no effect */
		void resize(size_type numElements);

		/** Adds a new Element at the back of the FixedVector
		 *
		 * @param	element the Element to push at the back of the FixedVector
		 * @return	an iterator to the element
		 * @note	if the FixedVector is full, it causes undefined behavior */
		iterator push_back(T element);

		/** Adds a new Element at the back of the FixedVector
		 *
		 * @param	args the arguments needed for calling the constructor of
		 *			the new Element
		 * @return	an iterator to the element
		 * @note	if the FixedVector is full, it causes undefined behavior */
		template <typename... Args>
		iterator emplace_back(Args&&... args);

		/** Removes the last Element of the FixedVector
		 * @note	if the FixedVector is empty, it causes undefined behavior */
		void pop_back();

		/** Removes the element located at the given iterator from the
		 * FixedVector
		 *
		 * @param	it an iterator to the Element
		 * @note	the next stored elements will be displaced one position
		 *			left inside the FixedVector */
		iterator erase(const_iterator it);
	};

}

#include "FixedVector.inl"

#endif		// FIXED_VECTOR_H
