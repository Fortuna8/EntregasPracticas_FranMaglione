#include "HealthComponent.h"
#include "Math/UnrealMathUtility.h" // Necesario para usar FMath::Clamp

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Valores por defecto
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Nos aseguramos de arrancar con la vida al máximo
	CurrentHealth = MaxHealth;
}

void UHealthComponent::ModifyHealth(float Amount)
{
	// FMath::Clamp evita que la vida baje de 0 o supere la vida máxima
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);

	// Mensaje de debug para ver qué está pasando
	if (GEngine)
	{
		FString ActionType = (Amount >= 0) ? TEXT("Curacion") : TEXT("Dano");
		FColor MsgColor = (Amount >= 0) ? FColor::Green : FColor::Red;

		FString Msg = FString::Printf(TEXT("%s recibida. Salud actual: %f"), *ActionType, CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, MsgColor, Msg);
	}
}
