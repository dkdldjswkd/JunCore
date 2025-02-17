#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkClient.h"
#include "MyGameInstance.generated.h"

UCLASS() 
class TEST_CLIENT_2_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
};