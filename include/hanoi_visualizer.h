#ifndef HANOI_VISUALIZER_H
#define HANOI_VISUALIZER_H

#include "raylib.h"
#include "hanoi_benchmark.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700

// fill the benchmark table/graph
void visualizeResults(BenchmarkData *data);

// draw the benchmark table
int drawTable(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont, int scroll);

// draw the benchmark graph
void drawGraph(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont);

// draw text using custom font
void DrawTextWithFallBack(Font font, const char *text, Vector2 position, int fontSize, float spacing, Color color);

#endif
