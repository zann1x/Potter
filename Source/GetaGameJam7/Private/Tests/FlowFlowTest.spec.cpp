#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "MyTestUtils.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectIterator.h"

#include "GetaGameJam7Character.h"

BEGIN_DEFINE_SPEC(FlowFlowTest, "PotterTests.FlowFlow", FMyTestUtils::TestFlags)
	UWorld* World;
END_DEFINE_SPEC(FlowFlowTest)

void FlowFlowTest::Define()
{
	// PRE TEST
	BeforeEach([this]()
	{
		AutomationOpenMap(TEXT("/Game/Maps/Map0"));
		World = FMyTestUtils::GetWorld();
		TestNotNull("World is not null", World);
	});	
	
	// TESTS
	LatentIt("Test Kill Method", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			Character->Kill();
		});

		FPlatformProcess::Sleep((0.2f));

		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			TestTrue("Character is dead", Character->AutomationIsDead());
		});

		TestDone.Execute();
	});

	It("Test Reset Method", [this]()
	{
		AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
		Character->Reset();

		TestTrue("Character is set to idle", Character->AutomationIsIdle());
		TestTrue("Character has full health", Character->AutomationHasFullHealth());

		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), Actors);
		APlayerStart* PlayerStart = Cast<APlayerStart>(Actors[0]);
		TestTrue("Character is at start position", Character->GetActorLocation() == PlayerStart->GetActorLocation());
	});

	LatentIt("Test Win Method", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			Character->Win();
		});
		
		FPlatformProcess::Sleep(0.2f);

		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			TestTrue("Character is in potted state", Character->AutomationIsPotted());
		});

		TestDone.Execute();
	});

	// AFTER TEST
	AfterEach([this]()
	{
		FMyTestUtils::Exit();
	});
}
