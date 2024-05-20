// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignments_1_2.h"

#define LOCTEXT_NAMESPACE "FAssignments_1_2Module"

void FAssignments_1_2Module::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAssignments_1_2Module::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssignments_1_2Module, Assignments_1_2)