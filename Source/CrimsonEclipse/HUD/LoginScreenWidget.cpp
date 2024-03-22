// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginScreenWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RegistrationScreenWidget.h"


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
	if (RegistrationButton)
	{
		RegistrationButton->OnClicked.AddDynamic(this, &ULoginScreenWidget::RegistrationButtonClicked);
	}
	RegistrationWidget = CreateWidget<URegistrationScreenWidget>(GetWorld(), WidgetForRegister);

	return true;
}

void ULoginScreenWidget::LoginButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red,
			FString(TEXT("Login Button Pressed")));
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
	Request->SetURL("https://021b-178-214-250-28.ngrok-free.app/api/User/login");
	Request->SetVerb("POST");

	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(LoginRequestStr);
	Request->ProcessRequest();
}

void ULoginScreenWidget::RegistrationButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red,
			FString(TEXT("RegistrationButtonPressed")));
	}
	if (RegistrationWidget)
	{
		RegistrationWidget->AddToViewport();
		RegistrationWidget->SetLoginWidget(this);
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULoginScreenWidget::OnGetLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully)
{
	if (Response)
	{
		FString Resp = Response->GetContentAsString();
		ResponseText->SetText(FText::FromString(Resp));
		if (Response->GetResponseCode() == 200)
		{
			GetWorld()->ServerTravel("/Game/TopDownCPP/Maps/TopDownExampleMap");
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

void ULoginScreenWidget::BeginDestroy()
{
	Super::BeginDestroy();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red,
			FString(TEXT("Widget Destroyed !")));
	}
}

void ULoginScreenWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}