// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/Helpers/SceneMarker.h"

#include "Data/WeaponPartData.h"


USceneMarker::USceneMarker(): WeaponMarkerType(EWeaponGearPartType::None)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

void USceneMarker::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponPartRow.DataTable || WeaponPartRow.RowName.IsNone())
		return;
	
	const FWeaponPartData* WData = WeaponPartRow.DataTable->FindRow<FWeaponPartData>(WeaponPartRow.RowName, WeaponPartRow.RowName.ToString());
	if (WData)
	{
		StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("StaticMesh"));
		StaticMeshComponent->SetStaticMesh(WData->BaseWeaponPartData.UStaticMesh);
		StaticMeshComponent->RegisterComponent();
		StaticMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void USceneMarker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
