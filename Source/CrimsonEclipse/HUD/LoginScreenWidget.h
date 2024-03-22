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

	UFUNCTION(BlueprintCallable)
	void RegistrationButtonClicked();

	virtual void NativeDestruct() override;

	void OnGetLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully);

	virtual void BeginDestroy() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResponseText;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* LoginTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RegistrationButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetForRegister;

private:

	FHttpModule* Http;
	class URegistrationScreenWidget* RegistrationWidget;
};
