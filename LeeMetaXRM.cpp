#include "LeeMetaXRM.h"

#define LOCTEXT_NAMESPACE "FLeeMetaXRM"

void FLeeMetaXRMModule::StartupModule()
{
	FString ShaderDir = FPaths::ProjectPluginsDir() + "LeeXRContent/Source/LeeMetaXRM/Shaders";

	FString abc = FPaths::ConvertRelativePathToFull(ShaderDir);
	UE_LOG(LogTemp, Warning, TEXT("Shader Path : %s"), *abc);
	AddShaderSourceDirectoryMapping(TEXT("/LeeMetaXRM"), ShaderDir);
}

void FLeeMetaXRMModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLeeMetaXRMModule, LeeMetaXRM);