#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <memory>
#include <unordered_map>

namespace se::utils {

	/**
	 * Class Repository, it provides a single point for storing and accessing
	 * to all the Elements of the given types
	 */
	class Repository
	{
	private:	// Nested types
		struct RepoTableTypes;
		template <typename KeyType, typename ValueType> struct RepoTableType;
		struct IRepoTable;
		template <typename KeyType, typename ValueType> struct RepoTable;
		using IRepoTableUPtr = std::unique_ptr<IRepoTable>;

	private:	// Attributes
		/** Maps each RepoTable type id with its respective RepoTable */
		std::unordered_map<std::size_t, IRepoTableUPtr> mRepoTables;

	public:		// Functions
		/** Initializes the Repo so it can hold elements of @tparam ValueType
		 * type indexed by @tparam KeyType
		 * @note it must be called only once before trying to
		 * add, remove or searching elements of the given types */
		template <typename KeyType, typename ValueType>
		void init();

		/** Adds the given element to the Repository
		 *
		 * @param	key the Key with which the new element can be identified in
		 *			the Repository
		 * @param	value the element to add to the Repository
		 * @return	a pointer to the new element, nullptr if it wasn't
		 *			inserted */
		template <typename KeyType, typename ValueType>
		std::shared_ptr<ValueType> add(
			const KeyType& key, std::unique_ptr<ValueType> value
		);

		/** Removes an element with the given key from the Repository
		 *
		 * @param	key the Key with which the element was added to the
		 *			Repository */
		template <typename KeyType, typename ValueType>
		void remove(const KeyType& key);

		/** Searchs an element with the given key value
		 *
		 * @param	key the key value used for searching
		 * @return	a pointer to the element found, nullptr if it wasn't
		 *			found */
		template <typename KeyType, typename ValueType>
		std::shared_ptr<ValueType> find(const KeyType& key);
	private:
		/** @return	a reference to the RepoTable with the given @tparam KeyType
		 *			and @tparam ValueType */
		template <typename KeyType, typename ValueType>
		RepoTable<KeyType, ValueType>& getRepoTable();
	};

}

#include "Repository.hpp"

#endif		// REPOSITORY_H
