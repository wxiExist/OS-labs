#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    double* input;      
    double* output;     
    double* kernel;      
    int rows;            
    int cols;            
    int kernelSize;      
    int rowStart;        
    int rowEnd;          
} ThreadData;


DWORD WINAPI threadConvolution(LPVOID lpParam)
{
    ThreadData* td = (ThreadData*)lpParam;

    int kHalf = td->kernelSize / 2;  
    for (int r = td->rowStart; r < td->rowEnd; r++) {
        for (int c = 0; c < td->cols; c++) {
            double sum = 0.0;
            for (int kr = 0; kr < td->kernelSize; kr++) {
                for (int kc = 0; kc < td->kernelSize; kc++) {
                    int rr = r + kr - kHalf;
                    int cc = c + kc - kHalf;
                    
                    if (rr >= 0 && rr < td->rows && cc >= 0 && cc < td->cols) {
                        sum += td->input[rr * td->cols + cc] 
                               * td->kernel[kr * td->kernelSize + kc];
                    }
                }
            }
            td->output[r * td->cols + c] = sum;
        }
    }
    return 0;
}

void applyConvolution2D(
    double* input, 
    double* output, 
    double* kernel, 
    int rows, 
    int cols, 
    int kernelSize, 
    int maxThreads)
{
    int numThreads = (rows < maxThreads) ? rows : maxThreads;
    
    int chunkSize = rows / numThreads;
    int remainder = rows % numThreads;
    
    HANDLE* threads = (HANDLE*)malloc(numThreads * sizeof(HANDLE));
    ThreadData* td = (ThreadData*)malloc(numThreads * sizeof(ThreadData));
    
    int currentStart = 0;
    
    for (int i = 0; i < numThreads; i++) {
        int start = currentStart;
        int size = chunkSize + ((i < remainder) ? 1 : 0);
        int end = start + size;
        
        td[i].input = input;
        td[i].output = output;
        td[i].kernel = kernel;
        td[i].rows = rows;
        td[i].cols = cols;
        td[i].kernelSize = kernelSize;
        td[i].rowStart = start;
        td[i].rowEnd = end;
        

        threads[i] = CreateThread(
            NULL,              
            0,                 
            threadConvolution, 
            &td[i],            
            0,                 
            NULL               
        );
        
        currentStart = end;
    }
    
    WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);
    
    for (int i = 0; i < numThreads; i++) {
        CloseHandle(threads[i]);
    }
    free(threads);
    free(td);
}
int main(int argc, char* argv[])
{
    if (argc < 6) {
        printf("Usage: %s <rows> <cols> <kernelSize> <K> <maxThreads>\n", argv[0]);
        return 1;
    }
    
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int kernelSize = atoi(argv[3]);
    int K = atoi(argv[4]);
    int maxThreads = atoi(argv[5]);
    
    if (rows <= 0 || cols <= 0 || kernelSize <= 0 || K <= 0 || maxThreads <= 0) {
        printf("Error: invalid arguments\n");
        return 1;
    }
    
    double* matrix = (double*)malloc(rows * cols * sizeof(double));
    double* temp   = (double*)malloc(rows * cols * sizeof(double));
    
    srand(12345);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            matrix[r * cols + c] = (double)(rand() % 100) / 10.0;
        }
    }
    
    double* kernel = (double*)malloc(kernelSize * kernelSize * sizeof(double));
    for (int i = 0; i < kernelSize * kernelSize; i++) {
        kernel[i] = (double)(rand() % 10) / 10.0;
    }
    
    printf("Matrix size: %dx%d\n", rows, cols);
    printf("Kernel size: %d\n", kernelSize);
    printf("Number of iterations (K): %d\n", K);
    printf("Maximum number of threads: %d\n", maxThreads);


    for (int i = 0; i < K; i++) {
        applyConvolution2D(matrix, temp, kernel, rows, cols, kernelSize, maxThreads);
        double* swap = matrix;
        matrix = temp;
        temp = swap;
    }
   
    
    printf("Resulting matrix:\n");
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%6.2f ", matrix[r * cols + c]);
        }
        printf("\n");
    }
   
    
    // Free resources
    free(matrix);
    free(temp);
    free(kernel);

    return 0;
}
