#ifndef FIXED_VECTOR_INL
#define FIXED_VECTOR_INL

namespace se::utils {

	template <typename T, std::size_t N>
	FixedVector<T, N>::FixedVector(std::initializer_list<T> list) :
		mNumElements( (N < list.size())? N : list.size() )
	{
		size_type i = 0;
		for (const T& member : list) {
			if (i < mNumElements) {
				mElements[i] = member;
			}
			++i;
		}
	}


	template <typename T, std::size_t N>
	bool operator==(const FixedVector<T, N>& fv1, const FixedVector<T, N>& fv2)
	{
		bool equal = true;

		if (fv1.mNumElements == fv2.mNumElements) {
			for (auto i = 0; i < fv1.mNumElements; ++i) {
				if (fv1.mElements[i] != fv2.mElements[i]) {
					equal = false;
					break;
				}
			}
		}
		else {
			equal = false;
		}

		return equal;
	}


	template <typename T, std::size_t N>
	bool operator!=(const FixedVector<T, N>& fv1, const FixedVector<T, N>& fv2)
	{
		return !(fv1 == fv2);
	}


	template <typename T, std::size_t N>
	void FixedVector<T, N>::clear()
	{
		resize(0);
	}


	template <typename T, std::size_t N>
	void FixedVector<T, N>::resize(size_type numElements)
	{
		if (numElements <= N) {
			mNumElements = numElements;
		}
	}


	template <typename T, std::size_t N>
	typename FixedVector<T, N>::iterator FixedVector<T, N>::push_back(T element)
	{
		mElements[mNumElements] = element;
		mNumElements++;

		return &mElements[mNumElements - 1];
	}


	template <typename T, std::size_t N>
	template <typename... Args>
	typename FixedVector<T, N>::iterator FixedVector<T, N>::emplace_back(Args&&... args)
	{
		mElements[mNumElements] = T(std::forward<Args>(args)...);
		mNumElements++;

		return &mElements[mNumElements - 1];
	}


	template <typename T, std::size_t N>
	void FixedVector<T, N>::pop_back()
	{
		mNumElements--;
	}


	template <typename T, std::size_t N>
	typename FixedVector<T, N>::iterator FixedVector<T, N>::erase(const_iterator it)
	{
		if (it != end()) {
			for (iterator it2 = it + 1; it2 != end(); ++it2) {
				*(it2 - 1) = *it2;
			}
			mNumElements--;
		}

		return it;
	}

}

#endif		// FIXED_VECTOR_INL
