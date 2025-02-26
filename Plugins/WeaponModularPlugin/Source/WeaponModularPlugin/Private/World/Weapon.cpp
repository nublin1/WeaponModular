// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Helpers/SC_WeaponPartAttachmentPoint.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	TArray<USC_WeaponPartAttachmentPoint*> Components;
	GetComponents<USC_WeaponPartAttachmentPoint>(Components);

	WeaponMarkers = Components;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	InitAttachPoints();
}

void AWeapon::InitAttachPoints()
{
	if (WeaponMarkers.IsEmpty())
		return;

	for (int i=0; i<WeaponMarkers.Num(); i++)
	{
		WeaponMarkers[i]->Initialize();
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

