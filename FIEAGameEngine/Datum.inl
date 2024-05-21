#pragma once
#include <cassert>
#include "Datum.h"

namespace Fiea::GameEngine
{

	template<class T>
	inline Datum::Datum(std::initializer_list<T> list)
	{
		throw std::exception("Passed an initializer list of an unsupported Datum type.");
	}
	template<>
	inline Datum::Datum(std::initializer_list<int> list)
	{
		size_t newCap = list.size();
		_bExternalDatum = false;
		_mSize = newCap;
		_mCapacity = newCap;
		size_t idx = 0;

		_mType = DatumType::Int;
		_mData.i = (int*)malloc(sizeof(int) * newCap);
		for (auto item : list)
		{
			new(_mData.i + idx) int(item);
			++idx;
		}
	}
	template<>
	inline Datum::Datum(std::initializer_list<float> list)
	{
		size_t newCap = list.size();
		_bExternalDatum = false;
		_mSize = newCap;
		_mCapacity = newCap;
		size_t idx = 0;

		_mType = DatumType::Float;
		_mData.f = (float*)malloc(sizeof(float) * newCap);
		for (auto item : list)
		{
			new(_mData.f + idx) float(item);
			++idx;
		}
	}
	template<>
	inline Datum::Datum(std::initializer_list<std::string> list)
	{
		size_t newCap = list.size();
		_bExternalDatum = false;
		_mSize = newCap;
		_mCapacity = newCap;
		size_t idx = 0;

		_mType = DatumType::String;
		_mData.str = (std::string*)malloc(sizeof(std::string) * newCap);
		for (auto item : list)
		{
			new(_mData.str + idx) std::string(item);
			++idx;
		}
	}
	template<>
	inline Datum::Datum(std::initializer_list<glm::vec4> list)
	{
		size_t newCap = list.size();
		_bExternalDatum = false;
		_mSize = newCap;
		_mCapacity = newCap;
		size_t idx = 0;

		_mType = DatumType::Vector;
		_mData.gvec = (glm::vec4*)malloc(sizeof(glm::vec4) * newCap);
		for (auto item : list)
		{
			new(_mData.gvec + idx) glm::vec4(item);
			++idx;
		}
	}
	template<>
	inline Datum::Datum(std::initializer_list<glm::mat4x4> list)
	{
		size_t newCap = list.size();
		_bExternalDatum = false;
		_mSize = newCap;
		_mCapacity = newCap;
		size_t idx = 0;

		_mType = DatumType::Matrix;
		_mData.gmat = (glm::mat4x4*)malloc(sizeof(glm::mat4x4) * newCap);
		for (auto item : list)
		{
			new(_mData.gmat + idx) glm::mat4x4(item);
			++idx;
		}
	}
	template<>
	inline Datum::Datum(std::initializer_list<Scope*> list)
	{
		size_t newCap = list.size();
		_bExternalDatum = false;
		_mSize = newCap;
		_mCapacity = newCap;
		size_t idx = 0;

		_mType = DatumType::Table;
		_mData.scp = (Scope**)malloc(sizeof(Scope*) * newCap);
		for (auto item : list)
		{
			new(_mData.scp + idx) Scope* (item);
			++idx;
		}
	}
	template<>
	inline Datum::Datum(std::initializer_list<RTTI*> list)
	{
		size_t newCap = list.size();
		_bExternalDatum = false;
		_mSize = newCap;
		_mCapacity = newCap;
		size_t idx = 0;

		_mType = DatumType::Pointer;
		_mData.rtti = (RTTI**)malloc(sizeof(RTTI*) * newCap);
		for (auto item : list)
		{
			new(_mData.rtti + idx) RTTI* (item);
			++idx;
		}
	}

	template<class T>
	inline void Datum::SetStorage(T* arr, size_t size)
	{
		if (_mType != DatumType::Unknown && _bExternalDatum != false)
			throw std::exception("cannot call SetStorage on a Datum that contains internal storage");

		_mData.vPtr = arr;
		_mSize = size;
		_mCapacity = size;
		_bExternalDatum = true;

		if constexpr (std::is_same_v<T, int>)
		{
			_mType = DatumType::Int;
		}
		else if constexpr (std::is_same_v<T, float>)
		{
			_mType = DatumType::Float;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			_mType = DatumType::String;
		}
		else if constexpr (std::is_same_v<T, glm::vec4>)
		{
			_mType = DatumType::Vector;
		}
		else if constexpr (std::is_same_v<T, glm::mat4x4>)
		{
			_mType = DatumType::Matrix;
		}
		else if constexpr (std::is_same_v<T, Scope*>)
		{
			_mType = DatumType::Table;
		}
		else if constexpr (std::is_same_v<T, RTTI*>)
		{
			_mType = DatumType::Pointer;
		}
	}
}
