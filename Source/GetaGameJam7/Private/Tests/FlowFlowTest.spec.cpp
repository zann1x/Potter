#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "MyTestUtils.h"

#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
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

		FPlatformProcess::Sleep(0.2f);

		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			TestTrue("Character is dead", Character->IsDead());
		});

		TestDone.Execute();
	});

	LatentIt("Test Reset Method", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			Character->Reset();

			TestTrue("Character has full health", Character->GetWaterLevel() == Character->MaxWaterLevel);

			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), Actors);
			APlayerStart* PlayerStart = Cast<APlayerStart>(Actors[0]);
			TestTrue("Character is at player start position", Character->GetActorLocation() == PlayerStart->GetActorLocation());
		});

		FPlatformProcess::Sleep(0.2f);

		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
			TestTrue("Character is set to idle", Character->IsIdle());
		});

		TestDone.Execute();
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
			TestTrue("Character is in potted state", Character->IsPotted());
		});

		TestDone.Execute();
	});

	It("Test Take Damage Method", [this]()
	{
		AGetaGameJam7Character* Character = Cast<AGetaGameJam7Character>(World->GetFirstPlayerController()->GetCharacter());
		float InitialWaterLevel = Character->GetWaterLevel();
		
		float DamageAmount = 100.0f;
		float TakenDamage = Character->TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);
		
		TestTrue("Taken damage is as big as applied damage", TakenDamage == DamageAmount);
		TestTrue("Character's health decreased by amount of damage", Character->GetWaterLevel() == InitialWaterLevel - DamageAmount);
	});

	// AFTER TEST
	AfterEach([this]()
	{
		FMyTestUtils::Exit();
	});
}
