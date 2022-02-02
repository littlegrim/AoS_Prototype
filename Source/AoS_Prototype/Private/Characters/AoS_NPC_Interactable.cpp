// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AoS_NPC_Interactable.h"
#include "Components/Actor/AoS_DialogueComponent.h"
#include "Components/Scene/AoS_InteractableComponent.h"

AAoS_NPC_Interactable::AAoS_NPC_Interactable()
{
	DialogueComponent = CreateDefaultSubobject<UAoS_DialogueComponent>(TEXT("DialogueComponent"));
	InteractableComponent = CreateDefaultSubobject<UAoS_InteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(RootComponent);
}
