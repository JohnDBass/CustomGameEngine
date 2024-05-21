#pragma once
#include "framework.h"
#include "RTTI.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "Datum.h"

//class Datum;

namespace Fiea::GameEngine
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);
	public:

		/**
		 * @brief Pre-allocate capacity for this Scope's map and vector at construction
		 * @param initialCapacity
		*/
		Scope(size_t initialCapacity = 0);
		/**
		 * @brief Lval copy constructor - Deep copies all Datum containers 'owned' by the other scope.
		 * To copy table datums, a new Scope is copy constructed from each Scope 'contained' in that Datum and then adopted by the current Scope.
		 * All non-Table datums are default constructed at the appropriate key with Append, and their contents copied from other using an assignment op.
		 * Will not copy other's parent scope, so any copy constructed Scope forms a new root Scope.
		 * @param other Lval other Scope
		*/
		Scope(const Scope& other);
		/**
		 * @brief Rval copy constructor - Uses move semantics to 'steal' other's map and vec, calling std::vec's and std::un_map's move assignment ops.
		 * Then reparents any child Scopes
		 * @param other Rval other Scope
		*/
		Scope(Scope&& other) noexcept;
		/**
		 * @brief Effectively the Lval copy constructor with the added step of clearing any contained data in this Scope.
		 * @param other Lval other Scope
		 * @return
		*/
		Scope& operator=(const Scope& other);
		/**
		 * @brief Just like the move constructor, but will first clear any contained data.
		 * @param other Rval other Scope
		 * @return
		*/
		Scope& operator=(Scope&& other) noexcept;

		/**
		 * @brief Calls CLear() and Orphan() if necessary on this scope
		*/
		virtual ~Scope();

		/**
		 * @brief Deep copies this Scope and provides a pointer to the copy
		 * @return 
		*/
		[[nodiscard]] virtual Scope* Clone() const;

		/**
		 * @brief Scopes are considered equivalent if their datums (and the data contained in those datums) are equivalent and
		 * contained at the same index of their respective data containers. They do not need to have the same parents.
		 * @param rhs The Scope to compare against
		 * @return
		*/
		bool operator==(const Scope& rhs) const;
		bool operator!=(const Scope& rhs) const;

		/**
		 * @brief Given a string key, Find will find the most closely nested Datum associated with that key, if it exists.
		 * Uses std::unordered_map.find() for this purpose, which will return an end() iterator if the key is not found in the map.
		 * Returns a pointer to the found Datum or a nullptr if the Datum is not found.
		 * @param key String key passed by ref
		 * @return
		*/
		Datum* Find(const std::string& key);
		const Datum* Find(const std::string& key) const;

		/**
		 * @brief Given a ptr to a Scope, will search this Scope's containers for that child.
		 * Returns a ptr to the Datum containing that child, or null if the child is not found.
		 * @param child
		 * @param idx
		 * @return
		*/
		Datum* FindContainedScope(const Scope* child, size_t& idx);
		const Datum* FindContainedScope(const Scope* child, size_t& idx) const;

		/**
		 * @brief Append will search this Scope's map for the presence of the passed key, constructing a new key value pair if the key is not found.
		 * Takes advantage of the std::unordered_map's []operator functionality, which will default construct a new object of the map's value type
		 * paired with the passed key the map does not already contain that key.
		 * @param key
		 * @return
		*/
		Datum& Append(const std::string& key);
		/**
		 * @brief AppendScope will default construct a new scope childed to a datum associated with the passed key.
		 * Calls Append to ensure we have a Datum paired with that key.
		 * @param key
		 * @return
		*/
		Scope* AppendScope(const std::string& key);
		
		/* Simple Append wrapper */
		Datum& operator[](const std::string& key);
		/**
		 * @brief Uses the std::vec []operator on this Scope's datum vec to return a reference to the Datum contained at the passed index.
		 * @param idx
		 * @return
		*/
		Datum& operator[](size_t idx);
		const Datum& operator[](const size_t idx) const;
		
		/**
		 * @brief Thin wrapper for the Search out param variant, using a dummy Scope pointer. 
		 * Search will recursively search this Scope's data, and the data of this Scope's ancestors, for a datum paired with the passed string key.
		 * Returns a ptr to the Datum if found, or a nullptr if not found.
		 * @param key
		 * String key paired with the returned Datum
		 * @return
		*/
		Datum* Search(const std::string& key);

		/**
		 * @brief 
		 * Search will recursively search this Scope's data, and the data of this Scope's ancestors, for a datum paired with the passed string key.
		 * This Search variant uses an out parameter that will contain a reference to the Scope that contains the returned Datum.
		 * @param key
		 * String key paired with the returned Datum
		 * @param scope
		 * @return
		*/
		Datum* Search(const std::string& key, Scope*& scope);

		const Datum* Search(const std::string& key) const;
		const Datum* Search(const std::string& key, const Scope*& scope) const;

		/**
		 * @brief Adopt will first call Orphan() on the passed child ref, then reparent that child to this scope by first calling append and
		 * then pushing the child into the datum of the passed key.
		 * @param key The key at which the child's datum container can be found
		 * @param child The child to adopt
		*/
		void Adopt(const std::string& key, Scope* child);

		Scope* GetParent() const;

		///**
		// * @brief Recursively checks whether this scope or one of its ancestors is a child of the passed Scope reference
		// * @param parent Ptr to the potential ancestor Scope
		// * @return
		//*/
		//bool IsDescendantOf(Scope* parent);

		/**
		 * @brief Recursively checks whether this scope or one of its ancestors is a child of the passed Scope reference
		 * @param parent Ptr to the potential ancestor Scope
		 * @return
		*/
		bool IsDescendantOf(const Scope* parent) const;

		///**
		// * @brief 
		// * Simply returns  !IsDescendantOf() 
		// * @param descendant 
		// * @return 
		//*/
		//bool IsAncestorOf(Scope* descendant);

		/**
		 * @brief
		 * Simply returns  !IsDescendantOf()
		 * @param descendant
		 * @return
		*/
		bool IsAncestorOf(const Scope* descendant) const;

		/**
		 * @brief Returns the number of Datums held by this scope;
		 * @return 
		*/
		size_t Size() const { return _datumVec.size(); }

	protected:
			/**
		 * @brief Calls EraseScope on the datum that 'contains' the passed child, then nulls the child's parentScope.
		 * This method is intended to only ever be called through Adopt() or the destructor.
		 * @param child
		*/
			[[nodiscard]] Scope* Orphan(Scope* child);

	private:
		Scope* _parentScope = nullptr;
		std::vector<std::pair<std::string, Datum*>> _datumVec;
		std::unordered_map<std::string, Datum> _datumMap;
				
		/**
		 * @brief 
		 * Searches through every Scope contained by this Scope and its children, destructing each contained scope and removing them from the Datums that held them.
		*/
		void Clear();

		
	};
}