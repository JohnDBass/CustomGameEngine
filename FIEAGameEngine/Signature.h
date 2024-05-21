#pragma once
#include <string>
#include "Datum.h"

namespace Fiea::GameEngine
{
	/**
	 * @brief 
	 * I've defined my Signatures as objects rather than structs as it seemed like the best way to keep Signature's contents unmutable
	 * Unless there's a future use-case I'm not aware of, it seems like the definition at least of prescribed attributes should never change
	*/
	class Signature final
	{

	public:
		Signature() = default;
		~Signature() = default;
		Signature(const Signature& rhs) : _name(rhs._name), _type(rhs._type), _size(rhs._size), _offset(rhs._offset) {};
		Signature(std::string n, DatumType t, size_t s, size_t o) : _name(n), _type(t), _size(s), _offset(o) {};

		const std::string& Name() const { return _name; };
		const DatumType& Type() const { return _type; };
		const size_t& Size() const { return _size; };
		const size_t& Offset() const { return _offset; };


	private:
		std::string _name;
		DatumType _type = DatumType::Unknown;
		size_t _size = 0;
		size_t _offset = 0;

	};
}