// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "CrimsonEclipse/Projectile/CEProjectileActor.h"


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

	UE_LOG(LogTemp, Warning, TEXT("WEAPON SPAWNED"));
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


void AWeapon::SpawnProjectile(APawn* SpawnInstigator, FVector AimVector)
{
	const FVector SocketLocation = WeaponMesh->GetSocketLocation("ProjectileSocket");
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = SpawnInstigator;
	SpawnParameters.Owner = SpawnInstigator;
	FVector SpawnLocation = SocketLocation;
	GetWorld()->SpawnActor<ACEProjectileActor>(ProjectileClass, SpawnLocation, AimVector.Rotation(), SpawnParameters);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AWeapon, WeaponState);
}

