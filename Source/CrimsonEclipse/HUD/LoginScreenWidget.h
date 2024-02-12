// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "LoginScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONECLIPSE_API ULoginScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;
	
	UFUNCTION(BlueprintCallable)
	void LoginButtonClicked();

	void OnGetLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully);

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* LoginTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RegistrationButton;

private:

	FHttpModule* Http;
};
