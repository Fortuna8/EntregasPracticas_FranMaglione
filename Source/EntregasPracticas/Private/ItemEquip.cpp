//"C:\Users\ffmag\Documents\Github\EntregasPracticas_FranMaglione\Intermediate\Build\Win64\x64\EntregasPracticasEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"

#include "ItemEquip.h"
#include "Components/SphereComponent.h"

// Valores por defauklt
AItemEquip::AItemEquip()
{
	ItemEMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemEMesh");
	SetRootComponent(ItemEMesh);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("StaticMeshComponent");
	SphereCollision->SetupAttachment(ItemEMesh);
	SphereCollision->SetSphereRadius(100.f);
}
 

void AItemEquip::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!OtherActor) return;
	if (!HasAuthority()) return;
	if (OtherActor -> Implements<IInteractableInterface>())
	{
		USkeletalMeshComponent* SkeletalMeshComponent = IInteractableInterface:: Execute_GetSkeletalMesh(OtherActor);
		AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}
