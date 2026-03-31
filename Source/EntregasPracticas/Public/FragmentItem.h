// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "FragmentItem.generated.h"

/**
 * 
 */

// 1. Declaramos el Delegado. 
// Le pasamos un parámetro (FName) para que avise QUÉ fragmento se recolectó.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFragmentCollectedSignature, FName, CollectedFragmentID);


UCLASS()
class ENTREGASPRACTICAS_API AFragmentItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	AFragmentItem();

	// 2. Instancia del delegado para que otros (como el Assembler) se puedan suscribir
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFragmentCollectedSignature OnFragmentCollected;

protected:
	// 3. Identificador único para este fragmento
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fragment Data")
	FName FragmentID;

public:
	// 4. Sobrescribimos la función de interacción de la clase padre
	virtual void Interact_Implementation(AActor* Interactor) override;
};
