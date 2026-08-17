
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CEnemyHealthWidgetComponent.generated.h"


UCLASS()
class DEMONKING_API UCEnemyHealthWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void UpdateHealth(float Hp);


protected:
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<class UCEnemyHealthWidget> EnemyHealthWidget;
	
};
