#include <cuda_runtime.h>
#include <mutex>
#include <cassert>
#include "Hack.h"

void hackCUDAMalloc(void **p, size_t size)
{
  static std::mutex mutex;
  mutex.lock();

  size_t dev_free = 0, dev_total = 0;
  cudaMemGetInfo(&dev_free, &dev_total);
  DbgPrintf("Dev free %.1f MB\n", (float)dev_free/1024/1024);

  *p = 0;
  
  if(dev_free > size + 1024 * 1024 * 1024)
  {
    DbgPrintf("Will be malloc on Grapic Card %.1f MB\n", (float)size/1024/1024);
    cudaMalloc(p, size);
    if(*p == 0)
    {
      DbgPrintf("Malloc on Grapic Card failed, try cudaMallocManaged\n");
      cudaMallocManaged(p, size);
      assert(*p != 0);
    }
  }
  else
  {
    DbgPrintf("Will be malloc on CPU %.1f MB\n", (float)size/1024/1024);
    cudaHostAlloc(p, size, cudaHostAllocDefault);
    assert(*p != 0);
  }

  mutex.unlock();
}