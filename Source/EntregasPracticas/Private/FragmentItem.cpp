// Fill out your copyright notice in the Description page of Project Settings.


#include "FragmentItem.h"

AFragmentItem::AFragmentItem()
{
	// Le damos un ID por defecto para que no esté vacío
	FragmentID = TEXT("Default_ID");
}

void AFragmentItem::Interact_Implementation(AActor* Interactor)
{
	// Primero, llamamos a la lógica base si la hubiera (opcional, pero buena práctica)
	Super::Interact_Implementation(Interactor);

	// 1. Notificamos a todo el que esté escuchando que este fragmento se recolectó
	OnFragmentCollected.Broadcast(FragmentID);

	// 2. Agregar al sistema del personaje.
	// NOTA: Como la consigna 5 nos pide crear el FragmentComponent después, 
	// por ahora dejamos el log preparado. Cuando hagamos el punto 5, conectamos esto.
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("Fragmento recolectado: %s"), *FragmentID.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, Message);
	}

	// 3. Destruimos el actor para que desaparezca del mundo y no se pueda agarrar dos veces
	Destroy();
}