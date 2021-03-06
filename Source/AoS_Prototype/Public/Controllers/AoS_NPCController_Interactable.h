// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/AoS_NPCController.h"
#include "AoS_NPCController_Interactable.generated.h"

UCLASS()
class AOS_PROTOTYPE_API AAoS_NPCController_Interactable : public AAoS_NPCController
{
	GENERATED_BODY()

	UPROPERTY()
	class UBehaviorTree* DialogueTree;

public:

	UFUNCTION()
	void StartDialogue();
	UFUNCTION()
	void EndDialogue();
	
	UFUNCTION()
	void SetDialogueTree(UBehaviorTree* DialogueTreeToSet) {DialogueTree = DialogueTreeToSet;}
	
	UFUNCTION(BlueprintPure)
	UBehaviorTree* GetDialogueTree() {return DialogueTree;}
	
};
