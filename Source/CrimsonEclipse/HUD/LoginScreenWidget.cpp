// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginScreenWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

bool ULoginScreenWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	Http = &FHttpModule::Get();

	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &ULoginScreenWidget::LoginButtonClicked);
	}

	return true;
}

void ULoginScreenWidget::LoginButtonClicked()
{

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red,
			FString(TEXT("Button")));
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	TSharedPtr<FJsonObject> loginData = MakeShareable(new FJsonObject);

	FString Login = LoginTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();

	loginData->SetStringField("email", Login);
	loginData->SetStringField("password", Password);

	FString LoginRequestStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&LoginRequestStr);
	FJsonSerializer::Serialize(loginData.ToSharedRef(), Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &ULoginScreenWidget::OnGetLoginResponse);
	Request->SetURL("http://192.168.1.182:5199/api/User/login");
	Request->SetVerb("POST");

	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(LoginRequestStr);
	Request->ProcessRequest();
}

void ULoginScreenWidget::OnGetLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully)
{
	if (Response)
	{

		if (Response->GetResponseCode() == 200)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 15.f, FColor::Emerald,
					FString(TEXT("Welcome")));
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 15.f, FColor::Emerald,
					FString(TEXT("No responce")));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 15.f, FColor::Emerald,
				FString(TEXT("No connect")));
		}
	}
}
