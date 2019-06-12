#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "MyTestUtils.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectIterator.h"

#include "PPottie.h"
#include "FlowFlow.mock.h"

BEGIN_DEFINE_SPEC(PottieTest, "PotterTests.Pottie", FMyTestUtils::TestFlags)
	UWorld* World;
	APPottie* Pottie;
	TUniquePtr<Potter::FMockFlowFlow> MockFlowFlow;
END_DEFINE_SPEC(PottieTest)

void PottieTest::Define()
{
	using namespace Potter;

	// PRE TEST
	BeforeEach([this]()
	{
		AutomationOpenMap(TEXT("/Game/Maps/Map0"));
		World = FMyTestUtils::GetWorld();
		TestNotNull("World is not null", World);

		MockFlowFlow.Reset(NewObject<FMockFlowFlow>());
	});	
	
	// TESTS
	It("Test Potting Method", [this]()
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(World, APPottie::StaticClass(), Actors);
		Pottie = Cast<APPottie>(Actors[0]);

		Pottie->AutomationDoThePot(Cast<AGetaGameJam7Character>(MockFlowFlow.Get()));

		TestTrue("Pottie is hidden", Pottie->bHidden);
	});

	// AFTER TEST
	AfterEach([this]()
	{
		FMyTestUtils::Exit();
	});
}
