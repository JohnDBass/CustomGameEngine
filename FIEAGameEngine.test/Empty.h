#pragma once

#include "RTTI.h"

namespace Fiea::GameEngine::Test
{
	class Empty : public RTTI
	{
	public:
		Empty() = default;

		RTTI_DECLARATIONS(Empty, RTTI);
		

	};
}