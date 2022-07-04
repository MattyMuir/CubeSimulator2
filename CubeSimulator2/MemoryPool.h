#pragma once
#include <vector>

template <typename T>
class MemoryPool
{
public:
	MemoryPool(int batchSize_)
		: batchSize(batchSize_) {}

	~MemoryPool()
	{
		for (char* batch : batches)
			delete[] batch;
	}

	template <typename... Args>
	T* Emplace(Args&&... args)
	{
		if (batches.size() == 0)
		{
			batches.push_back(new char[batchSize * sizeof(T)]);
		}

		if (currentBatchIndex == batchSize)
		{
			// Allocate new batch
			char* newBatch = new char[batchSize * sizeof(T)];
			batches.push_back(newBatch);
			currentBatch++;
			currentBatchIndex = 0;
		}

		T* newObj = new (batches[currentBatch] + currentBatchIndex * sizeof(T)) T(std::forward<Args>(args)...);
		currentBatchIndex++;
		return newObj;
	}

	void Clear()
	{
		for (char* batch : batches)
			delete[] batch;
		batches.clear();
		currentBatch = 0;
		currentBatchIndex = 0;
	}

private:
	size_t batchSize;
	int currentBatch = 0;
	int currentBatchIndex = 0;
	std::vector<char*> batches;
};