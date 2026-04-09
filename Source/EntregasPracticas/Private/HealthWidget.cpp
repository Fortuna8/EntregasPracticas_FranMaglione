#include "HealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		// El ProgressBar de Unreal va de 0.0 a 1.0
		float Percent = CurrentHealth / MaxHealth;
		HealthBar->SetPercent(Percent);
	}

	if (HealthText)
	{
		FString HealthString = FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth));
		HealthText->SetText(FText::FromString(HealthString));
	}
}