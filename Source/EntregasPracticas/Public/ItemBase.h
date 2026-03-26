// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "ItemBase.generated.h"

UCLASS()
class ENTREGASPRACTICAS_API AItemBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Componente visual básico que tendrán todos los ítems
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class UStaticMeshComponent* ItemMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Declaramos la función de la interfaz. 
	// OJO: Al ser un BlueprintNativeEvent, en C++ se le agrega el sufijo "_Implementation"
	virtual void Interact_Implementation(AActor* Interactor) override;

};
