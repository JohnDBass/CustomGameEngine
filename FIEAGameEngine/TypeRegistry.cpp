#include "pch.h"
#include "TypeRegistry.h"

namespace Fiea::GameEngine
{
	const std::vector<Signature>& TypeRegistry::GetSignatures(size_t typeID)
	{
		if (_signatureList == nullptr)
			_signatureList = std::make_unique<std::unordered_map<size_t, std::vector<Signature>>>();

		return _signatureList->at(typeID);
	}


	void TypeRegistry::RegisterSignatures(size_t typeID, const std::vector<Signature>& signatures)
	{
		if (_signatureList == nullptr)
			_signatureList = std::make_unique<std::unordered_map<size_t, std::vector<Signature>>>();

		auto foundSignature = _signatureList->find(typeID);
		if (foundSignature == _signatureList->end())
		{
			_signatureList->insert({ typeID, signatures });
		}

	}

	void TypeRegistry::RegisterChildSignatures(size_t childID, size_t parentID, const std::vector<Signature>& childSignatures)
	{
		if (childID == parentID)
			return;

		if (_signatureList == nullptr)
			_signatureList = std::make_unique<std::unordered_map<size_t, std::vector<Signature>>>();

		if (_signatureList->find(parentID) == _signatureList->end())
		{
			throw std::exception("Trying to register child signatures with an invalid parentID.");
		}

		std::vector<Signature> sig = _signatureList->find(parentID)->second;
		sig.insert(sig.end(), childSignatures.begin(), childSignatures.end());
		RegisterSignatures(childID, sig);
	}

}