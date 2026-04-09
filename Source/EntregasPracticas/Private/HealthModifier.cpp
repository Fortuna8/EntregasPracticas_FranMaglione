#include "HealthModifier.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h" // Incluimos tu nuevo componente
#include "TimerManager.h"    // Para manejar el tiempo en bucle

AHealthModifier::AHealthModifier()
{
	PrimaryActorTick.bCanEverTick = false;

	ModifierZone = CreateDefaultSubobject<UBoxComponent>(TEXT("ModifierZone"));
	RootComponent = ModifierZone;
	ModifierZone->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	ModifierZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ModifierZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	ModifierZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Valores por defecto
	ModifierType = EModifierType::Damage;
	EffectAmount = 10.0f; // Saca/Cura 10 puntos
	TickInterval = 1.0f;  // Cada 1 segundo
	TickCount = 0;

	// Creamos el componente visual
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));

	// Lo "enganchamos" (attach) a la caja de colisión, que es nuestro RootComponent
	VisualMesh->SetupAttachment(RootComponent);

	// Apagamos la colisión del mesh visual. 
	// Queremos que el BoxComponent sea el ÚNICO que detecte si el jugador entra o sale.
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// Creamos la luz puntual
	ZoneLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ZoneLight"));

	// La enganchamos a la raíz (la caja de colisión) para que quede en el centro de la zona
	ZoneLight->SetupAttachment(RootComponent);

	// Le damos algunos valores por defecto para que se note apenas lo pongas en el nivel
	ZoneLight->Intensity = 3000.0f;
	ZoneLight->AttenuationRadius = 300.0f; // El radio que ilumina (un poco más grande que tu caja de 200x200x200)

	// Colores por defecto (Rojo para daño, Verde para curación)
	DamageColor = FLinearColor::Red;
	HealingColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Verde puro
}

void AHealthModifier::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 1. Determinamos qué color usar según el Enum
	FLinearColor SelectedColor = (ModifierType == EModifierType::Damage) ? DamageColor : HealingColor;

	// 2. Aplicamos el color a la luz
	if (ZoneLight)
	{
		ZoneLight->SetLightColor(SelectedColor);
	}

	// 3. Aplicamos el color al material del Mesh
	if (VisualMesh)
	{
		// Creamos el material dinámico si no existe (usando el material que ya tenga el mesh)
		if (!DynamicMaterial)
		{
			DynamicMaterial = VisualMesh->CreateDynamicMaterialInstance(0);
		}

		if (DynamicMaterial)
		{
			// OJO: Tu material en Unreal debe tener un parámetro de tipo Vector llamado "Color"
			DynamicMaterial->SetVectorParameterValue(TEXT("Color"), SelectedColor);
		}
	}
}

void AHealthModifier::BeginPlay()
{
	Super::BeginPlay();

	ModifierZone->OnComponentBeginOverlap.AddDynamic(this, &AHealthModifier::OnOverlapBegin);
	ModifierZone->OnComponentEndOverlap.AddDynamic(this, &AHealthModifier::OnOverlapEnd);
}

void AHealthModifier::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// Buscamos si el actor que entró tiene la "mochila" de salud
		TargetHealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();

		if (TargetHealthComponent)
		{
			// Reiniciamos el contador
			TickCount = 0;

			// --- PUNTO 3: TIMER EN BUCLE ---
			// El parámetro final en 'true' significa que el timer se repite infinitamente
			GetWorld()->GetTimerManager().SetTimer(ModifierTimerHandle, this, &AHealthModifier::ApplyEffectTick, TickInterval, true);
		}
	}
}

void AHealthModifier::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		// Verificamos si el que sale es el mismo que estábamos afectando
		UHealthComponent* ExitingComponent = OtherActor->FindComponentByClass<UHealthComponent>();

		if (ExitingComponent && ExitingComponent == TargetHealthComponent)
		{
			// Frenamos y limpiamos el Timer
			GetWorld()->GetTimerManager().ClearTimer(ModifierTimerHandle);

			// Vaciamos el puntero por seguridad
			TargetHealthComponent = nullptr;

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Efecto detenido. Personaje salió de la zona."));
		}
	}
}

void AHealthModifier::ApplyEffectTick()
{
	if (TargetHealthComponent)
	{
		// Convertimos el Enum en matemática: Si es daño, el número se vuelve negativo
		float FinalAmount = (ModifierType == EModifierType::Damage) ? -EffectAmount : EffectAmount;

		// Aplicamos el efecto al componente de vida
		TargetHealthComponent->ModifyHealth(FinalAmount);

		// Sumamos 1 al contador
		TickCount++;

		// Disparamos el delegado para avisar cuántos ticks van
		OnModifierTick.Broadcast(TickCount);
	}
}