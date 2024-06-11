
#pragma once

#include "CoreMinimal.h"

class AMeshGenerator;
class ASSIGNMENTS_API FAsyncMeshGenerationTask : public FNonAbandonableTask
{
public:
	FAsyncMeshGenerationTask(AMeshGenerator* InScatterActor);
	FVector GetRandomPointInSphere(const FVector& Origin, float Radius);

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncScatterTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:


	TWeakObjectPtr<AMeshGenerator> ScatterActor;

	TArray<FVector> MeshLocationArray;
};
