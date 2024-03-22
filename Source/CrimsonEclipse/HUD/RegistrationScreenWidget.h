// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "RegistrationScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONECLIPSE_API URegistrationScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void RegisterButtonClicked();

	UFUNCTION()
	void LoginScreenButtonClicked();

	virtual void NativeDestruct() override;

	void OnGetRegistrationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully);

protected:

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* EmailTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ConfirmPasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* RegisterButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoginScreenButton;

	class ULoginScreenWidget* LoginWidget = nullptr;

private:

	FHttpModule* Http;

public:
	void SetLoginWidget(ULoginScreenWidget* widget);
};
