// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "DamageItem.generated.h"

/**
 * 
 */
UCLASS()
class ENTREGASPRACTICAS_API ADamageItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	ADamageItem();

protected:
	// Variable para definir cuánto daño o cuántos puntos quita este ítem
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Settings")
	float DamageAmount;

public:
	// Sobrescribimos la función de interacción
	virtual void Interact_Implementation(AActor* Interactor) override;
};
