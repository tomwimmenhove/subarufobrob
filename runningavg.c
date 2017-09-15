#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "runningavg.h"

void runningAvgInit(runningAvgContext* ctx, int len)
{
        ctx->history = malloc(len * sizeof(double));
        memset(ctx->history, 0, len * sizeof(double));
        ctx->len = len;
        ctx->pos = 0;
        ctx->total = 0.0;
}

void runningAvgDone(runningAvgContext* ctx)
{
        free(ctx->history);
}

double runningAvg(runningAvgContext* ctx, double sample)
{
        ctx->total -= ctx->history[ctx->pos];
        ctx->history[ctx->pos] = sample;

        ctx->total += sample;

        ctx->pos++;
        if (ctx->pos >= ctx->len)
        {
                ctx->pos = 0;
        }

        return ctx->total / (double) ctx->len;;
}

