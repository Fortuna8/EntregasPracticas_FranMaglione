// Copyright Epic Games, Inc. All Rights Reserved.


#include "EntregasPracticasPlayerController.h"
#include "HealthWidget.h"
#include "EntregasPracticasCharacter.h"
#include "HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "EntregasPracticas.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AEntregasPracticasPlayerController::BeginPlay()
{
	Super::BeginPlay();


	// NETWORKING: La UI solo se crea si somos el controlador LOCAL
	if (IsLocalController() && HealthWidgetClass)
	{
		HealthWidget = CreateWidget<UHealthWidget>(this, HealthWidgetClass);
		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
		}

		// Buscamos al personaje y su componente de vida para vincularnos
		if (AEntregasPracticasCharacter* MyChar = Cast<AEntregasPracticasCharacter>(GetPawn()))
		{
			if (UHealthComponent* HealthComp = MyChar->FindComponentByClass<UHealthComponent>())
			{
				// Nos vinculamos al evento de cambio de vida
				HealthComp->OnHealthChanged.AddDynamic(this, &AEntregasPracticasPlayerController::HandleHealthChanged);

				// Inicializamos la barra con los valores actuales
				HealthWidget->UpdateHealthBar(HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
			}
		}
	}

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogEntregasPracticas, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AEntregasPracticasPlayerController::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (HealthWidget)
	{
		HealthWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void AEntregasPracticasPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}
