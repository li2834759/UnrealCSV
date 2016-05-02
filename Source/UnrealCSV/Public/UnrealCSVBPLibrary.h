// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "UnrealCSVBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UUnrealCSVBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	static void GetCSVRowDataAndProperty(UDataTable* Table, FName InRowName, FName InColumnName, uint8*& OutRowData, UProperty*& OutProperty);

	UFUNCTION(BlueprintCallable, Category = "UnrealCSVTesting")
	static void GetCSVTableString(UDataTable* DataTable, FString& OutResult);

	UFUNCTION(BlueprintCallable, Category = "UnrealCSVTesting")
	static void GetCSVRowNames(UDataTable* Table, TArray<FName>& OutRowNames);

	/** Get a Row from a DataTable given a RowName */
	UFUNCTION(BlueprintCallable, Category = "UnrealCSVTesting")
	static int32 GetIntValueFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName);

	/** Get a Row from a DataTable given a RowName */
	UFUNCTION(BlueprintCallable, Category = "UnrealCSVTesting")
	static float GetFloatValueFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName);

	/** Get a Row from a DataTable given a RowName */
	UFUNCTION(BlueprintCallable, Category = "UnrealCSVTesting")
	static FString GetStringValueFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName);

	/** Get a Row from a DataTable given a RowName */
	UFUNCTION(BlueprintCallable, Category = "UnrealCSVTesting")
	static TAssetPtr<UTexture> GetTextureFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName);
};
