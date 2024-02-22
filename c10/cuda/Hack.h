#define DbgLogSwitch 1

#define DbgPrintf(...) \
if (DbgLogSwitch) { \
  printf(__VA_ARGS__); \
}

void hackCUDAMalloc(void **p, size_t size);