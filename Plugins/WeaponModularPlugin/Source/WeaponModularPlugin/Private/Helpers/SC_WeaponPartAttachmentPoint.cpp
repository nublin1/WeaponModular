// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/SC_WeaponPartAttachmentPoint.h"

#include "Data/WeaponPartData.h"


USC_WeaponPartAttachmentPoint::USC_WeaponPartAttachmentPoint(): RetrievedWeaponPartData(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_WeaponPartAttachmentPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void USC_WeaponPartAttachmentPoint::Initialize()
{
	if (!InitialWeaponPartRow.DataTable || InitialWeaponPartRow.RowName.IsNone())
		return;

	if (FWeaponPartData* WData = InitialWeaponPartRow.DataTable->FindRow<FWeaponPartData>(
		InitialWeaponPartRow.RowName, InitialWeaponPartRow.RowName.ToString()))
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
		if(StaticMeshComponent)
			StaticMeshComponent->DestroyComponent();
		
		StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(),
			                                                      TEXT("StaticMesh"));
		StaticMeshComponent->RegisterComponent();
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

	if (OnStaticMeshUpdated.IsBound())
		OnStaticMeshUpdated.Broadcast();
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
