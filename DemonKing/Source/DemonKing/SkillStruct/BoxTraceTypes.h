#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "BoxTraceTypes.generated.h"

UENUM(BlueprintType)
enum class EBoxTraceDirection : uint8
{
	FORWARD	UMETA(DisplayName = "Forward"),
	BACKWARD UMETA(DisplayName = "Backward"),
	RIGHT UMETA(DisplayName = "Right"),
	LEFT UMETA(DisplayName = "Left")
};

USTRUCT(BlueprintType)
struct DEMONKING_API FBoxTraceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTrace")
	float Value = 1.0f; // 밸류 값

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTrace")
	float MinDistance = 100.0f; // 최소 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTrace")
	float MaxDistance = 300.0f; // 최대 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTrace")
	float StartDistance = 100.0f;


	// 박스 트레이스 값을 더 세밀하게 하기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTrace")
	FVector MinBoxHalfSize = FVector(30.0f, 30.0f, 30.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTrace")
	FVector MaxBoxHalfSize = FVector(100.0f, 80.0f, 80.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	EBoxTraceDirection BoxTraceDirection = EBoxTraceDirection::FORWARD;

};


