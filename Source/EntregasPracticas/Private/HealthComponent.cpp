#include "HealthComponent.h"
#include "Math/UnrealMathUtility.h" // Necesario para usar FMath::Clamp
#include "Net/UnrealNetwork.h" // MUY IMPORTANTE PARA MULTIJUGADOR

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Valores por defecto
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	// Le decimos al componente que SE REPLIQUE en red
	SetIsReplicatedByDefault(true);
}

// Reglas de replicación (obligatorio)
void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicamos la vida actual a todos
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Solo el servidor define la vida inicial
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;
	}
}

void UHealthComponent::ModifyHealth(float Amount)
{

	// ¡Solo el Servidor (Authority) puede modificar la vida!
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	// FMath::Clamp evita que la vida baje de 0 o supere la vida máxima
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);

	// Como el servidor no llama al OnRep automáticamente para sí mismo, disparamos el delegado localmente
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	// Mensaje de debug para ver qué está pasando
	if (GEngine)
	{
		FString ActionType = (Amount >= 0) ? TEXT("Curacion") : TEXT("Dano");
		FColor MsgColor = (Amount >= 0) ? FColor::Green : FColor::Red;

		FString Msg = FString::Printf(TEXT("%s recibida. Salud actual: %f"), *ActionType, CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, MsgColor, Msg);
	}
}

// Esto se ejecuta SOLO en los clientes cuando la vida replicada llega desde el servidor
void UHealthComponent::OnRep_CurrentHealth()
{
	// Avisamos a la UI del cliente que la vida cambió
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}
