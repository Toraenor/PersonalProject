// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStateAITeleport.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotifyStateAITeleport::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner))
		return;

	const UBlackboardComponent* BlackboardComp = UAIBlueprintHelperLibrary::GetBlackboard(Owner);
	if (!IsValid(BlackboardComp))
		return;
	
	const FName KeyName = FName("TargetActor");
	UObject* Value = BlackboardComp->GetValueAsObject(KeyName);
	
	if (!IsValid(Value))
		return;

	const ACharacter* Player = Cast<ACharacter>(Value);

	if (!IsValid(Player))
		return;

	const FVector StartLocation = Owner->GetActorLocation();
	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector PlayerToOwnerVec = (StartLocation - PlayerLocation).GetSafeNormal();
	const FVector TeleportLocation = PlayerLocation + PlayerToOwnerVec * Offset;
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, PlayerLocation);
	LookAtRotation.Pitch = Owner->GetActorRotation().Pitch;
	LookAtRotation.Roll = Owner->GetActorRotation().Roll;
	const FVector InterpLocation = FMath::VInterpTo(StartLocation, TeleportLocation, FrameDeltaTime, InterpSpeed);
	Owner->SetActorLocationAndRotation(InterpLocation, LookAtRotation);
	
}
