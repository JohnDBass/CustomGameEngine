#include "pch.h"
#include "Attributed.h"
#include "TypeRegistry.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(size_t type)
	{
		_mTypeID = type;
		AppendAttributes(_mTypeID);
	}
	
	Attributed::Attributed(const Attributed& other) : Scope(other)
	{
		_mTypeID = other._mTypeID;
		AppendAttributes(_mTypeID);
	}

	Attributed::Attributed(Attributed&& other) noexcept : Scope(std::move(other))
	{
		_mTypeID = other._mTypeID;
		AppendAttributes(_mTypeID);
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		assert(_mTypeID == other._mTypeID);
		Scope::operator=(static_cast<Scope>(other));

		AppendAttributes(_mTypeID);

		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		assert(_mTypeID == other._mTypeID);
		Scope::operator=(static_cast<Scope>(std::move(other)));
		AppendAttributes(_mTypeID);

		return *this;
	}
	

	bool Attributed::IsAttribute(const std::string& str) const
	{
		return nullptr != Find(str);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& str) const
	{
		const std::vector<Signature> v = TypeRegistry::GetSignatures(_mTypeID);
		for (Signature s : v)
		{
			if (s.Name() == str)
				return true;
		}
		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& str) const
	{
		return IsAttribute(str) && !IsPrescribedAttribute(str);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& str)
	{
		if (IsPrescribedAttribute(str))
			throw std::exception("Called Append on a prescribed attribtue of this class");

		return Append(str);
	}

	void Attributed::AppendAttributes(size_t typeID)
	{
		Append(std::string("this")).SetStorage(this, DatumType::Pointer, 1);

		const std::vector<Signature>& sigVec = TypeRegistry::GetSignatures(typeID);
		for (Signature s : sigVec)
		{
			if (s.Offset() == 0)
				Append(s.Name());

			else
				Append(s.Name()).SetStorage((reinterpret_cast<char*>(this)) + s.Offset(), s.Type(), s.Size());
		}
	}
}
