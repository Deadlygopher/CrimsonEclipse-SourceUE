// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "CrimsonEclipse/Projectile/CEProjectileActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	//bReplicates = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	//SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponMesh, TEXT("MeaponMesh Assertion"));
	checkf(ProjectileClass || WeaponType == EWeaponType::EWT_Melee, TEXT("ProjectileClass Assertion"));
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:

		break;
	}
}*/


// SPAWN PROJECTILE REPLICATION //
void AWeapon::SpawnProjectile(APawn* SpawnInstigator, FVector AimVector, float BaseDamage)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = SpawnInstigator;
	SpawnParameters.Owner = SpawnInstigator;
	FVector SpawnLocation = FVector (WeaponMesh->GetSocketLocation("ProjectileSocket"));

	FTransform SpawnTransform(AimVector.Rotation(), SpawnLocation, FVector(1));
	auto Projectile = GetWorld()->SpawnActorDeferred<ACEProjectileActor>(ProjectileClass, SpawnTransform,
		SpawnInstigator, SpawnInstigator);
	Projectile->SetWeaponDamage(BaseDamage);
	Projectile->FinishSpawning(SpawnTransform);
	//UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}

// SPAWN PROJECTILE REPLICATION //

void AWeapon::MakeLightAttackSound() const
{
	UGameplayStatics::PlaySoundAtLocation(this, LightAttackSound, GetActorLocation());
}

void AWeapon::SpawnWeaponTrail()
{
	auto NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(WeaponNiagaraTrail, WeaponMesh, "WeaponAttachPoint",
		WeaponMesh->GetSocketLocation("MiddleTrackingSocket"), WeaponMesh->GetSocketRotation("MiddleTrackingSocket"),
		EAttachLocation::KeepWorldPosition, true, true);

	float TrailWidth = (WeaponMesh->GetSocketLocation("StartTrackingSocket") - WeaponMesh->GetSocketLocation("EndTrackingSocket")).Length();
	float TrailDuration = WeaponLightAttackAnimMontage->GetPlayLength();
	UE_LOG(LogTemp, Warning, TEXT("%f"), TrailDuration);
	NiagaraComponent->SetFloatParameter("TrailWidth", TrailWidth);
}

void AWeapon::CompleteSpawnWeaponTrail()
{
	//if (WeaponNiagaraTrail) WeaponNiagaraTrail->BeginDestroy();
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AWeapon, WeaponState);
	//DOREPLIFETIME(AWeapon, SpawnLocation);
}

