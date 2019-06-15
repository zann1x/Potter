// Fill out your copyright notice in the Description page of Project Settings.


#include "PLava.h"
#include "PaperSpriteComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
APLava::APLava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	PaperSprite->SetupAttachment(RootComponent);
	PaperSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	PaperSprite->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	PaperSprite->OnComponentBeginOverlap.AddDynamic(this, &APLava::OnBeginOverlap);
}

// Called when the game starts or when spawned
void APLava::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APLava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APLava::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->TakeDamage(500.0f, FDamageEvent(), nullptr, this);
}
