// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Capture.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"

USAction_Capture::USAction_Capture()
{
	ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery2);
	ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	
}

void USAction_Capture::FindTargetLocation()
{

	FRotator R;
	FVector V;
	if (Character) {
		Character->GetActorEyesViewPoint(StartLocation, R);
		EndLocation = StartLocation + (R.Vector() * 2000);
		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, 20.0f, ObjectType, false, ActorsToIgnore, EDrawDebugTrace::Type::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);
		//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f);
		StartLocation = Character->GetMesh()->GetSocketLocation("Cast_L") + (R.Vector() * 150);
		if (HitResult.bBlockingHit) {
			EndLocation = HitResult.Location;
			Actor = HitResult.GetActor();
			V = Actor->GetActorLocation();
			FRichCurve RichCurve[3];
			RichCurve[0].AddKey(0.0f, StartLocation.X);
			RichCurve[0].AddKey(0.5f, EndLocation.X);
			RichCurve[1].AddKey(0.0f, StartLocation.Y);
			RichCurve[1].AddKey(0.5f, EndLocation.Y);
			FVector vect = Character->GetActorLocation();

			if (StartLocation.Z < vect.Z || EndLocation.Z < vect.Z) 
			{
				RichCurve[2].AddKey(0.0f, vect.Z);
				RichCurve[2].AddKey(0.5f, vect.Z);
			}
			else 
			{
				RichCurve[2].AddKey(0.0f, StartLocation.Z);
				RichCurve[2].AddKey(0.5f, EndLocation.Z);
			}
			

			CurveVector->FloatCurves[0] = RichCurve[0];
			CurveVector->FloatCurves[1] = RichCurve[1];
			CurveVector->FloatCurves[2] = RichCurve[2];
		}
		V = EndLocation - StartLocation;

		float X = V.X * V.X;
		float Y = V.Y * V.Y;
		float Z = V.Z * V.Z;
		float S = FMath::Sqrt(X + Y);
		Yaw = 180 / 3.1415926 * FMath::Acos(V.X / S);
		float S1 = FMath::Sqrt((S * S) + Z);
		Pitch = 180 / 3.1415926 * FMath::Acos(S / S1);
		//if (Character->GetActorRotation().Yaw < 0)
		//	Yaw *= -1;
		if((StartLocation.Y-EndLocation.Y)>0)
			Yaw *= -1;
		if (V.Z < 0)
			Pitch *= -1;

		//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, FString::Printf(TEXT("R1:%f      R2:%f"), Character->GetActorRotation().Yaw,Yaw));
		//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, FString::Printf(TEXT("Start:%f      End:%f"), StartLocation.Length(), EndLocation.Length()));
		//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, Character->GetActorRotation().Vector().ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, EndLocation.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("%f"), Pitch));
		//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), EndLocation, 100, 12, FLinearColor::White, 0.5f, 2.0f);

	}
}



void USAction_Capture::OnTimelineUpdate(FVector vector)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, vector.ToString());
	if (!b) {
		//if (Actor->IsA(APawn::StaticClass())) {
			Character->SetActorLocation(vector);
		//}
	}

	if (StringA) {
		float D = FVector::Dist(vector,StartLocation) / 2;
		StringA->SetActorScale3D(FVector(D, 2.0f, 2.0f));
		//StringA->SetActorLocation(vector);
	}
}

void USAction_Capture::OnTimelineFinished()
{
	if (b&&HitResult.bBlockingHit) {
		Timeline->PlayFromStart();
		b = false;
	}
	else {
		StringA->Destroy();
	}
}

void USAction_Capture::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	////初始化时间轴
	Character = Cast<ACharacter>(Instigator);
	if (Character) {
		if (Timeline == NULL) {
			Timeline = Character->GetComponentByClass<UTimelineComponent>();
			if (Timeline) {
				CurveVector = NewObject<UCurveVector>();
				OnTimelineVector.BindUFunction(this, TEXT("OnTimelineUpdate"));
			    Timeline->AddInterpVector(CurveVector, OnTimelineVector);
				Timeline->SetTimelineFinishedFunc(FSimpleDelegate::CreateUObject(this,&USAction_Capture::OnTimelineFinished));
				Timeline->SetLooping(false);
				Timeline->SetTimelineLength(1.0f);
				Timeline->SetPlayRate(1);
		     }
		
		}
		Character->GetWorldTimerManager().SetTimer(TimeHandle, this, &USAction_Capture::FindTargetLocation, 0.5f, true);
		//GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &USAction_Capture::FindTargetLocation, 0.5f, true);
	}
	
}

void USAction_Capture::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	Character = Cast<ACharacter>(Instigator);
	if (Character) {
		Character->GetWorldTimerManager().ClearTimer(TimeHandle);
		if (Timeline) {
			if (!Timeline->IsPlaying())
			{
				//float D = FVector::Dist(EndLocation,StartLocation)/StringA->GetActorScale().X;
				StringA = GetWorld()->SpawnActor<AActor>(StringActor, FTransform(FRotator(Pitch, Yaw, 0), StartLocation, FVector(0, 0, 0)));
				StringA->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
				b = true;
				Timeline->PlayFromStart();
			}
		}
	}
}
