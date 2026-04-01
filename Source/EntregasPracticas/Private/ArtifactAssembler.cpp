#include "ArtifactAssembler.h" // Siempre primero
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h" // Para poder buscar actores en el mundo
#include "FragmentItem.h"           // Para conocer el delegado de los fragmentos

AArtifactAssembler::AArtifactAssembler()
{
	PrimaryActorTick.bCanEverTick = false;

	// Creamos el mesh y lo ponemos como raíz
	AssemblerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AssemblerMesh"));
	RootComponent = AssemblerMesh;

	// Colisión para que en el futuro el rayo de interacción pueda chocar contra él
	AssemblerMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AssemblyTime = 3.0f; // 3 segundos
	bIsReadyToAssemble = false;
	TotalFragmentsInLevel = 0;
	CurrentFragmentsCollected = 0;
}

void AArtifactAssembler::BeginPlay()
{
	Super::BeginPlay();

	// --- PUNTO 7: SUSCRIPCIÓN AL DELEGADO ---

	// 1. Creamos un arreglo vacío para guardar los actores que encontremos
	TArray<AActor*> FoundFragments;

	// 2. Buscamos TODOS los actores de la clase AFragmentItem que estén en el nivel
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFragmentItem::StaticClass(), FoundFragments);

	// 3. Recorremos (bucle) los fragmentos encontrados
	for (AActor* Actor : FoundFragments)
	{
		TotalFragmentsInLevel = FoundFragments.Num(); // Guardamos cuántos fragmentos hay en total en el mapa

		// Nos aseguramos que sea un FragmentItem válido
		if (AFragmentItem* Fragment = Cast<AFragmentItem>(Actor))
		{
			// 4. Nos suscribimos a su radio (delegado). 
			// AddDynamic toma: (Objeto que escucha, y la función que va a ejecutar)
			Fragment->OnFragmentCollected.AddDynamic(this, &AArtifactAssembler::HandleFragmentCollected);
		}
	}
}

void AArtifactAssembler::HandleFragmentCollected(FName FragmentID)
{
	// Sumamos 1 al contador
	CurrentFragmentsCollected++;

	// Validamos si ya juntamos todos usando una CONDICIÓN (Punto 10)
	if (CurrentFragmentsCollected >= TotalFragmentsInLevel && !bIsReadyToAssemble)
	{
		bIsReadyToAssemble = true; // Cambiamos el estado

		// Disparamos el evento visual para el Blueprint
		OnReadyToAssembleVisuals();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("¡Todos los fragmentos recolectados! El ensamblador está listo."));
		}
	}
}

void AArtifactAssembler::Interact_Implementation(AActor* Interactor)
{
	if (!bIsReadyToAssemble)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Aún faltan fragmentos por recolectar."));
		return;
	}

	// Si llegamos acá, es porque está listo.
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Iniciando ensamblaje... Espera 3 segundos."));
	}

	// 1. Apagamos la bandera para que el jugador no pueda apretar la 'E' mil veces seguidas y lanzar 50 timers
	bIsReadyToAssemble = false;

	// 2. Iniciamos el Timer
	// Parámetros: (El Handle, El Objeto que llama a la función, La Función a ejecutar, El Tiempo, Si se repite en bucle (false))
	GetWorld()->GetTimerManager().SetTimer(AssemblyTimerHandle, this, &AArtifactAssembler::CompleteAssembly, AssemblyTime, false);
};

void AArtifactAssembler::CompleteAssembly()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("¡Artefacto final ensamblado!"));
	}

	// Validamos que se haya seleccionado una clase en el Blueprint
	if (ArtifactToSpawn)
	{
		// Calculamos dónde spawnearlo (un poquito más arriba del ensamblador para que no se choquen)
		FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, -150.0f);
		FRotator SpawnRotation = GetActorRotation();

		// Spawneamos el objeto en el mundo
		GetWorld()->SpawnActor<AActor>(ArtifactToSpawn, SpawnLocation, SpawnRotation);
	}

	// Opcional: Destruimos el ensamblador porque ya cumplió su propósito
	Destroy();
}
