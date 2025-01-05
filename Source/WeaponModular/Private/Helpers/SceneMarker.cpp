// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/Helpers/SceneMarker.h"

#include "Data/WeaponGearData.h"


USceneMarker::USceneMarker(): WeaponMarkerType(EWeaponGearPartSlot::None), RetrievedWeaponPartData(nullptr)
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

	if (FWeaponGearData* WData = WeaponPartRow.DataTable->FindRow<FWeaponGearData>(
		WeaponPartRow.RowName, WeaponPartRow.RowName.ToString()))
	{
		RetrievedWeaponPartData = WData;
		UpdateStaticMeshComponent();
	}
}

void USceneMarker::UpdateStaticMeshComponent()
{
	if (RetrievedWeaponPartData)
	{
		if (!StaticMeshComponent)
		{
			StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(),
			                                                      TEXT("StaticMesh"));
			StaticMeshComponent->RegisterComponent();
		}
		
		StaticMeshComponent->SetStaticMesh(RetrievedWeaponPartData->BaseWeaponPartData.UStaticMesh);
		StaticMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		if (StaticMeshComponent)
		{
			StaticMeshComponent->DestroyComponent();
			StaticMeshComponent = nullptr;
		}
	}
}

void USceneMarker::SetRetrievedWeaponPartData(FWeaponGearData* NewWeaponPartData)
{
	if (RetrievedWeaponPartData == NewWeaponPartData)
		return;
	
	RetrievedWeaponPartData = NewWeaponPartData; 
}

void USceneMarker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
