// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBlockDataCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"


FTextBlockDataCustomization::FTextBlockDataCustomization()
{
}

void FTextBlockDataCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	auto NamePropertyHandle = PropertyHandle->GetChildHandle("TextContent");
	auto TablePropertyHandle = PropertyHandle->GetChildHandle("Table");
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()	
	];
// 	.ValueContent()
// 		.MinDesiredWidth(500)
// 		[
// 			SNew(SVerticalBox)
// 			+ SVerticalBox::Slot()
// 			.Padding(2.0f)
// 			.AutoHeight()
// 			[
// 				SNew(SProperty,TablePropertyHandle)
// 				.ShouldDisplayName(false)
// 			]
// 			+ SVerticalBox::Slot()
// 			.AutoHeight()
// 			[
// 				SNew(SProperty,NamePropertyHandle)
// 				.ShouldDisplayName(false)
// 			]
// 		];
}

void FTextBlockDataCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	auto NamePropertyHandle = PropertyHandle->GetChildHandle("TextContent");
	auto TablePropertyHandle = PropertyHandle->GetChildHandle("Table");

	ChildBuilder.AddCustomRow(FText::FromString(TEXT("ChildRow")))
	.NameContent()
	[
		TablePropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(500)
	[
		SNew(SProperty,TablePropertyHandle)
		.ShouldDisplayName(false)
	];

	ChildBuilder.AddCustomRow(FText::FromString(TEXT("ChildRow")))
	.NameContent()
	[
		NamePropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(500)
	[
		SNew(SProperty,NamePropertyHandle)
		.ShouldDisplayName(false)
	];

}
