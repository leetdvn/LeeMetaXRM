

#pragma once

#include "Blueprint/WidgetTree.h"
#include "Internationalization/StringTable.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

/**
 * 
 */

namespace LeeXRWidgetUltils
{
	/// <summary>
	/// Find a widget by name 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="UUserWidget"></param>
	/// <param name="FName"></param>
	/// <returns></returns>
	template<class T>
	FORCEINLINE T* LeeXRFindWidget(UUserWidget* inBaseWG,const FName& Name) {
		UWidget* LocalParent = inBaseWG->GetRootWidget()->GetOuter();

		if (auto TreeWG = Cast<UWidgetTree>(LocalParent))
		{
			if (auto Widget = TreeWG->FindWidget<T>(Name))
			{
				return Widget;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Get all buttons
	///Rule Button Name must match the key name
	///Rule 1: Find all buttons
	///Rule 2: Find all keys from the string table inTable
	///Rule 3: Set the display name of each button name to  the Keys
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="inBaseWG"></param>
	/// <returns></returns>
	template<class T>
	FORCEINLINE TArray<T*> LeeXRFindWidgetsOfClass(UUserWidget* inBaseWG) {

		TArray<UWidget*> Widgets{};
		TArray<T*> FoundWidgets{};

		UObject* LocalParent = inBaseWG->GetRootWidget()->GetOuter();

		if (auto RootWg = Cast<UWidgetTree>(LocalParent))
		{
			RootWg->GetAllWidgets(Widgets);

			for (auto Widget : Widgets)
			{
				if (auto FoundWidget = Cast<T>(Widget))
				{
					FoundWidgets.AddUnique(FoundWidget);
				}
			}
		}
		return FoundWidgets;
	}


}