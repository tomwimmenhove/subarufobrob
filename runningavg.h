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

