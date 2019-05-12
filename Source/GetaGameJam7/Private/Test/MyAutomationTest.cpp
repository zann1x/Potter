// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MyAutomationTest, "MyTestGroup.MyAutomationTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MyAutomationTest::RunTest(const FString& Parameters)
{
	return false;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MyAutomationTestTwo, "MyTestGroup.MyAutomationTestTwo", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MyAutomationTestTwo::RunTest(const FString& Parameters)
{
	return true;
}
