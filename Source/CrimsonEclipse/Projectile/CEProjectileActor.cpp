// Fill out your copyright notice in the Description page of Project Settings.


#include "CEProjectileActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogProjectileActor);

ACEProjectileActor::ACEProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileComponent->InitialSpeed = 1000; //Magic Number

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(ProjectileMesh);
	CapsuleComponent->SetCapsuleHalfHeight(100);
	CapsuleComponent->SetCapsuleRadius(10);

}

void ACEProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	checkf(ProjectileComponent, TEXT("Projectile component Assertion"));
	
	CapsuleComponent->MoveIgnoreActors.Add(this);
	CapsuleComponent->MoveIgnoreActors.Add(GetOwner());
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ACEProjectileActor::OnProjectileHit);
}

void ACEProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEProjectileActor::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetOwner() != OtherActor)
	{
		if (OtherActor->IsA(APawn::StaticClass()))
		{
			ProjectileComponent->Deactivate();
			CapsuleComponent->OnComponentBeginOverlap.RemoveAll(this);
			CapsuleComponent->Deactivate();
			ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigator()->GetController(),
				GetInstigator(), UDamageType::StaticClass());

			auto SkeletalMesh = Cast<USkeletalMeshComponent>(SweepResult.Component);
			if (!SkeletalMesh) return;
			//auto ClosestBone = SkeletalMesh->FindClosestBone(ProjectileMesh->GetComponentLocation());
			auto ClosestBone = SkeletalMesh->FindClosestBone(GetActorLocation());
			FAttachmentTransformRules AttachmentTransform{ EAttachmentRule::KeepWorld, true };
			AttachToComponent(SkeletalMesh, AttachmentTransform, ClosestBone);
			//UE_LOG(LogProjectileActor, Warning, TEXT("%s"), *GetInstigator()->GetName());
			UE_LOG(LogProjectileActor, Warning, TEXT("%s"), *ClosestBone.ToString());
		}
		else
		{
			ProjectileComponent->Deactivate();
			CapsuleComponent->OnComponentBeginOverlap.RemoveAll(this);
			CapsuleComponent->Deactivate();
			ProjectileMesh->SetSimulatePhysics(true);
			//UE_LOG(LogProjectileActor, Warning, TEXT("Not Actor %s"), *OtherActor->GetName());
		}
	}
}
