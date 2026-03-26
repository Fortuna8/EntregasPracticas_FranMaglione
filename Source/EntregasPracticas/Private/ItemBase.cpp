// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Creamos el componente de malla estática y lo hacemos la raíz del actor
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	// Configuramos la colisión para que bloquee el canal de visibilidad.
	// Esto es crucial para que el Line Trace (rayo) de nuestro personaje choque contra el ítem.
	ItemMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::Interact_Implementation(AActor* Interactor)
{
	// Como esta es la clase "Base", por ahora solo ponemos un mensaje de prueba.
	// Más adelante, las clases hijas (FragmentItem, DamageItem) van a reemplazar 
	// este comportamiento con el suyo propio.
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Interactuaste con un ItemBase genérico!"));
	}
}
