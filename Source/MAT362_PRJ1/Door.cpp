// Fill out your copyright notice in the Description page of Project Settings.

#include "MAT362_PRJ1.h"
#include "Door.h"
#include <algorithm>


// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	condition initialCondition(100.f, 20.f, 30.f);
	condition increaseRate(1, 1, 1);
	
	/* temprature */
	tempCold.desc.push_back(16.f);
	tempCold.desc.push_back(16.f);
	tempCold.desc.push_back(23.f);

	tempNormal.desc.push_back(16.f);
	tempNormal.desc.push_back(23.f);
	tempNormal.desc.push_back(30.f);

	tempHot.desc.push_back(23.f);
	tempHot.desc.push_back(30.f);
	tempHot.desc.push_back(30.f);


	/* Humidity */
	humidityLow.desc.push_back(10.f);
	humidityLow.desc.push_back(10.f);
	humidityLow.desc.push_back(30.f);

	humidityNormal.desc.push_back(10.f);
	humidityNormal.desc.push_back(30.f);
	humidityNormal.desc.push_back(50.f);

	humidityHigh.desc.push_back(30.f);
	humidityHigh.desc.push_back(50.f);
	humidityHigh.desc.push_back(50.f);


	/* CO2 */
	CO2Normal.desc.push_back(700.f);
	CO2Normal.desc.push_back(700.f);
	CO2Normal.desc.push_back(1000.f);

	CO2NotGood.desc.push_back(700.f);
	CO2NotGood.desc.push_back(2000.f);
	CO2NotGood.desc.push_back(3000.f);

	CO2Danger.desc.push_back(2000.f);
	CO2Danger.desc.push_back(3000.f);
	CO2Danger.desc.push_back(3000.f);


	/* Fuzzy controller */
	Close.desc.push_back(0);
	Close.desc.push_back(0);
	Close.desc.push_back(90.f);

	Open.desc.push_back(0);
	Open.desc.push_back(90.f);
	Open.desc.push_back(90.f);
}

int NominalToScale(MembershipFunction functions[], int offset, float value)
{
	float max = -FLT_MAX;
	int scale = 0;

	for (int i = offset; i < offset + 3; ++i)
	{
		float cal = GetFiringPoint(functions[i], value);
		if (max < cal)
		{
			max = cal;
			scale = i - offset+1;
		}
	}
	return scale;
}

float FindMinFiringPoint(MembershipFunction functions[], int offset, float values[])
{
	float min = FLT_MAX;
	int numOfFuzzySets = sizeof(values) / sizeof(float);

	for (int i = 0; i < numOfFuzzySets; ++i) // iterate from A to number of fuzzy sets
	{
		float cal = GetFiringPoint(functions[offset + (i/2)*3], values[i]);
		if (min > cal)
			min = cal;
	}
	return min;
}

bool IsOpen(int inTemp, int outTemp, int inHumi, int outHumi)
{
	int temp = inTemp != 2 ? abs(outTemp - inTemp) : 0;
	int humi = (outHumi - inHumi) * 2;

	return temp + humi >= 2;
}

// Called every frame
void ADoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	std::vector<float> rules;

	MembershipFunction functions[] =
	{
		tempCold, tempNormal, tempHot,
		humidityLow, humidityNormal, humidityHigh,
		CO2Normal, CO2NotGood, CO2Danger
	};

	float values[] =
	{
		Temprature, OutsideTemprature, Humidity, OutsideHumidity, CO2
	};

	int inTemp, outTemp, inHumi, outHumi, inCO2, outCO2;
	inTemp = NominalToScale(functions, 0, Temprature);
	outTemp = NominalToScale(functions, 0, Temprature);
	inHumi = NominalToScale(functions, 3, Humidity);
	outHumi = NominalToScale(functions, 3, Humidity);
	inCO2 = NominalToScale(functions, 6, CO2);
	outCO2 = NominalToScale(functions, 6, CO2);

	bool isOpen = IsOpen(inTemp, outTemp, inHumi, outHumi);


	int numMemfuncs = 3;
	int numFuzzySets = 4;

	std::vector<float> alpha;
	for(unsigned i = 0; i < numMemfuncs; ++i)
		alpha.push_back(FindMinFiringPoint(functions, i, values)); // calcuate Ai ^ Bi ^ ...

	std::vector<float> ai, bi, ci, di;
	for (unsigned i = 0; i < numMemfuncs; ++i)
	{
		float a = functions[i].desc.
	}
}

TSFuzzySystem::TSFuzzySystem()
{
}


TSFuzzySystem::~TSFuzzySystem()
{
}

void TSFuzzySystem::Register(string name, MembershipFunction membershipfunc)
{
	MAPVMEMBERSHIPFUNCTION::iterator finder = systemDesc.find(name);
	if (finder != systemDesc.end())
		finder->second.push_back(move(membershipfunc));
	else
	{
		VMEMBERSHIPFUNCTION vMemfunc;
		vMemfunc.push_back(move(membershipfunc));
		systemDesc.insert(MAPVMEMBERSHIPFUNCTION::value_type(name, vMemfunc));
	}
}

float GetFiringPoint(const MembershipFunction& memfunc, float value)
{
	if (memfunc.desc.size() < 3)
		return -1.f;

	if (((memfunc.desc[0] == memfunc.desc[1]) && value <= memfunc.desc[0]) ||
		((memfunc.desc[memfunc.desc.size() - 2] == memfunc.desc[memfunc.desc.size() - 1]) && value >= memfunc.desc[memfunc.desc.size() - 1]))
		return 1.f;
	else if (value <= memfunc.desc[0] || memfunc.desc[memfunc.desc.size() - 1] <= value)
		return 0.f;

	for (unsigned i = 0; i < memfunc.desc.size() - 1; ++i)
	{
		if (value >= memfunc.desc[i] && value <= memfunc.desc[i + 1])
		{
			// if this is increasing
			if (i == 0)
			{
				float top = value - memfunc.desc[i];
				float bot = memfunc.desc[i + 1] - memfunc.desc[i];
				return top / bot;
			}
			// if this is decreasing
			else if (i + 1 == memfunc.desc.size() - 1)
			{
				float top = memfunc.desc[i + 1] - value;
				float bot = memfunc.desc[i + 1] - memfunc.desc[i];
				return top / bot;
			}
			else
				return 1.f;
		}
	}

	return -1.f;
}