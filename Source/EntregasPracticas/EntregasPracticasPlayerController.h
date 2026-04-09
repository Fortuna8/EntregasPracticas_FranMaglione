// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EntregasPracticasPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AEntregasPracticasPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	// La clase del Widget que vamos a usar
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UHealthWidget> HealthWidgetClass;

	// Referencia al widget ya creado
	UPROPERTY()
	UHealthWidget* HealthWidget;

	// Función que se ejecutará cuando el delegado de vida avise un cambio
	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

};
