// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MyAutomationTest, "MyTestGroup.MyAutomationTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MyAutomationTest::RunTest(const FString& Parameters)
{

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MySampleTest, "MyTestGroup.MySampleTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter)

bool MySampleTest::RunTest(const FString& Parameters)
{
	TestTrue("Load the game map", AutomationOpenMap("/Game/Maps/Map0"));
	TestTrue("Load the main menu map", AutomationOpenMap("/Game/Maps/Map_MainMenu"));

	return true;
}
