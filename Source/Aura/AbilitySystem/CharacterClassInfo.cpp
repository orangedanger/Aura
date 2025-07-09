// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterDefaultInfo(ECharacterClass CharacterClass)
{
	return CharcaterClassInfo.FindChecked(CharacterClass);
}
