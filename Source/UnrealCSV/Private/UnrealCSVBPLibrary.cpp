// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealCSVPrivatePCH.h"
#include "UnrealCSVBPLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogUnrealCSV, Log, All);

UUnrealCSVBPLibrary::UUnrealCSVBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UUnrealCSVBPLibrary::GetCSVTableString(UDataTable* DataTable, FString& OutResult)
{
	if (!DataTable->RowStruct)
	{
		return;
	}

	// Write the header (column titles)
	OutResult = TEXT("---");
	for (TFieldIterator<UProperty> It(DataTable->RowStruct); It; ++It)
	{
		UProperty* BaseProp = *It;
		check(BaseProp);

		OutResult += TEXT(",");
		OutResult += BaseProp->GetName();
	}
	OutResult += TEXT("\n");

	// Write each row
	for (auto RowIt = DataTable->RowMap.CreateConstIterator(); RowIt; ++RowIt)
	{
		FName RowName = RowIt.Key();
		OutResult += RowName.ToString();

		uint8* RowData = RowIt.Value();
		if (!DataTable->RowStruct)
		{
			return;
		}

		for (TFieldIterator<UProperty> It(DataTable->RowStruct); It; ++It)
		{
			UProperty* BaseProp = *It;
			check(BaseProp);

			const void* Data = BaseProp->ContainerPtrToValuePtr<void>(RowData, 0);

			OutResult += TEXT(",");

			const FString PropertyValue = DataTableUtils::GetPropertyValueAsString(BaseProp, (uint8*)RowData);
			OutResult += TEXT("\"");
			OutResult += PropertyValue.Replace(TEXT("\""), TEXT("\"\""));
			OutResult += TEXT("\"");
		}

		OutResult += TEXT("\n");
	}
}

void UUnrealCSVBPLibrary::GetCSVRowNames(UDataTable* Table, TArray<FName>& OutRowNames)
{
	if (Table)
	{
		OutRowNames = Table->GetRowNames();
	}
	else
	{
		OutRowNames.Empty();
	}
}

void UUnrealCSVBPLibrary::GetCSVRowDataAndProperty(UDataTable* Table, FName InRowName, FName InColumnName, uint8*& OutRowData, UProperty*& OutProperty)
{
	if (Table == nullptr)
	{
		return;
	}

	// First build array of properties
	for (TFieldIterator<UProperty> It(Table->RowStruct); It; ++It)
	{
		UProperty* Prop = *It;
		check(Prop != NULL);
		FName PropName = FName(*Prop->GetName());
		if (!PropName.Compare(InColumnName))
		{
			UProperty* ColumnProp = Prop;

			// Now iterate over rows
			for (auto RowIt = Table->RowMap.CreateConstIterator(); RowIt; ++RowIt)
			{
				FName RowName = RowIt.Key();
				if (!RowName.Compare(InRowName))
				{
					OutRowData = RowIt.Value();
					OutProperty = ColumnProp;
				}
			}
		}
	}
}

FString UUnrealCSVBPLibrary::GetStringValueFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName)
{
	uint8* RowData = nullptr;
	UProperty* ColumnProp = nullptr;
	
	GetCSVRowDataAndProperty(Table, InRowName, InColumnName, RowData, ColumnProp);
	if (RowData != nullptr && ColumnProp != nullptr)
	{
		return DataTableUtils::GetPropertyValueAsString(ColumnProp, RowData);
	}

	return TEXT("");
}

int32 UUnrealCSVBPLibrary::GetIntValueFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName)
{
	FString ResultStr = GetStringValueFromDataTable(Table, InRowName, InColumnName);
	return ResultStr.IsNumeric() ? FCString::Atoi(*ResultStr) : -1;
}

float UUnrealCSVBPLibrary::GetFloatValueFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName)
{
	FString ResultStr = GetStringValueFromDataTable(Table, InRowName, InColumnName);
	return ResultStr.IsNumeric() ? FCString::Atof(*ResultStr) : -1.f;
}

TAssetPtr<UTexture> UUnrealCSVBPLibrary::GetTextureFromDataTable(UDataTable* Table, FName InRowName, FName InColumnName)
{
	uint8* RowData = nullptr;
	UProperty* ColumnProp = nullptr;

	GetCSVRowDataAndProperty(Table, InRowName, InColumnName, RowData, ColumnProp);
	if (RowData != nullptr && ColumnProp != nullptr)
	{
		void* PropertyValue = ColumnProp->ContainerPtrToValuePtr<void>(RowData, 0);
		FAssetPtr& AssetPtr = *(FAssetPtr*)PropertyValue;

		FStringAssetReference ID;
		UObject *Object = AssetPtr.Get();
		if (Object)
		{
			// Use object in case name has changed.
			ID = FStringAssetReference(Object);
		}
		else
		{
			ID = AssetPtr.GetUniqueID();
		}

		FStreamableManager StreamableManager;
		UTexture* TextureAsset = Cast<UTexture>(StreamableManager.SynchronousLoad(ID));
		TAssetPtr<UTexture> TextureAssetPtr = TextureAsset;
		return TextureAssetPtr;
	}

	return nullptr;
}

