#pragma once

#include "EngineMinimal.h"
#include "CoreMinimal.h"
#include <Stats/Stats2.h>
#include <Stats/Stats.h>
#include "Stats/StatsMisc.h"


// Level Macro 
#if UE_BUILD_SHIPPING
#define DEBUG_LOG 0
#define LEE_PROFILE_LEVEL 0
#else
#define DEBUG_LOG 0
#define LEE_PROFILE_LEVEL 1
#endif //!UE_BUILD_SHIPPING


DEFINE_LOG_CATEGORY_STATIC(LeeXRMacro, Log, All);

#define TAU 6.2831855

extern int64 allocatedSize;

// Memory Stat
#define REGISTER_LEESTAT(StatName,GroupId,StatId)	\
DECLARE_STATS_GROUP(TEXT(StatName), GroupId,StatId) \
DECLARE_CYCLE_STAT(TEXT(StatName), STAT_constant, GroupId);

//Global Stat Group
#if LEE_PROFILE_LEVEL
REGISTER_LEESTAT("LeeXR_Profiling", STATGROUP_ICTUMV, STATCAT_ICTUMV);
#endif

//Stat Group
#if STATS 

#define LEE_SCOPE_CYCLE_COUNTER(STATNAME)									\
	LEE_DECLARE_STAT(LEE_##STATNAME,STATNAME);								\
	static DEFINE_STAT(STATNAME)											\
	FScopeCycleCounter CycleCount_ICTUMV_##STATNAME(GET_STATID(STATNAME));	

#define LEE_DECLARE_STAT(Description, StatName)																					\
DECLARE_STAT(TEXT(#Description), StatName, STATGROUP_ICTUMV, EStatDataType::ST_int64,												\
EStatFlags::CycleStat | EStatFlags::ClearEveryFrame ,																			\
FPlatformMemory::EMemoryCounterRegion::MCR_Invalid);																																		
#else																															
#define LEE_SCOPE_CYCLE_COUNTER(STATNAME)
#endif

#if WITH_EDITOR
#define LEE_CHECK(expr) checkSlow((expr))
#else
#define LEE_CHECK(expr) check((expr)) 
#endif

#define LEE_LOG(Category,Verbosity, Format, ...) UE_LOG(Category, Verbosity, TEXT(Format), ##__VA_ARGS__)

#define LeeScreenLog(Format,Color, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, Color, FString::Printf(TEXT(Format), ##__VA_ARGS__),true,FVector2D(4,4))

#if WITH_EDITOR
#define PROPERTYCHANGED(Class,inProperty) PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(Class, inProperty)
#endif

extern float  MemoriesSize;

DECLARE_MEMORY_STAT_POOL_EXTERN(TEXT("ICTUMV_TotalMemories"), STAT_ICTUMV_TotalMemories, STATGROUP_ICTUMV, FPlatformMemory::MCR_Physical, );

// Memory Stat

#define ADDMEMORYSTAT(inObject,ClassStat)	\
	float ResourceSize = inObject->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal);	\
	INC_MEMORY_STAT_BY(ClassStat, ResourceSize);\
	MemoriesSize += ResourceSize;	\
	INC_MEMORY_STAT_BY(STAT_ICTUMV_TotalMemories, MemoriesSize);

#define REMOVEMEMORYSTAT(inObject,ClassStat)	\
	float ResourceSize = inObject->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal);	\
	DEC_MEMORY_STAT_BY(ClassStat, ResourceSize);\
	MemoriesSize -= ResourceSize;	\
	DEC_MEMORY_STAT_BY(STAT_ICTUMV_TotalMemories, ResourceSize);