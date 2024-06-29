#include "node.h"
#include <stdio.h>

#define BATCH 32

__global__ void JustCopy(uint8_t *bf, uint8_t *res, int cols)
{
    //int i = blockIdx.x * blockDim.x + threadIdx.x;
    res[0] = 4;
    bf[0] = 5;
    for(int i=0;i<BATCH*FIELD_Y*FIELD_X;i++){
        bf[i] = 10;
    }
    //if(i < cols) {
    //    bf[i*FIELD_Y] = res[0];
    //}
}

int main(void)
{
    uint8_t start[FIELD_X][FIELD_Y];
    memset(start, 0, sizeof(start));
    start[0][1]=2;
    start[0][3]=2;

    uint8_t batch_fields[BATCH][FIELD_X][FIELD_Y];
    for(int i=0;i<BATCH;i++)
        memcpy(batch_fields[i], start, sizeof(start));

    uint8_t results[BATCH*FIELD_X][FIELD_X][FIELD_Y];
    uint8_t *d_batch_fields;
    uint8_t *d_results;

    cudaMalloc(&d_batch_fields, sizeof(batch_fields));
    cudaMalloc(&d_results, sizeof(results));
    
    cudaMemcpy(d_batch_fields, batch_fields, sizeof(batch_fields), cudaMemcpyHostToDevice);

    for(int i=0;i<FIELD_X;i++)
        cudaMemcpy(d_results+i*BATCH*FIELD_SZ, d_batch_fields, sizeof(batch_fields), cudaMemcpyDeviceToDevice);

    JustCopy<<<3,18>>>(d_batch_fields, d_results, BATCH*FIELD_X);

    uint8_t end[FIELD_X][FIELD_Y];
    //cudaMemcpy(end, d_results, sizeof(end), cudaMemcpyDeviceToHost);
    cudaMemcpy(end, d_batch_fields, sizeof(end), cudaMemcpyDeviceToHost);
    print_field(end);

    cudaFree(d_batch_fields);
    cudaFree(d_results);
    return 0;
}
