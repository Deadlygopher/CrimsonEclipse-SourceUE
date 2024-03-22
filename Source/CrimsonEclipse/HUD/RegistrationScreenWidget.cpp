// Fill out your copyright notice in the Description page of Project Settings.


#include "RegistrationScreenWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "LoginScreenWidget.h"

bool URegistrationScreenWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	Http = &FHttpModule::Get();

	if (RegisterButton)
	{
		RegisterButton->OnClicked.AddDynamic(this, &URegistrationScreenWidget::RegisterButtonClicked);
	}
	if (LoginScreenButton)
	{
		LoginScreenButton->OnClicked.AddDynamic(this, &URegistrationScreenWidget::LoginScreenButtonClicked);
	}

	return true;
}

void URegistrationScreenWidget::RegisterButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red,
			FString(TEXT("Register Button Pressed")));
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	TSharedPtr<FJsonObject> registrationData = MakeShareable(new FJsonObject);

	FString Email = EmailTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();
	FString ConfirmPassword = ConfirmPasswordTextBox->GetText().ToString();

	registrationData->SetStringField("email", Email);
	registrationData->SetStringField("password", Password);
	registrationData->SetStringField("confirmPassword", ConfirmPassword);

	FString RegistrationRequestStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RegistrationRequestStr);
	FJsonSerializer::Serialize(registrationData.ToSharedRef(), Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &URegistrationScreenWidget::OnGetRegistrationResponse);
	Request->SetURL("https://021b-178-214-250-28.ngrok-free.app/api/User/register");
	Request->SetVerb("POST");

	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RegistrationRequestStr);
	Request->ProcessRequest();
}

void URegistrationScreenWidget::OnGetRegistrationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully)
{
	if (Response)
	{
		FString Resp = Response->GetContentAsString();
		if (Response->GetResponseCode() == 200)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 5.f, FColor::Green,
					FString(Resp));
			}
		}
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 5.f, FColor::Blue,
				FString(Resp));
		}
	}
}

void URegistrationScreenWidget::LoginScreenButtonClicked()
{
	if (LoginWidget)
	{
		LoginWidget->SetVisibility(ESlateVisibility::Visible);
		RemoveFromParent();
	}
}

void URegistrationScreenWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

void URegistrationScreenWidget::SetLoginWidget(ULoginScreenWidget* widget)
{
	if (LoginWidget!=nullptr)
	{
		return;
	}
	LoginWidget = widget;
}
