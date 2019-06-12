#pragma once

#if WITH_DEV_AUTOMATION_TESTS

#include "GetaGameJam7Character.h"

namespace Potter
{
	class FMockFlowFlow
		: public AGetaGameJam7Character
	{
	public:
		virtual void Win()
		{
			AnimState = EAnimationState::POTTED;
		}

	public:
		EAnimationState AnimState;
	};
}

#endif // WITH_DEV_AUTOMATION_TESTS
