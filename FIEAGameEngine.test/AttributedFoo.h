#pragma once

#include "Attributed.h"
#include "Foo.h"

constexpr int arrSize = 4;

namespace Fiea::GameEngine::Test
{
	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);

	public:
		/**
		 * @brief 
		 * This test child class of Attributed is only responsible for initializing its data that corresponds to its prescribed attributes (signatures)
		*/
		AttributedFoo();
		AttributedFoo(const AttributedFoo& rhs);
		AttributedFoo(AttributedFoo&& rhs) noexcept;
		AttributedFoo& operator=(const AttributedFoo & rhs);
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept;
		virtual ~AttributedFoo();

		[[nodiscard]] AttributedFoo* Clone() const override;

		/**
		 * @brief 
		 * Returns the Signature vector containing this class's prescribed attributes
		 * @return 
		*/
		static std::vector<Signature> Signatures();
		

	private:
		int extInt = 4;
		float extFloat = 2.5f;
		std::string extString = "testytest";
		glm::vec4 extVector = { 2.2f, 3.1f, 4.1f, 5.2f };
		glm::mat4x4 extMatrix = { 0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
									0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f };
		Foo* extPtr = new Foo(5);

		int extIntArr[arrSize];
		float extFloatArr[arrSize];
		std::string extStringArr[arrSize];
		glm::vec4 extVectorArr[arrSize];
		glm::mat4x4 extMatrixArr[arrSize];
	};
}