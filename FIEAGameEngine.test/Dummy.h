#pragma once
#include "Empty.h"

namespace Fiea::GameEngine::Test
{
	class Dummy : public Empty
	{
	public:
		Dummy() = default;

		RTTI_DECLARATIONS(Dummy, Empty);
	};
}