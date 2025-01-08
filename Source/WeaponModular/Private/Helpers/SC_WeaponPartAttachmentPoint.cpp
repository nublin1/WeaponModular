// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/Helpers/SC_WeaponPartAttachmentPoint.h"

#include "Data/WeaponPartData.h"


USC_WeaponPartAttachmentPoint::USC_WeaponPartAttachmentPoint(): RetrievedWeaponPartData(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_WeaponPartAttachmentPoint::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponPartRow.DataTable || WeaponPartRow.RowName.IsNone())
		return;

	if (FWeaponPartData* WData = WeaponPartRow.DataTable->FindRow<FWeaponPartData>(
		WeaponPartRow.RowName, WeaponPartRow.RowName.ToString()))
	{
		RetrievedWeaponPartData = WData;
		WeaponPointType = RetrievedWeaponPartData->BaseWeaponPartData.TypeProperties;
		UpdateStaticMeshComponent();
	}
}

void USC_WeaponPartAttachmentPoint::UpdateStaticMeshComponent()
{
	if (RetrievedWeaponPartData)
	{
		if (!StaticMeshComponent)
		{
			StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(),
			                                                      TEXT("StaticMesh"));
			StaticMeshComponent->RegisterComponent();
		}
		
		StaticMeshComponent->SetStaticMesh(RetrievedWeaponPartData->BaseWeaponPartData.VisualProperties.StaticMesh);
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

void USC_WeaponPartAttachmentPoint::SetRetrievedWeaponPartData(FWeaponPartData* NewWeaponPartData)
{
	if (RetrievedWeaponPartData == NewWeaponPartData)
		return;
	
	RetrievedWeaponPartData = NewWeaponPartData; 
}

void USC_WeaponPartAttachmentPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
