// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <map>
#include <string>
#include "GameFramework/Actor.h"
#include "Door.generated.h"

using namespace std;

struct MembershipFunction
{
	vector<float> desc;

	float GetLast() { if (desc.empty()) return -1.f; return desc[desc.size() - 1]; }
};

typedef vector<MembershipFunction> VMEMBERSHIPFUNCTION;
typedef map<string, VMEMBERSHIPFUNCTION> MAPVMEMBERSHIPFUNCTION;

class condition
{
public:
	float co2;
	float temprature;
	float humidity;

	condition(float c, float t, float h) : co2(c), temprature(t), humidity(h)
	{
	}

	condition operator+(const condition& rhs)
	{
		return condition(co2 + rhs.co2, temprature + rhs.temprature, humidity + rhs.humidity);
	}

	condition operator*(const int x)
	{
		return condition(co2 * static_cast<float>(x), temprature * static_cast<float>(x), humidity  * static_cast<float>(x));
	}
};

class TSFuzzySystem
{
public:
	TSFuzzySystem();
	~TSFuzzySystem();

	void Register(string name, MembershipFunction membershipfunc);

private:
	MAPVMEMBERSHIPFUNCTION systemDesc;
};

float GetFiringPoint(const MembershipFunction& memfunc, float value);

UCLASS()
class MAT362_PRJ1_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Environment)
	float CO2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Environment)
	float Humidity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Environment)
	float Temprature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Environment)
	float OutsideHumidity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Environment)
	float OutsideTemprature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Environment)
	float doorAngle;

	MembershipFunction tempCold, tempNormal, tempHot;
	MembershipFunction humidityLow, humidityNormal, humidityHigh;
	MembershipFunction CO2Normal, CO2NotGood, CO2Danger;
	MembershipFunction Close, Open;
};
