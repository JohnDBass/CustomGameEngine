#pragma once
#include <unordered_map>
#include "Signature.h"
#include <memory>

namespace Fiea::GameEngine
{
	class TypeRegistry
	{

	public:
		/**
		 * @brief 
		 * Returns a ref to the signature vector at key typeID.
		 * Throws a std::out_of_range exception if the key is not found in the map
		 * @param typeID - RTTI Id of Attributed class
		 * map key
		 * @return 
		*/
		static const std::vector<Signature>& GetSignatures(size_t typeID);

		/**
		 * @brief 
		 * Inserts passed signature vector into map at key typeID, if that key does not already exist
		 * @param typeID 
		 * map key - RTTI Id of Attributed class
		 * @param signatures 
		 * map value - prescribed attributes of attributed class
		*/
		static void RegisterSignatures(size_t typeID, const std::vector<Signature>& signatures);

		/**
		 * @brief 
		 * Appends the parent signature vector to the front of the passed child signature vector, then adds the new combined signature vector to _signatureList.
		 * @param childID 
		 * @param parentID 
		 * @param childSignatures 
		*/
		static void RegisterChildSignatures(size_t childID, size_t parentID, const std::vector<Signature>& childSignatures);


	private:
		inline static std::unique_ptr<std::unordered_map<size_t, std::vector<Signature>>> _signatureList;

	};
}