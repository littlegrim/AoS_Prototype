// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AoS_GameInstance.generated.h"

class UAoS_CaseManager;
class AAoS_Nick;
class UAoS_CharacterData;
class APlayerStart;
class AAoS_Character;

UENUM(BlueprintType)
enum EPlayerMode
{
	PM_ExplorationMode     UMETA(DisplayName = "ExplorationMode"),
	PM_DialogueMode      UMETA(DisplayName = "DialogueMode"),
	PM_ObservationMode   UMETA(DisplayName = "ObservationMode"),
	PM_InspectionMode   UMETA(DisplayName = "InspectionMode"),
	PM_InterrogationMode   UMETA(DisplayName = "nterrogationMode"),
	PM_GameMenuMode   UMETA(DisplayName = "GameMenuMode"),
	PM_VendorMode   UMETA(DisplayName = "VendorMode"),
  };

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerModeChanged, EPlayerMode, NewPlayerMode);

UCLASS()
class AOS_PROTOTYPE_API UAoS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	UAoS_GameInstance();

	UPROPERTY(BlueprintAssignable, Category = "PlayerData")
	FOnPlayerModeChanged OnPlayerModeChanged;

	UPROPERTY(EditAnywhere, Category = "PlayerData")
	UAoS_CharacterData* CDA_NickSpade;
	
	UFUNCTION(Exec, Category = Cases)
	void ResetCase(FString CaseToResetName);
	
	UFUNCTION(BlueprintCallable)
	void SpawnPlayer();
	
	UFUNCTION(BlueprintPure, Category = "PlayerData")
	EPlayerMode GetPlayerMode() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerData")
	void SetPlayerMode(EPlayerMode InPlayerMode);
		
private:

	UPROPERTY()
	UAoS_CaseManager* CaseManager;
	UPROPERTY()
	AAoS_Nick* NickSpadeCharacter;
	UPROPERTY()
	TSubclassOf<AAoS_Character> NickClassRef;
	EPlayerMode PlayerMode;

	virtual void Init() override;

	APlayerStart* GetPlayerStart() const;
	
};
