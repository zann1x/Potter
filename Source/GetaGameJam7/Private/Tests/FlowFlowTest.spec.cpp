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
	BeforeEach([this]()
	{
		AutomationOpenMap(TEXT("/Game/Maps/Map0"));
		World = FMyTestUtils::GetWorld();
		TestNotNull("World is not null", World);
	});	
	
	LatentIt("Test Kill Method", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			Character->Kill();
		});

		FPlatformProcess::Sleep((0.5f));

		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			TestTrue("Character is dead", Character->AutomationIsDead());
		});

		TestDone.Execute();
	});

	//It("Test Kill Method", [this]()
	//{
	//	AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
	//	Character->Kill();
	//	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
	//	TestTrue("Character is dead", Character->AutomationIsDead());
	//});

	AfterEach([this]()
	{
		FMyTestUtils::Exit();
	});
}
