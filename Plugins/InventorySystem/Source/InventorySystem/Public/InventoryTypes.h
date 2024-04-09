#pragma once

#include "InventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FInvPoint2D
{
	GENERATED_USTRUCT_BODY()

	FInvPoint2D()
	{
		X = 0;
		Y = 0;
	}

	FInvPoint2D(const int32 InX, const int32 InY)
	{
		X = InX;
		Y = InY;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, UIMin = 0))
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, UIMin = 0))
	int32 Y;

	bool operator == (const FInvPoint2D& Other) const
	{
		return Other.X == X && Other.Y == Y;
	}

	FInvPoint2D operator + (const FInvPoint2D& Other) const
	{
		return FInvPoint2D(Other.X + X, Other.Y + Y);
	}

	bool operator > (const FInvPoint2D& Other) const
	{
		return Other.X > X && Other.Y > Y;
	}

	bool operator >= (const FInvPoint2D& Other) const
	{
		return Other.X >= X && Other.Y >= Y;
	}

	bool operator < (const FInvPoint2D& Other) const
	{
		return (Other.X < X && Other.Y < Y);
	}

	bool operator <= (const FInvPoint2D& Other) const
	{
		return (Other.X <= X && Other.Y <= Y);
	}
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common								UMETA(DisplayName = "Common"),
	EIR_Uncommon							UMETA(DisplayName = "Uncommon"),
	EIR_Rare								UMETA(DisplayName = "Rare"),
	EIR_Epic								UMETA(DisplayName = "Epic"),
	EIR_Legendary							UMETA(DisplayName = "Legendary"),
};