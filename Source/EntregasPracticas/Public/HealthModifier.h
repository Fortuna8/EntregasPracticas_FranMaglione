#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthModifier.generated.h"

class UBoxComponent;
class UHealthComponent; // Forward declaration de nuestro nuevo componente

// --- PUNTO 2: OPCIÓN CONFIGURABLE (ENUM) ---
UENUM(BlueprintType)
enum class EModifierType : uint8
{
	Damage UMETA(DisplayName = "Zona de Daño"),
	Healing UMETA(DisplayName = "Zona de Curación")
};

// --- PUNTO 4: EVENTO DE CONTEO (DELEGADO) ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierTickSignature, int32, CurrentTickCount);

UCLASS()
class ENTREGASPRACTICAS_API AHealthModifier : public AActor
{
	GENERATED_BODY()

public:
	AHealthModifier();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifier Zone")
	UBoxComponent* ModifierZone;

	// Variables configurables desde el editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier Settings")
	EModifierType ModifierType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier Settings")
	float EffectAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier Settings")
	float TickInterval;

	// Manejador del Timer y contador de ticks
	FTimerHandle ModifierTimerHandle;
	int32 TickCount;

	// Puntero para guardar a quién estamos afectando actualmente
	UPROPERTY()
	UHealthComponent* TargetHealthComponent;

	// Componente visual para ver la zona en el nivel
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifier Visuals")
	class UStaticMeshComponent* VisualMesh;

	// Luz para darle ambientación a la zona
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifier Visuals")
	class UPointLightComponent* ZoneLight;

	// Función que se ejecutará en cada Tick del Timer
	void ApplyEffectTick();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Función que se ejecuta cuando movemos el actor o cambiamos una variable en el editor
	virtual void OnConstruction(const FTransform& Transform) override;

	// Colores configurables para cada modo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier Visuals")
	FLinearColor DamageColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier Visuals")
	FLinearColor HealingColor;

	// Variable para guardar el material dinámico
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterial;

public:
	// Instancia del delegado para usar en Blueprints
	UPROPERTY(BlueprintAssignable, Category = "Modifier Events")
	FOnModifierTickSignature OnModifierTick;
};
