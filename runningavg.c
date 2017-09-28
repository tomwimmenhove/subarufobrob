/*
 *  Copyright 2017 Tom Wimmenhove<tom.wimmenhove@gmail.com>
 *
 *  This file is part of Subarufobrob
 *
 *  Subarufobrob is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Subarufobrob is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Subarufobrob.  If not, see <http://www.gnu.org/licenses/>.
 */

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

