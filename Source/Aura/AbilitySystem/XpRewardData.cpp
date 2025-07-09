// Fill out your copyright notice in the Description page of Project Settings.


#include "XpRewardData.h"

int32 UXpRewardData::FindLevelAtXp(int32 inXp)
{
	if (XpRewardDataArray.Num() <= 1) return 1;

	for (int level = 1; level < XpRewardDataArray.Num() ; level++)
	{
		if (inXp < XpRewardDataArray[level].RequiredXp)
		{
			return level;
		}
	}
	return XpRewardDataArray.Num()-1;
}
