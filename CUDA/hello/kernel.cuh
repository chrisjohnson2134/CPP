const size_t THREADS_IN_BLOCK = 1024;

typedef double numeric_t;

// sample kernel function headers
__global__ void sumKernel(numeric_t* out, numeric_t* f, numeric_t* blockSum, size_t N);
__global__ void expKernel(numeric_t* out, numeric_t* in, size_t N);
// ..

// strong-typed wrapper for a kernel with 4 arguments
template <typename T1, typename T2, typename T3, typename T4>
void runKernel(void (*fun)(T1, T2, T3, T4), int Blocks, T1 arg1, T2 arg2, T3 arg3, T4 arg4) { 
    fun <<<Blocks, THREADS_IN_BLOCK >>> (arg1, arg2, arg3, arg4);
}

// strong-typed wrapper for a kernel with 3 arguments
template <typename T1, typename T2, typename T3>
void runKernel(void (*fun)(T1, T2, T3), int Blocks, T1 arg1, T2 arg2, T3 arg3) { 
    fun <<<Blocks, THREADS_IN_BLOCK >>> (arg1, arg2, arg3);
}

// ...

// the one-argument fun cannot have implementation here
void runKernel(void (*fun)(), int Blocks);