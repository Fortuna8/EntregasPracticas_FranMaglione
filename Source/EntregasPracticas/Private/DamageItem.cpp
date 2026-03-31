// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageItem.h"
#include "Kismet/GameplayStatics.h" // Necesario para usar el sistema de daño de Unreal

ADamageItem::ADamageItem()
{
	// Seteamos un valor de daño por defecto
	DamageAmount = 25.0f;
}

void ADamageItem::Interact_Implementation(AActor* Interactor)
{
	// Llamamos a la lógica base (el mensaje amarillo que pusimos en ItemBase, si querés podés borrar el Super para que no salga)
	Super::Interact_Implementation(Interactor);

	if (Interactor)
	{
		// Usamos el sistema de daño estándar de Unreal Engine
		UGameplayStatics::ApplyDamage(Interactor, DamageAmount, nullptr, this, UDamageType::StaticClass());

		// Mensaje en pantalla para validar que funciona
		if (GEngine)
		{
			FString Message = FString::Printf(TEXT("Auch! El jugador recibio %f de dano."), DamageAmount);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Message);
		}
	}

	// Destruimos el actor para que sea una trampa de un solo uso
	Destroy();
}