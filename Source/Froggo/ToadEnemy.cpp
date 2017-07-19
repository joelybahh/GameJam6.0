// Fill out your copyright notice in the Description page of Project Settings.

#include "Froggo.h"
#include "ToadEnemy.h"


// Sets default values
AToadEnemy::AToadEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AToadEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToadEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


