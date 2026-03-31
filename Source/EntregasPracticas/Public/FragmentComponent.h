#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FragmentComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENTREGASPRACTICAS_API UFragmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFragmentComponent();

protected:
	// Cantidad necesaria para armar el artefacto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fragments")
	int32 TotalFragmentsNeeded;

	// Arreglo (Array) para guardar los IDs de los fragmentos que vamos juntando
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fragments")
	TArray<FName> CollectedFragments;

public:
	// Función para agregar un fragmento
	UFUNCTION(BlueprintCallable, Category = "Fragments")
	void AddFragment(FName FragmentID);

	// Función para saber cuántos tenemos (BlueprintPure es para que en Blueprint sea un nodo verde sin pin de ejecución)
	UFUNCTION(BlueprintPure, Category = "Fragments")
	int32 GetCollectedCount() const;

	// Función para validar si ya completamos el objetivo
	UFUNCTION(BlueprintPure, Category = "Fragments")
	bool IsArtifactComplete() const;
};