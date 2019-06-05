#include "PotterTests.h"

#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

#include "GetaGameJam7GameMode.h"

// Copy of the hidden method GetAnyGameWorld() in AutomationCommon.cpp.
// Marked as temporary there, hence, this one is temporary, too.
UWorld* GetTestWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();

    for (const FWorldContext& Context : WorldContexts)
    {
        if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game)) && (Context.World() != nullptr))
        {
            return Context.World();
        }
    }

    return nullptr;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameTest, "PotterTests.Game", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter)

bool GameTest::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TEXT("/Game/Maps/Persistent0"));

    UWorld* World = GetTestWorld();

    TestTrue("GameMode class is set correctly", World->GetAuthGameMode()->IsA<AGetaGameJam7GameMode>());

	return true;
}
