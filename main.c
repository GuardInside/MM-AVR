#include <stdio.h>
#include <stdlib.h>

struct memBlock
{
    void*   ptr;
    size_t  size;
};

struct microController
{
    const size_t    memLim;
    size_t          memAlloc;
};

void avail(const struct microController* m, size_t* n)
{
    *n = m->memLim - m->memAlloc;
}

struct memBlock* microMalloc(struct microController* m, size_t n)
{
    size_t memAvail = 0;
    avail(m, &memAvail);
    if(n > memAvail)
        return NULL;
    m->memAlloc += n;
    struct memBlock* block = malloc(sizeof(struct memBlock));
    block->ptr = malloc(n);
    block->size = n;
    return block;
}

void microFree(struct microController* m, struct memBlock* block)
{
    m->memAlloc -= block->size;
    free(block->ptr);
    free(block);
}

void microMemInfo(struct microController* m)
{
    size_t memAvail = 0;
    avail(m, &memAvail);
    printf("Memory info\nLimit: %d\nIn use: %d\nAvailable: %d\n\n", m->memLim, m->memAlloc, memAvail);
}

int main()
{
    struct microController myMicroController = {512, 0};
    microMemInfo(&myMicroController);

    struct memBlock* memBlock0 = microMalloc(&myMicroController, 256);
    microMemInfo(&myMicroController);

    struct memBlock* memBlock1 = microMalloc(&myMicroController, 128);
    microMemInfo(&myMicroController);

    microFree(&myMicroController, memBlock0);
    microMemInfo(&myMicroController);

    return 0;
}
