// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    bWantsBeginPlay = true;
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
    Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

// Look for attached PhysicsHandle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing PhysicsHandle Component"), *GetOwner()->GetName())
    }
}

// Look for attached Input Component (appears only at runtime)
void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing Input Component"), *GetOwner()->GetName())
    }
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
    /// Setup query parameters
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

    /// Ray-cast out to reach distance
    FHitResult LineTraceResults;
    GetWorld()->LineTraceSingleByObjectType(
        LineTraceResults,
        GetReachLineStart(),
        GetReachLineEnd(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );

    return LineTraceResults;
}

FVector UGrabber::GetReachLineStart()
{
    FVector PlayerLoc;
    FRotator PlayerRot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLoc, PlayerRot);

    return PlayerLoc;
}

FVector UGrabber::GetReachLineEnd()
{
    FVector PlayerLoc;
    FRotator PlayerRot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLoc, PlayerRot);

    return PlayerLoc + PlayerRot.Vector() * Reach;
}

// Try and reach any actors with physics body collision channel set and if we hit attach a physics handle
void UGrabber::Grab()
{
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
    auto ActorHit = HitResult.GetActor();

    if (ActorHit)
    {
        if (!PhysicsHandle) { return; }
        PhysicsHandle->GrabComponent(
            ComponentToGrab,
            NAME_None, // No bones needed
            ComponentToGrab->GetOwner()->GetActorLocation(),
            true // Allow Rotation
        );
    }
}

// Release Physics handle
void UGrabber::Release()
{
    if (!PhysicsHandle) { return; }
    PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!PhysicsHandle) { return; }
    /// If the physics handle is attached
    if (PhysicsHandle->GrabbedComponent)
    {
        GetReachLineEnd();
        // Move the object that we're holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }

}

