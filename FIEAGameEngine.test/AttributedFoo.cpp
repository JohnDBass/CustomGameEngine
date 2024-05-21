
#include "pch.h"
#include "AttributedFoo.h"

using namespace std::string_literals;

namespace Fiea::GameEngine::Test
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() : Attributed(TypeIdClass()) {}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs) : 
		Attributed(rhs), 
		extInt(rhs.extInt), 
		extFloat(rhs.extFloat),
		extString(rhs.extString), 
		extVector(rhs.extVector), 
		extMatrix(rhs.extMatrix)
	{
		delete extPtr;
		extPtr = new Foo(*rhs.extPtr);
		for (int i = 0; i < arrSize; ++i)
		{
			extIntArr[i] = rhs.extIntArr[i];
			extFloatArr[i] = rhs.extFloatArr[i];
			extStringArr[i] = rhs.extStringArr[i];
			extVectorArr[i] = rhs.extVectorArr[i];
			extMatrixArr[i] = rhs.extMatrixArr[i];
		}
	}

	AttributedFoo::AttributedFoo(AttributedFoo&& rhs) noexcept : 
		Attributed(std::move(rhs)), 
		extInt(rhs.extInt), 
		extFloat(rhs.extFloat),
		extString(rhs.extString), 
		extVector(rhs.extVector), 
		extMatrix(rhs.extMatrix)
	{
		delete(extPtr);
		extPtr = std::move(rhs.extPtr);
		rhs.extPtr = nullptr;
		for (int i = 0; i < arrSize; ++i)
		{
			extIntArr[i] = rhs.extIntArr[i];
			extFloatArr[i] = rhs.extFloatArr[i];
			extStringArr[i] = rhs.extStringArr[i];
			extVectorArr[i] = rhs.extVectorArr[i];
			extMatrixArr[i] = rhs.extMatrixArr[i];
		}
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
	{
		if (rhs == *this)
			return *this;

		extInt = rhs.extInt;
		extFloat = rhs.extFloat;
		extString = rhs.extString;
		extVector = rhs.extVector;
		extMatrix = rhs.extMatrix;

		delete extPtr;
		extPtr = new Foo(*rhs.extPtr);
		for (int i = 0; i < arrSize; ++i)
		{
			extIntArr[i] = rhs.extIntArr[i];
			extFloatArr[i] = rhs.extFloatArr[i];
			extStringArr[i] = rhs.extStringArr[i];
			extVectorArr[i] = rhs.extVectorArr[i];
			extMatrixArr[i] = rhs.extMatrixArr[i];
		}

		return *this;
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& rhs) noexcept
	{
		if (rhs == *this)
			return *this;

		extInt = std::move(rhs.extInt);
		extFloat = std::move(rhs.extFloat);
		extString = std::move(rhs.extString);
		extVector = std::move(rhs.extVector);
		extMatrix = std::move(rhs.extMatrix);

		delete(extPtr);
		extPtr = std::move(rhs.extPtr);
		rhs.extPtr = nullptr;
		for (int i = 0; i < arrSize; ++i)
		{
			extIntArr[i] = rhs.extIntArr[i];
			extFloatArr[i] = rhs.extFloatArr[i];
			extStringArr[i] = rhs.extStringArr[i];
			extVectorArr[i] = rhs.extVectorArr[i];
			extMatrixArr[i] = rhs.extMatrixArr[i];
		}

		return *this;
	}
	

	AttributedFoo::~AttributedFoo()
	{
		delete(extPtr);
	}

	AttributedFoo* AttributedFoo::Clone() const {
		return new AttributedFoo(*this);
	}

	std::vector<Signature> AttributedFoo::Signatures() {
		return std::vector<Signature> {
			{ "externalInteger"s, DatumType::Int, 1, offsetof(AttributedFoo, extInt) },
			{ "externalFloat"s, DatumType::Float, 1, offsetof(AttributedFoo, extFloat) },
			{ "externalString"s, DatumType::String, 1, offsetof(AttributedFoo, extString) },
			{ "externalVector"s, DatumType::Vector, 1, offsetof(AttributedFoo, extVector) },
			{ "externalMatrix"s, DatumType::Matrix, 1, offsetof(AttributedFoo, extMatrix) },
			{ "externalPointer"s, DatumType::Pointer, 1, offsetof(AttributedFoo, extPtr) },
			{ "externalIntegerArray"s, DatumType::Int, 4, offsetof(AttributedFoo, extIntArr) },
			{ "externalFloatArray"s, DatumType::Float, 4, offsetof(AttributedFoo, extFloatArr) },
			{ "externalStringArray"s, DatumType::String, 4, offsetof(AttributedFoo, extStringArr) },
			{ "externalVectorArray"s, DatumType::Vector, 4, offsetof(AttributedFoo, extVectorArr) },
			{ "externalMatrixArray"s, DatumType::Matrix, 4, offsetof(AttributedFoo, extMatrixArr) },
		};
	}
}