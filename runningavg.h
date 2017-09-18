#ifndef RUNNINGAVG_H
#define RUNNINGAVG_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
        int len;
        double total;
        int pos;
        double* history;
} runningAvgContext;

void runningAvgInit(runningAvgContext* ctx, int len);
void runningAvgDone(runningAvgContext* ctx);
double runningAvg(runningAvgContext* ctx, double sample);

#endif // RUNNINGAVG_H

