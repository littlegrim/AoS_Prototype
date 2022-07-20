// Fill out your copyright notice in the Description page of Project Settings.


#include "Cases/AoS_CaseManager.h"
#include "Cases/AoS_Case.h"
#include "Cases/AoS_Part.h"
#include "Cases/AoS_Objective.h"

UAoS_CaseManager::UAoS_CaseManager()
{
	
}

void UAoS_CaseManager::AcceptCase(UAoS_Case* CaseToAccept)
{
	if (!CaseToAccept)
	{
		return;
	}
	
	if (!AcceptedCases.Contains(CaseToAccept))
	{
		if (!ActiveCase)
		{
			SetActiveCase(CaseToAccept);
		}
	
		AcceptedCases.AddUnique(CaseToAccept);
		OnCaseAccepted.Broadcast(CaseToAccept);	
	}
}

void UAoS_CaseManager::SetActiveCase(UAoS_Case* CaseToSet)
{
	if (!CaseToSet)
	{
		return;
	}
	
	if (ActiveCase)
	{
		ActiveCase->SetCaseIsActive(false, this);
	}
	
	CaseToSet->SetCaseIsActive(true, this);
	ActiveCase = CaseToSet;
	OnCaseActivated.Broadcast(CaseToSet);
}

void UAoS_CaseManager::CompleteObjective(UAoS_Objective* ObjectiveToComplete)
{
	if (!ActiveCase || !ActivePart || !ObjectiveToComplete)
	{
		return;
	}
	
	for (const UAoS_Objective* CurrentObjective : ActivePart->GetActiveObjectives())
	{
		if (ObjectiveToComplete == CurrentObjective && CurrentObjective->GetObjectiveIsActive())
		{
			ObjectiveToComplete->SetObjectiveComplete(true);
			ObjectiveCompleted(ObjectiveToComplete);
		}
	}
}

void UAoS_CaseManager::ResetCases()
{
	for (UAoS_Case* AcceptedCase : AcceptedCases)
	{
		AcceptedCase->ResetCase();
	}
	AcceptedCases.Empty();
	CompletedCases.Empty();
}

UAoS_Case* UAoS_CaseManager::GetActiveCase() const
{
	return ActiveCase;
}

TArray<UAoS_Case*> UAoS_CaseManager::GetAcceptedCases() const
{
	return AcceptedCases;
}

TArray<UAoS_Case*> UAoS_CaseManager::GetCompletedCases() const
{
	return CompletedCases;
}

UAoS_Part* UAoS_CaseManager::GetActivePart() const
{
	return ActivePart;
}

TArray<UAoS_Objective*> UAoS_CaseManager::GetActiveObjectives() const
{
	return ActiveObjectives;
}

void UAoS_CaseManager::SetActivePart(UAoS_Part* PartToSet)
{
	ActivePart = PartToSet;
}

void UAoS_CaseManager::SetActiveObjectives(TArray<UAoS_Objective*> ObjectivesToSet)
{
	ActiveObjectives = ObjectivesToSet;
}

void UAoS_CaseManager::ObjectiveCompleted(UAoS_Objective* CompletedObjective)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Objective Completed Called!"));	

	if (!CompletedObjective)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Objective Pointer Null!"));	
		return;
	}
	
	if(!CheckForCompletedPart())
	{
		if (ActivePart->bCompleteObjectivesInOrder)
		{
			ActiveObjectives.Empty();
			ActivePart->ActivateObjectives(this);
		}
		OnObjectiveComplete.Broadcast(CompletedObjective);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Objective Completed!"));	
	}
}

void UAoS_CaseManager::PartCompleted(UAoS_Part* CompletedPart)
{
	if (!CompletedPart)
	{
		return;
	}

	if (!CheckForCompletedCase())
	{
		ActiveCase->DeactivatePart(this);
		ActiveCase->ActivatePart(this);
		OnPartComplete.Broadcast(CompletedPart);
	}
}

void UAoS_CaseManager::CaseCompleted(UAoS_Case* CompletedCase)
{
	if (!CompletedCase)
	{
		return;
	}
	
	CompletedCases.AddUnique(CompletedCase);
	ActiveCase = nullptr;
}

bool UAoS_CaseManager::CheckForCompletedPart()
{
	int32 CompletedObjectives = 0;
	for (const UAoS_Objective* CurrentObjective : ActiveCase->GetActivePart()->GetAllObjectives())
	{
		if(CurrentObjective->GetObjectiveComplete())
		{
			CompletedObjectives++;
		}
	}
	if (CompletedObjectives == ActivePart->GetAllObjectives().Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString::Printf(TEXT("Current Part: %s"), *ActivePart->GetName()));

		ActivePart->SetPartComplete(true);
		PartCompleted(ActivePart);
		return true;
	}
	return false;
}

bool UAoS_CaseManager::CheckForCompletedCase()
{
	int32 CompletedParts = 0;
	for (const UAoS_Part* CurrentPart : ActiveCase->GetAllParts())
	{
		if(CurrentPart->GetPartIsComplete())
		{
			CompletedParts++;
		}
	}
	if (CompletedParts == ActiveCase->GetAllParts().Num())
	{
		ActiveCase->SetCaseComplete(true);
		CaseCompleted(ActiveCase);
		return true;
	}
	return false;
}
