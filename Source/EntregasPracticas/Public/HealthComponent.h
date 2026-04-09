#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// Declaramos el delegado que la UI va a escuchar (necesita la vida actual y la máxima)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, MaxHealth);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENTREGASPRACTICAS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	// Instancia del delegado
	UPROPERTY(BlueprintAssignable, Category = "Health Events")
	FOnHealthChangedSignature OnHealthChanged;
		
protected:
	virtual void BeginPlay() override;

	// Vida máxima del personaje
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	// Vida actual
	// Le decimos a Unreal: "Cuando esta variable cambie en la red, ejecutá OnRep_CurrentHealth"
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	// 3. Función que se ejecuta en los CLIENTES cuando reciben el nuevo valor de vida
	UFUNCTION()
	void OnRep_CurrentHealth();

public:

	// Función OBLIGATORIA cuando replicamos variables en un componente
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Función central para curar (valores positivos) o dañar (valores negativos)
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ModifyHealth(float Amount);

	// Getter para saber cuánta vida tenemos
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }
};