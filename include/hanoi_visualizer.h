#ifndef HANOI_VISUALIZER_H
#define HANOI_VISUALIZER_H

#include "raylib.h"
#include "hanoi_benchmark.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700

// Visualization functions (opens Raylib window)
void visualizeResults(BenchmarkData *data);
void drawTable(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont, int startIndex);
void drawGraph(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont);

#endif