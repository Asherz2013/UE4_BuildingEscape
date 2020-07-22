// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UGrabber();

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

    // How far can the player reach in CM
    float Reach = 100.f;

    UPhysicsHandleComponent* PhysicsHandle = nullptr;

    UInputComponent* InputComponent = nullptr;

    // Ray-cast and grab whats in reach
    void Grab();

    void Release();

    // Find attached Physics
    void FindPhysicsHandleComponent();

    // Setup (assumed) attached Input Component
    void SetupInputComponent();

    // Return hit for first physics body in reach
    const FHitResult GetFirstPhysicsBodyInReach();

    // Returns current start of Reach line
    FVector GetReachLineStart();

    // Returns current end of Reach line
    FVector GetReachLineEnd();
};
