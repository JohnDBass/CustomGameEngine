
#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include "RTTI.h"
#include <stdexcept>

namespace Fiea::GameEngine
{
	Datum::Datum()
	{
		_mType = DatumType::Unknown;
		_mData.vPtr = nullptr;
		size_t _mSize = 0;
		size_t _mCapacity = 0;
		bool _bExternalDatum = false;
	}

	/**
	 * @brief Copy construct this Datum using another lvalue Datum.
	 If the other Datum has a type (!Type:Unknown), copy construct appropriate value types with placement new.
	 * @param other An lvavlue Datum
	*/
	Datum::Datum(const Datum& other)
	{
		_mType = other._mType;
		_mSize = other._mSize;
		_mCapacity = other._mCapacity;

		if (other._bExternalDatum)
		{
			_mData.vPtr = other._mData.vPtr;
			_bExternalDatum = true;
			return;
		}		

		_bExternalDatum = false;

		switch (_mType)
		{
		case DatumType::Unknown :
			_mData.vPtr = nullptr;
			_bExternalDatum = false;
			break;
		case DatumType::Int :
			_mData.i = (int*)malloc(sizeof(int) * _mCapacity);
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.i + i) int(*(other._mData.i + i));
			}
			break;
		case DatumType::Float :
			_mData.f = (float*)malloc(sizeof(float) * _mCapacity);
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.f + i) float(*(other._mData.f + i));
			}
			break;
		case DatumType::String :
			_mData.str = (std::string*)malloc(sizeof(std::string) * _mCapacity);
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.str + i) std::string(*(other._mData.str + i));
			}
			break;
		case DatumType::Vector :
			_mData.gvec = (glm::vec4*)malloc(sizeof(glm::vec4) * _mCapacity);
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.gvec + i) glm::vec4(*(other._mData.gvec + i));
			}
			break;
		case DatumType::Matrix :
			_mData.gmat = (glm::mat4x4*)malloc(sizeof(glm::mat4x4) * _mCapacity);
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.gmat + i) glm::mat4x4(*(other._mData.gmat + i));
			}
			break;
		case DatumType::Table:
			_mData.scp = (Scope**)malloc(sizeof(Scope*) * _mCapacity);
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.scp + i) Scope * (*(other._mData.scp + i));
			}
			break;
		case DatumType::Pointer:
			_mData.rtti = (RTTI**)malloc(sizeof(RTTI*) * _mCapacity);
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.rtti + i) RTTI* (*(other._mData.rtti + i));
			}
			break;
		default: break;
		}
	}
	/**
		 * @brief Copy construct using an rvalue Datum, this datum simply assumes the data ptr of the other datum
		 * @param rhsOther An rvalue Datum
		*/
	Datum::Datum(Datum&& rhsOther) noexcept
	{
		_mType = rhsOther._mType;
		_mSize = rhsOther._mSize;
		_mCapacity = rhsOther._mCapacity;

		if (rhsOther._bExternalDatum)
		{
			_mData.vPtr = rhsOther._mData.vPtr;
			_bExternalDatum = true;
			rhsOther._mData.vPtr = nullptr;
			rhsOther._mType = DatumType::Unknown;
			return;
		}

		_bExternalDatum = false;
		_mData.vPtr = rhsOther._mData.vPtr;
		rhsOther._mData.vPtr = nullptr;
		rhsOther._mType = DatumType::Unknown;
	}

	/**
	 * @brief If this datum is not an Unknown or primitive Datum, call a destructor on each data element pointed to my _mdata, then free _mdata.
	*/
	Datum::~Datum()
	{
		if (_bExternalDatum == true)
		{
			return;
		}
		if (_mType == String)
		{
			for (size_t i = 0; i < _mSize; ++i)
			{
				std::string* str = _mData.str + i;
				str->~basic_string();
			}
		}
		free(_mData.vPtr);
	}

	/**
	 * @brief Throws exceptions if either Datum is an external datum, or if these Datums are not of compatible types
	 * Otherwise follows similar logic to the lvalue copy constructor, with the addition of a call to Clear() if this Datum contains data,
	 * and Reserve() if this datum does not have enough capacity
	 * @param other rhside Datum (lvalue)
	 * @return *this Datum
	*/
	Datum& Datum::operator=(const Datum& other)
	{
		if (other == *this)
			return *this;

		Clear();

		if (other._bExternalDatum)
		{
			_mType = other._mType;
			_mSize = other._mSize;
			_mCapacity = other._mCapacity;

			_mData.vPtr = other._mData.vPtr;
			_bExternalDatum = true;
			return *this;
		}

		_mType = other._mType;
		if (_mCapacity < other._mSize)
		{
			Reserve(other._mSize);
		}
		_mSize = other._mSize;

		switch (_mType)
		{
		case DatumType::Int:
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.i + i) int(*(other._mData.i + i));
			}
			break;
		case DatumType::Float:
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.f + i) float(*(other._mData.f + i));
			}
			break;
		case DatumType::String:
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.str + i) std::string(*(other._mData.str + i));
			}
			break;
		case DatumType::Vector:
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.gvec + i) glm::vec4(*(other._mData.gvec + i));
			}
			break;
		case DatumType::Matrix:
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.gmat + i) glm::mat4x4(*(other._mData.gmat + i));
			}
			break;
		case DatumType::Table:
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.scp + i) Scope*(*(other._mData.scp + i));
			}
			break;
		case DatumType::Pointer:
			for (size_t i = 0; i < _mSize; ++i)
			{
				new(_mData.rtti + i) RTTI* (*(other._mData.rtti + i));
			}
			break;
		default: break;
		}
		return *this;
	}

	/**
	 * @brief Similar to the lvalue assignment operator above, but simply points this._mData to other's _mData
	 * @param rhsOther rhside Datum (rvalue)
	 * @return *this Datum
	*/
	Datum& Datum::operator=(Datum&& rhsOther) noexcept
	{

		_mType = rhsOther._mType;
		Clear();

		if (_mCapacity < rhsOther._mSize)
		{
			Reserve(rhsOther._mSize);
		}

		_mSize = rhsOther._mSize;
		free(_mData.vPtr);
		_mData.vPtr = rhsOther._mData.vPtr;
		rhsOther._mData.vPtr = nullptr;
		rhsOther._mType = DatumType::Unknown;
		return *this;
	}


	Datum::Datum(int val)
	{
		_mType = DatumType::Int;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.i = (int*)malloc(sizeof(int));
		new(_mData.i) int(val);
	}
	Datum::Datum(float val)
	{
		_mType = DatumType::Float;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.f = (float*)malloc(sizeof(float));
		new(_mData.f) float(val);
	}
	Datum::Datum(const std::string& val)
	{
		_mType = DatumType::String;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.str = (std::string*)malloc(sizeof(std::string));
		new(_mData.str) std::string(val);
	}
	Datum::Datum(const glm::vec4& val)
	{
		_mType = DatumType::Vector;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.gvec = (glm::vec4*)malloc(sizeof(glm::vec4));
		new(_mData.gvec) glm::vec4(val);
	}
	Datum::Datum(const glm::mat4x4& val)
	{
		_mType = DatumType::Matrix;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.gmat = (glm::mat4x4*)malloc(sizeof(glm::mat4x4));
		new(_mData.gmat) glm::mat4x4(val);
	}


	Datum::Datum(std::string&& rhsVal)
	{
		_mType = DatumType::String;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.str = (std::string*)malloc(sizeof(std::string));
		new(_mData.str) std::string(std::move(rhsVal));
	}
	Datum::Datum(glm::vec4&& rhsVal)
	{
		_mType = DatumType::Vector;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.gvec = (glm::vec4*)malloc(sizeof(glm::vec4));
		new(_mData.gvec) glm::vec4(std::move(rhsVal));
	}
	Datum::Datum(glm::mat4x4&& rhsVal)
	{
		_mType = DatumType::Matrix;
		_bExternalDatum = false;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
		_mData.gmat = (glm::mat4x4*)malloc(sizeof(glm::mat4x4));
		new(_mData.gmat) glm::mat4x4(std::move(rhsVal));
	}

	Datum& Datum::operator=(int val)
	{
		if (_mType != DatumType::Unknown && _mType != DatumType::Int)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}
		if (_bExternalDatum)
		{
			*(_mData.i) = val;
			return *this;
		}

		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::Int;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.i = (int*)malloc(sizeof(int));
			new(_mData.i) int(val);
		}
		else
		{
			Clear();
			new(_mData.i) int(val);
		}
		_mSize = (size_t)1;
		return *this;
	}
	Datum& Datum::operator=(float val)
	{
		if (_mType != DatumType::Unknown && _mType != DatumType::Float)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}
		if (_bExternalDatum)
		{
			*(_mData.f) = val;
			return *this;
		}
		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::Float;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.f = (float*)malloc(sizeof(float));
			new(_mData.f) float(val);
		}
		else
		{
			Clear();
			new(_mData.f) float(val);
		}
		_mSize = (size_t)1;
		return *this;
	}
	Datum& Datum::operator=(const std::string& val)
	{
		if (_mType != DatumType::Unknown && _mType != DatumType::String)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}
		if (_bExternalDatum)
		{
			*(_mData.str) = val;
			return *this;
		}

		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::String;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.str = (std::string*)malloc(sizeof(std::string));
			new(_mData.str) std::string(val);
		}
		else
		{
			Clear();
			new(_mData.str) std::string(val);
		}
		_mSize = (size_t)1;
		return *this;
	}
	Datum& Datum::operator=(const glm::vec4& val)
	{
		if (_mType != DatumType::Unknown && _mType != DatumType::Vector)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}
		if (_bExternalDatum)
		{
			*(_mData.gvec) = val;
			return *this;
		}

		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::Vector;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.gvec = (glm::vec4*)malloc(sizeof(glm::vec4));
			new(_mData.gvec) glm::vec4(val);
		}
		else
		{
			Clear();
			new(_mData.gvec) glm::vec4(val);
		}
		_mSize = (size_t)1;
		return *this;
	}
	Datum& Datum::operator=(const glm::mat4x4& val)
	{
		if (_mType != DatumType::Unknown && _mType != DatumType::Matrix)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}
		if (_bExternalDatum)
		{
			*(_mData.gmat) = val;
			return *this;
		}
		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::Matrix;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.gmat = (glm::mat4x4*)malloc(sizeof(glm::mat4x4));
			new(_mData.gmat) glm::mat4x4(val);
		}
		else
		{
			Clear();
			new(_mData.gmat) glm::mat4x4(val);
		}
		_mSize = (size_t)1;
		return *this;
	}


	Datum& Datum::operator=(std::string&& rhsVal)
	{

		if (_mType != DatumType::Unknown && _mType != DatumType::String)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}
		if (_bExternalDatum)
		{
			*(_mData.str) = std::move(rhsVal);
			return *this;
		}
		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::String;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.str = (std::string*)malloc(sizeof(std::string));
			new(_mData.str) std::string(std::move(rhsVal));
		}
		else
		{
			Clear();
			new(_mData.str) std::string(std::move(rhsVal));
		}
		_mSize = (size_t)1;
		return *this;
	}
	Datum& Datum::operator=(glm::vec4&& rhsVal)
	{

		if (_mType != DatumType::Unknown && _mType != DatumType::Vector)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}

		if (_bExternalDatum)
		{
			*(_mData.gvec) = std::move(rhsVal);
			return *this;
		}

		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::Vector;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.gvec = (glm::vec4*)malloc(sizeof(glm::vec4));
			new(_mData.gvec) glm::vec4(std::move(rhsVal));
		}
		else
		{
			Clear();
			new(_mData.gvec) glm::vec4(std::move(rhsVal));
		}
		_mSize = (size_t)1;
		return *this;
	}
	Datum& Datum::operator=(glm::mat4x4&& rhsVal)
	{

		if (_mType != DatumType::Unknown && _mType != DatumType::Matrix)
		{
			throw std::exception("datum must be of the correct type or unassigned");
		}

		if (_bExternalDatum)
		{
			*(_mData.gmat) = std::move(rhsVal);
			return *this;
		}

		if (_mType == DatumType::Unknown)
		{
			_mType = DatumType::Matrix;
			_bExternalDatum = false;
			_mCapacity = (size_t)1;
			_mData.gmat = (glm::mat4x4*)malloc(sizeof(glm::mat4x4));
			new(_mData.gmat) glm::mat4x4(std::move(rhsVal));
		}
		else
		{
			Clear();
			new(_mData.gmat) glm::mat4x4(std::move(rhsVal));
		}
		_mSize = (size_t)1;
		return *this;
	}


	void Datum::PushBack(int val)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Int;

		else if (_mType != DatumType::Int)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.i + _mSize) int(val);
		_mSize++;
	}

	void Datum::PushBack(float val)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Float;

		else if (_mType != DatumType::Float)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.f + _mSize) float(val);
		_mSize++;
	}

	void Datum::PushBack(const std::string& val)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::String;

		else if (_mType != DatumType::String)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.str + _mSize) std::string(val);
		_mSize++;
	}

	void Datum::PushBack(const glm::vec4& val)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Vector;

		else if (_mType != DatumType::Vector)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.gvec + _mSize) glm::vec4(val);
		_mSize++;
	}

	void Datum::PushBack(const glm::mat4x4& val)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Matrix;

		else if (_mType != DatumType::Matrix)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.gmat + _mSize) glm::mat4x4(val);
		_mSize++;
	}

	void Datum::PushBack(Scope* val)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Table;

		else if (_mType != DatumType::Table)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}
		new(_mData.scp + _mSize) Scope*(val);
		_mSize++;
	}

	void Datum::PushBack(RTTI* val)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Pointer;

		else if (_mType != DatumType::Pointer)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.rtti + _mSize) RTTI*(val);
		_mSize++;
	}

	void Datum::PushBack(std::string&& rhsVal)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::String;

		else if (_mType != DatumType::String)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.str + _mSize) std::string(std::move(rhsVal));
		_mSize++;
	}

	void Datum::PushBack(glm::vec4&& rhsVal)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Vector;

		else if (_mType != DatumType::Vector)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}

		new(_mData.gvec + _mSize) glm::vec4(std::move(rhsVal));
		_mSize++;
	}

	void Datum::PushBack(glm::mat4x4&& rhsVal)
	{
		if (_bExternalDatum == true)
			throw std::exception("Attempted to call PushBack on an external Datum");

		if (_mType == DatumType::Unknown)
			_mType = DatumType::Matrix;

		else if (_mType != DatumType::Matrix)
			throw std::exception("Type mismatch between Datum and passed PushBack val");

		if (_mSize == _mCapacity)
		{
			size_t newCap = geo_cap_growth(_mCapacity + 1);
			Reserve(newCap);
		}
		new(_mData.gmat + _mSize) glm::mat4x4(std::move(rhsVal));
		_mSize++;
	}


	void Datum::PopBack()
	{
		if (_bExternalDatum == true)
		{
			throw std::exception("popback on an external datum is not supported");
		}

		if (_mSize == 0)
		{
			throw std::exception("Popback called on an empty Datum");
		}
		if (_mType == String)
		{
			std::string* str = _mData.str + _mSize - 1;
			str->~basic_string();
		}

		--_mSize;
	}

	bool Datum::operator==(const Datum& rhs) const
	{
		if (rhs._mSize != _mSize || rhs._mType != _mType || rhs._bExternalDatum != _bExternalDatum)
			return false;

		switch (_mType)
		{
		case DatumType::Unknown:
			return true;
			break;
		case DatumType::Int:
			for (size_t i = 0; i < _mSize; ++i)
				if (*(_mData.i + i) != *(rhs._mData.i + i))
					return false;
			break;
		case DatumType::Float:
			for (size_t i = 0; i < _mSize; ++i)
				if (*(_mData.f + i) != *(rhs._mData.f + i))
					return false;
			break;
		case DatumType::String:
			for (size_t i = 0; i < _mSize; ++i)
				if (*(_mData.str + i) != *(rhs._mData.str + i))
					return false;
			break;
		case DatumType::Vector:
			for (size_t i = 0; i < _mSize; ++i)
				if (*(_mData.gvec + i) != *(rhs._mData.gvec + i))
					return false;
			break;
		case DatumType::Matrix:
			for (size_t i = 0; i < _mSize; ++i)
				if (*(_mData.gmat + i) != *(rhs._mData.gmat + i))
					return false;
			break;

		default: break;
		}
		return true;
	}


	bool Datum::operator!=(const Datum& rhs) const
	{
		return !operator==(rhs);
	}

	DatumType Datum::Type() const
	{
		return _mType;
	}

	size_t Datum::Size() const
	{
		return _mSize;
	}

	size_t Datum::Capacity() const
	{
		return _mCapacity;
	}

	bool Datum::IsExternal() const
	{
		return _bExternalDatum;
	}

	void Datum::Reserve(size_t capacity)
	{
		if (_bExternalDatum == true)
			throw std::exception("reserve on an external datum is not supported");

		if (_mCapacity >= capacity || _mType == DatumType::Unknown)
		{
			return;
		}

		switch (_mType)
		{
		case DatumType::Int:
		{
			void* newLoc = malloc(sizeof(_mData.vPtr) * capacity);
			assert(newLoc != nullptr);
			memcpy(newLoc, _mData.vPtr, sizeof(_mData.vPtr) * _mSize);
			free(_mData.vPtr);
			_mData.vPtr = newLoc;
			break;
		}
		case DatumType::Float:
		{
			void* newLoc = malloc(sizeof(_mData.vPtr) * capacity);
			assert(newLoc != nullptr);
			memcpy(newLoc, _mData.vPtr, sizeof(_mData.vPtr) * _mSize);
			free(_mData.vPtr);
			_mData.vPtr = newLoc;
			break;
		}
		case DatumType::String:
		{
			void* newLoc = malloc(sizeof(std::string) * capacity);
			assert(newLoc != nullptr);
			memcpy(newLoc, _mData.vPtr, sizeof(std::string) * _mSize);
			free(_mData.vPtr);
			_mData.vPtr = newLoc;

			break;
		}
		case DatumType::Vector:
		{
			glm::vec4* newLoc = (glm::vec4*)malloc(sizeof(glm::vec4) * capacity);
			assert(newLoc != nullptr);
			memcpy(newLoc, _mData.gvec, sizeof(glm::vec4) * _mSize);
			free(_mData.gvec);
			_mData.gvec = newLoc;

			break;
		}
		case DatumType::Matrix:
		{
			glm::mat4x4* newLoc = (glm::mat4x4*)malloc(sizeof(glm::mat4x4) * capacity);
			assert(newLoc != nullptr);
			memcpy(newLoc, _mData.gmat, sizeof(glm::mat4x4) * _mSize);
			free(_mData.gmat);
			_mData.gmat = newLoc;

			break;
		}
		case DatumType::Table:
		{
			void* newLoc = malloc(sizeof(_mData.vPtr) * capacity);
			assert(newLoc != nullptr);
			memcpy(newLoc, _mData.vPtr, sizeof(_mData.vPtr) * _mSize);
			free(_mData.vPtr);
			_mData.vPtr = newLoc;
			break;
		}
		case DatumType::Pointer:
		{
			void* newLoc = malloc(sizeof(_mData.vPtr) * capacity);
			assert(newLoc != nullptr);
			memcpy(newLoc, _mData.vPtr, sizeof(_mData.vPtr) * _mSize);
			free(_mData.vPtr);
			_mData.vPtr = newLoc;
			break;
		}
		default: break;
		}
		_mCapacity = capacity;
	}

	void Datum::Clear()
	{
		if (_bExternalDatum == true)
			return;

		if (_mSize == 0)
			return;

		if (_mType == String)
		{
			for (size_t i = 0; i < _mSize; ++i)
			{
				std::string* str = _mData.str + i;
				str->~basic_string();
			}
		}
		_mSize = 0;		
	}

	void Datum::Resize(size_t size)
	{
		if (_bExternalDatum == true)
			throw std::exception("resize on an external datum is not supported");
		if (_mSize == size || DatumType::Unknown)
			return;

		if (_mSize > size)
		{
			if (_mType == String)
			{
				for (size_t i = size; i < _mSize; ++i)
				{
					std::string* str = _mData.str + i;
					str->~basic_string();
				}
			}			
			_mSize = size;
		}

		if (_mCapacity < size)
			Reserve(size);

		if (_mSize < size)
		{
			switch (_mType)
			{
			case DatumType::Int:
				for (size_t i = _mSize; i < size; ++i)
					new(_mData.i + i) int();
				_mSize = size;
				break;
			case DatumType::Float:
				for (size_t i = _mSize; i < size; ++i)
					new(_mData.f + i) float();
				_mSize = size;
				break;
			case DatumType::String:
				for (size_t i = _mSize; i < size; ++i)
					new(_mData.str + i) std::string();
				_mSize = size;
				break;
			case DatumType::Vector:
				for (size_t i = _mSize; i < size; ++i)
					new(_mData.gvec + i) glm::vec4();
				_mSize = size;
				break;
			case DatumType::Matrix:
				for (size_t i = _mSize; i < size; ++i)
					new(_mData.gmat + i) glm::mat4x4();
				_mSize = size;
				break;
			default: break;
			}
		}
	}
	

	int Datum::GetInt(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Int)
			throw std::exception("Incorrect DatumType");

		return *(_mData.i + idx);
	}
	float Datum::GetFloat(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Float)
			throw std::exception("Incorrect DatumType");

		return *(_mData.f + idx);
	}
	const std::string& Datum::GetString(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::String)
			throw std::exception("Incorrect DatumType");

		return *(_mData.str + idx);
	}
	const glm::vec4& Datum::GetVector(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Vector)
			throw std::exception("Incorrect DatumType");

		return *(_mData.gvec + idx);
	}
	const glm::mat4x4& Datum::GetMatrix(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Matrix)
			throw std::exception("Incorrect DatumType");

		return *(_mData.gmat + idx);
	}

	Scope* Datum::GetTable(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");

		if (_mType != DatumType::Table)
			throw std::exception("Incorrect DatumType");

		return (*(_mData.scp + idx));
	}

	RTTI* Datum::GetRTTI(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");

		if (_mType != DatumType::Pointer)
			throw std::exception("Incorrect DatumType");

		return (*(_mData.rtti + idx));
	}

	void Datum::SetType(DatumType type)
	{
		switch (type)
		{
		case DatumType::Int:
			_mData.i = (int*)malloc(sizeof(int));
			new(_mData.i) int();
			_bExternalDatum = false;
			break;
		case DatumType::Float:
			_mData.f = (float*)malloc(sizeof(float));
			new(_mData.f) float();
			_bExternalDatum = false;
			break;
		case DatumType::String:
			_mData.str = (std::string*)malloc(sizeof(std::string));
			new(_mData.str) std::string();
			_bExternalDatum = false;
			break;
		case DatumType::Vector:
			_mData.gvec = (glm::vec4*)malloc(sizeof(glm::vec4));
			new(_mData.gvec) glm::vec4();
			_bExternalDatum = false;
			break;
		case DatumType::Matrix:
			_mData.gmat = (glm::mat4x4*)malloc(sizeof(glm::mat4x4));
			new(_mData.gmat) glm::mat4x4();
			_bExternalDatum = false;
			break;
		case DatumType::Pointer:
			_bExternalDatum = false;
			break;
		case DatumType::Table:
			//_mData.scp = new Scope{};
			_bExternalDatum = false;
			break;
		case DatumType::Unknown:
			throw std::exception("Unsupported DatumType passed to SetType()");
			break;
		}
		_mType = type;
		_mSize = (size_t)1;
		_mCapacity = (size_t)1;
	}

	void Datum::SetInt(int val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Int;

		else if (_mType != DatumType::Int)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.i[idx] = val;
	}

	void Datum::SetFloat(float val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Float;

		else if (_mType != DatumType::Float)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.f[idx] = val;
	}

	void Datum::SetString(const std::string& val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::String;

		else if (_mType != DatumType::String)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.str[idx] = val;
	}

	void Datum::SetVector(const glm::vec4& val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Vector;

		else if (_mType != DatumType::Vector)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.gvec[idx] = val;
	}

	void Datum::SetMatrix(const glm::mat4x4& val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Matrix;

		else if (_mType != DatumType::Matrix)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.gmat[idx] = val;
	}



	void Datum::SetString(std::string&& val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::String;

		else if (_mType != DatumType::String)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.str[idx] = std::move(val);
	}

	void Datum::SetVector(glm::vec4&& val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Vector;

		else if (_mType != DatumType::Vector)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.gvec[idx] = std::move(val);
	}

	void Datum::SetMatrix(glm::mat4x4&& val, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Matrix;

		else if (_mType != DatumType::Matrix)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);

		_mData.gmat[idx] = std::move(val);
	}

	void Datum::AddInt(int intToAdd, size_t idx)
	{
		assert(_mType == DatumType::Int);
		assert(idx <= _mSize && idx >= 0);

		_mData.i[idx] += intToAdd;
	}


	void Datum::AddFloat(float floatToAdd, size_t idx)
	{
		assert(_mType == DatumType::Float);
		assert(idx <= _mSize && idx >= 0);

		_mData.f[idx] += floatToAdd;
	}

	std::string Datum::GetIntAsString(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Int)
			throw std::exception("Incorrect DatumType");

		return std::to_string(*(_mData.i + idx));
	}

	std::string Datum::GetFloatAsString(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Float)
			throw std::exception("Incorrect DatumType");

		return std::to_string(*(_mData.f + idx));
	}

	std::string Datum::GetVectorAsString(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Vector)
			throw std::exception("Incorrect DatumType");

		return glm::to_string(*(_mData.gvec + idx));
	}

	std::string Datum::GetMatrixAsString(size_t idx) const
	{
		if (idx >= _mSize)
			throw std::out_of_range("index out of range");
		if (_mType != DatumType::Matrix)
			throw std::exception("Incorrect DatumType");

		return glm::to_string(*(_mData.gmat + idx));
	}

	void Datum::SetIntAsString(const std::string& str, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Int;

		else if (_mType != DatumType::Int)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);		

		int input;
		int success = sscanf_s(str.c_str(), "%i", &input);
		if (success != 1)
			throw std::invalid_argument("invalid string input in SetVectorAsInt");

		_mData.i[idx] = input;
	}

	void Datum::SetFloatAsString(const std::string& str, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Float;

		else if (_mType != DatumType::Float)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);		

		float input;
		int success = sscanf_s(str.c_str(), "%f", &input);
		if (success != 1)
			throw std::invalid_argument("invalid string input in SetVectorAsInt");

		_mData.f[idx] = input;
	}

	void Datum::SetVectorAsString(const std::string& str, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Vector;

		else if (_mType != DatumType::Vector)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);
		
		float f0, f1, f2, f3;
		int success = sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &f0, &f1, &f2, &f3);
		if (success != 4) 
			throw std::invalid_argument("invalid string input in SetVectorAsString"); 
		
		_mData.gvec[idx] = glm::vec4(f0, f1, f2, f3);
	}

	void Datum::SetMatrixAsString(const std::string& str, size_t idx)
	{
		if (_mType == DatumType::Unknown)
			_mType = DatumType::Matrix;

		else if (_mType != DatumType::Matrix)
			throw std::exception("Incorrect DatumType");

		if (idx >= _mSize)
			Resize(idx + 1);		

		float f0, f1, f2, f3, f10, f11, f12, f13, f20, f21, f22, f23, f30, f31, f32, f33;
		int success = sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", 
			&f0, &f1, &f2, &f3, &f10, &f11, &f12, &f13, &f20, &f21, &f22, &f23, &f30, &f31, &f32, &f33);
		if (success != 16)
			throw std::invalid_argument("invalid string input in SetVectorAsString");

		_mData.gmat[idx] = glm::mat4x4(f0, f1, f2, f3, f10, f11, f12, f13, f20, f21, f22, f23, f30, f31, f32, f33);
	}

	void Datum::SetStorage(void* ptr, DatumType type, size_t size)
	{
		if (_mType != DatumType::Unknown && _bExternalDatum == false)
			throw std::exception("cannot call SetStorage on a Datum that contains internal storage");

		_mData.vPtr = ptr;
		_mType = type;
		_bExternalDatum = true;
		_mSize = size;
		_mCapacity = size;
	}

	void Datum::EraseScope(size_t idx)
	{
		if (_mType != DatumType::Table)
			throw std::exception("EraseScope can only be called on a table Datum");

		if (idx >= _mSize)
			throw std::out_of_range("EraseScope called with idx outside of Datum range");

		if (idx < (_mSize - (size_t)1))
		{
			for (size_t i = idx; i < _mSize - 1; ++i)
			{
				_mData.scp[i] = _mData.scp[i + 1];
			}
		}
		--_mSize;
	}


	size_t Datum::geo_cap_growth(size_t minCap) const
	{
		size_t geoGrowth = _mCapacity + _mCapacity / 2;
		return (geoGrowth > minCap) ? geoGrowth : minCap;
	}
}
