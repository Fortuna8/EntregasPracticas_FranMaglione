#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

UCLASS()
class ENTREGASPRACTICAS_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Esta función la llamaremos desde el PlayerController
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

protected:
	// meta = (BindWidget) busca un ProgressBar llamado "HealthBar" en el Blueprint
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	// Opcional: Para mostrar números (debe haber un TextBlock llamado "HealthText")
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;
};