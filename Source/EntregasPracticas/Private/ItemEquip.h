#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableInterface.h"
#include "Components/SphereComponent.h"
#include "ItemEquip.generated.h"


UCLASS()
class ENTREGASPRACTICAS_API AItemEquip : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	AItemEquip();
	
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "ItemEquip")
	UStaticMeshComponent * ItemEMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "ItemEquip")
	USphereComponent * SphereCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "ItemEquip")
	FName SocketName = "";
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "ItemEquip")
	int ItemDamage = 1;
	
};
