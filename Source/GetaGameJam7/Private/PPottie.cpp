// Fill out your copyright notice in the Description page of Project Settings.


#include "PPottie.h"
#include "GetaGameJam7Character.h"
#include "PaperSpriteComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APPottie::APPottie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	PaperSprite->SetupAttachment(RootComponent);
	PaperSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
	PaperSprite->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	PaperSprite->OnComponentBeginOverlap.AddDynamic(this, &APPottie::OnBeginOverlap);
}

// Called when the game starts or when spawned
void APPottie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPottie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPottie::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (OtherActor == Character)
	{
		SetActorHiddenInGame(true);

		AGetaGameJam7Character* FlowFlow = Cast<AGetaGameJam7Character>(Character);
		FlowFlow->Win();
	}
}

void APPottie::Reset_Implementation()
{
	// Don't call the super method here to avoid weird side effects of the default's engine implementation

	SetActorHiddenInGame(false);
}
