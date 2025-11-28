#ifndef HANOI_VISUALIZER_H
#define HANOI_VISUALIZER_H

#include "raylib.h"
#include "hanoi_benchmark.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700

// visualization functions

// fill the benchmark table/graph
void visualizeResults(BenchmarkData *data);

// draw the benchmark table
void drawTable(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont, int startIndex);

// draw the benchmark graph
void drawGraph(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont);

#endif