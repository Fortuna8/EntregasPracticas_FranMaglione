#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h" // Le ponemos la interfaz porque en el Punto 8/9 vamos a interactuar con él
#include "ArtifactAssembler.generated.h"

UCLASS()
class ENTREGASPRACTICAS_API AArtifactAssembler : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AArtifactAssembler();

protected:
	virtual void BeginPlay() override;

	// Componente visual del ensamblador
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assembler")
	class UStaticMeshComponent* AssemblerMesh;

	// Función que se va a ejecutar ("reaccionar") cada vez que un fragmento avise que fue recolectado.
	// OJO: Para que funcione con delegados dinámicos de Unreal, DEBE tener el macro UFUNCTION().
	UFUNCTION()
	void HandleFragmentCollected(FName FragmentID);


	// --- PUNTO 8: ESTADO Y VALIDACIÓN ---
	// Variable para saber si ya podemos interactuar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assembler State")
	bool bIsReadyToAssemble;

	// Contadores
	int32 TotalFragmentsInLevel;
	int32 CurrentFragmentsCollected;

	// Evento que llamamos en C++ pero diseñamos visualmente en el Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Assembler Visuals")
	void OnReadyToAssembleVisuals();

	// --- PUNTO 9: TIMER Y SPAWN ---

	// Estructura que maneja el tiempo en Unreal
	FTimerHandle AssemblyTimerHandle;

	// Cuántos segundos tarda en ensamblarse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembler Settings")
	float AssemblyTime;

	// Qué clase de objeto vamos a spawnear como premio final
	// TSubclassOf nos permite elegir cualquier Actor desde el Blueprint de forma segura
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assembler Settings")
	TSubclassOf<AActor> ArtifactToSpawn;

	// La función que se va a ejecutar cuando el Timer llegue a cero
	void CompleteAssembly();

public:	
	// Preparado para las consignas 8 y 9
	virtual void Interact_Implementation(AActor* Interactor) override;
};
