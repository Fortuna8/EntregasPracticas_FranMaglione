#include "FragmentComponent.h"

UFragmentComponent::UFragmentComponent()
{
	// Los componentes de datos puros no necesitan hacer Tick cada frame
	PrimaryComponentTick.bCanEverTick = false;

	// Seteamos que por defecto se necesiten 3 fragmentos (lo podés cambiar desde el Blueprint del personaje)
	TotalFragmentsNeeded = 3;
}

void UFragmentComponent::AddFragment(FName FragmentID)
{
	// AddUnique asegura que no agreguemos dos veces el mismo ID por error
	CollectedFragments.AddUnique(FragmentID);

	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Inventario: Fragmento '%s' guardado. Llevas %d de %d."),
			*FragmentID.ToString(), GetCollectedCount(), TotalFragmentsNeeded);
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Cyan, Msg);
	}
}

int32 UFragmentComponent::GetCollectedCount() const
{
	return CollectedFragments.Num();
}

bool UFragmentComponent::IsArtifactComplete() const
{
	return GetCollectedCount() >= TotalFragmentsNeeded;
}
