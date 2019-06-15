#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "MyTestUtils.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "PPottie.h"

BEGIN_DEFINE_SPEC(PottieTest, "PotterTests.Pottie", FMyTestUtils::TestFlags)
	UWorld* World;
	APPottie* Pottie;
END_DEFINE_SPEC(PottieTest)

void PottieTest::Define()
{
	// PRE TEST
	BeforeEach([this]()
	{
		AutomationOpenMap(TEXT("/Game/Maps/Map0"));
		World = FMyTestUtils::GetWorld();
		TestNotNull("World is not null", World);
	});	
	
	// TESTS
	It("Test Reset Method", [this]()
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(World, APPottie::StaticClass(), Actors);
		Pottie = Cast<APPottie>(Actors[0]);

		Pottie->Reset();

		TestTrue("Pottie is hidden", !Pottie->bHidden);
	});

	// AFTER TEST
	AfterEach([this]()
	{
		FMyTestUtils::Exit();
	});
}
