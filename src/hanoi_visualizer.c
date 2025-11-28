#include <stdio.h>
#include "hanoi_visualizer.h"

void visualizeResults(BenchmarkData *data)
{
  // set window settings
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower of Hanoi - Results Visualization");
  SetTargetFPS(60);

  // load fonts
  Font regularFont = FileExists("assets/Roboto-Regular.ttf") ? LoadFontEx("assets/Roboto-Regular.ttf", 32, 0, 0) : GetFontDefault();
  Font boldFont = FileExists("assets/Roboto-Bold.ttf") ? LoadFontEx("assets/Roboto-Bold.ttf", 32, 0, 0) : regularFont;

  // these are used to unload the fonts(free the memory)
  bool shouldUnloadRegular = (regularFont.texture.id != GetFontDefault().texture.id);
  bool shouldUnloadBold = (boldFont.texture.id != regularFont.texture.id) && (boldFont.texture.id != GetFontDefault().texture.id);

  // index where table start
  int tableStart = 0;

  // add textures to fonts so they don't look pixelated
  if (regularFont.texture.id != 0)
    SetTextureFilter(regularFont.texture, TEXTURE_FILTER_BILINEAR);
  if (boldFont.texture.id != 0 && boldFont.texture.id != regularFont.texture.id)
    SetTextureFilter(boldFont.texture, TEXTURE_FILTER_BILINEAR);

  // screen loop
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // title and method
    DrawTextEx(boldFont, "TOWER OF HANOI - BENCHMARK RESULTS", (Vector2){20, 20}, 24, 1, DARKBLUE);
    char methodText[50];
    sprintf(methodText, "Method: %s", data->method);
    DrawTextEx(boldFont, methodText, (Vector2){20, 50}, 20, 1, DARKGRAY);

    // define areas
    Rectangle tableArea = {20, 90, 350, SCREEN_HEIGHT - 110};
    Rectangle graphArea = {390, 90, SCREEN_WIDTH - 410, SCREEN_HEIGHT - 110};

    // Calculate visible rows
    int rowHeight = 25;
    int availableHeight = (int)tableArea.height - 80;
    int maxVisibleRows = availableHeight / rowHeight;
    int maxScroll = (data->count > maxVisibleRows) ? data->count - maxVisibleRows : 0;

    // handle scrolling
    tableStart -= (int)GetMouseWheelMove();
    if (IsKeyPressed(KEY_DOWN))
      tableStart++;
    if (IsKeyPressed(KEY_UP))
      tableStart--;
    if (IsKeyPressed(KEY_PAGE_DOWN))
      tableStart += maxVisibleRows;
    if (IsKeyPressed(KEY_PAGE_UP))
      tableStart -= maxVisibleRows;

    // define scrolling bandwidth
    if (tableStart < 0)
      tableStart = 0;
    if (tableStart > maxScroll)
      tableStart = maxScroll;

    // draw table and graph
    drawTable(data, tableArea, regularFont, boldFont, tableStart);
    drawGraph(data, graphArea, regularFont, boldFont);

    // draw scrollbar if the rows exceeded the visible rows
    if (data->count > maxVisibleRows)
    {
      float trackX = tableArea.x + tableArea.width - 12;
      float trackY = tableArea.y + 80;
      float trackW = 8;
      float trackH = availableHeight;

      // static scrollbar
      DrawRectangle(trackX, trackY, trackW, trackH, LIGHTGRAY);
      DrawRectangleLines(trackX, trackY, trackW, trackH, LIGHTGRAY);

      // moving scrollbar
      float visibleRatio = (float)maxVisibleRows / (float)data->count;
      float thumbHeight = visibleRatio * trackH;
      if (thumbHeight < 20)
        thumbHeight = 20;

      float scrollRatio = (float)tableStart / maxScroll;
      float thumbY = trackY + (trackH - thumbHeight) * scrollRatio;

      DrawRectangle(trackX + 1, thumbY, trackW - 2, thumbHeight, BLACK);
    }

    // ability to close window with esc
    const char *escText = "Press ESC to close";
    Vector2 escSize = MeasureTextEx(regularFont, escText, 16.0f, 1.0f);
    DrawTextEx(regularFont, escText, (Vector2){SCREEN_WIDTH - escSize.x - 20, 12}, 16.0f, 1.0f, GRAY);

    EndDrawing();
  }

  // unload fonts
  if (shouldUnloadRegular)
    UnloadFont(regularFont);
  if (shouldUnloadBold)
    UnloadFont(boldFont);

  CloseWindow();
}

void drawTable(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont, int startIndex)
{
  // Check if custom fonts are loaded (a loaded font has a non-zero baseSize)
  bool useCustomFonts = (regularFont.baseSize > 0 && boldFont.baseSize > 0);

  // Table background
  DrawRectangleRec(area, (Color){245, 245, 245, 255});
  DrawRectangleLinesEx(area, 2, DARKGRAY);

  // Title (bold font if available)
  if (useCustomFonts)
    DrawTextEx(boldFont, "Results Table", (Vector2){area.x + 10, area.y + 10}, 18, 1, DARKBLUE);
  else
    DrawText("Results Table", area.x + 10, area.y + 10, 18, DARKBLUE);

  // Header (bold font if available)
  int headerY = area.y + 45;
  if (useCustomFonts)
  {
    DrawTextEx(boldFont, "Disks", (Vector2){area.x + 20, headerY}, 16, 1, BLACK);
    DrawTextEx(boldFont, "Moves", (Vector2){area.x + 100, headerY}, 16, 1, BLACK);
    DrawTextEx(boldFont, "Time (sec)", (Vector2){area.x + 200, headerY}, 16, 1, BLACK);
  }
  else
  {
    DrawText("Disks", area.x + 20, headerY, 16, BLACK);
    DrawText("Moves", area.x + 100, headerY, 16, BLACK);
    DrawText("Time (sec)", area.x + 200, headerY, 16, BLACK);
  }
  DrawLine(area.x + 10, headerY + 25, area.x + area.width - 10, headerY + 25, DARKGRAY);

  // Calculate visible rows area for scrolling
  int rowY = headerY + 35;
  int rowHeight = 25;
  int rowsAreaY = rowY;
  int rowsAreaHeight = (int)(area.y + area.height - rowsAreaY - 10);
  if (rowsAreaHeight < rowHeight)
    rowsAreaHeight = rowHeight;
  int visibleRows = rowsAreaHeight / rowHeight;

  // Clip to rows area to enable scrolling without drawing outside bounds
  BeginScissorMode((int)area.x + 10, rowsAreaY - 5, (int)area.width - 20, rowsAreaHeight + 10);

  // Draw each visible row
  for (int row = 0; row < visibleRows; row++)
  {
    int i = startIndex + row;
    if (i >= data->count)
      break;

    // Format the data for display
    char diskStr[10], movesStr[20], timeStr[25];
    sprintf(diskStr, "%d", data->results[i].n);
    sprintf(movesStr, "%ld", data->results[i].moves);

    // Format time with appropriate precision based on magnitude
    if (data->results[i].time_sec < 0.001)
    {
      sprintf(timeStr, "< 0.001");
    }
    else if (data->results[i].time_sec < 1.0)
    {
      sprintf(timeStr, "%.6f", data->results[i].time_sec);
    }
    else
    {
      sprintf(timeStr, "%.3f", data->results[i].time_sec);
    }

    // Alternate row colors for better readability
    Color rowColor = (i % 2 == 0) ? (Color){255, 255, 255, 255} : (Color){240, 240, 240, 255};
    float y = rowY + row * rowHeight;
    DrawRectangle(area.x + 10, y - 5, area.width - 20, rowHeight - 2, rowColor);

    // Draw row data (using custom font if available)
    if (useCustomFonts)
    {
      DrawTextEx(regularFont, diskStr, (Vector2){area.x + 20, y}, 14, 1, DARKGRAY);
      DrawTextEx(regularFont, movesStr, (Vector2){area.x + 100, y}, 14, 1, DARKGRAY);
      DrawTextEx(regularFont, timeStr, (Vector2){area.x + 200, y}, 14, 1, DARKGRAY);
    }
    else
    {
      DrawText(diskStr, area.x + 20, y, 14, DARKGRAY);
      DrawText(movesStr, area.x + 100, y, 14, DARKGRAY);
      DrawText(timeStr, area.x + 200, y, 14, DARKGRAY);
    }
  }

  EndScissorMode();
}

void drawGraph(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont)
{
  // Check if custom fonts are loaded (a loaded font has a non-zero baseSize)
  bool useCustomFonts = (regularFont.baseSize > 0 && boldFont.baseSize > 0);

  // Graph background
  DrawRectangleRec(area, (Color){245, 245, 245, 255});
  DrawRectangleLinesEx(area, 2, DARKGRAY);

  // Title (bold font if available)
  if (useCustomFonts)
    DrawTextEx(boldFont, "Execution Time vs. Number of Disks", (Vector2){area.x + 10, area.y + 10}, 18, 1, DARKBLUE);
  else
    DrawText("Execution Time vs. Number of Disks", area.x + 10, area.y + 10, 18, DARKBLUE);

  // Need at least 2 data points to draw a graph
  if (data->count < 2)
    return;

  // Define graph plotting area with padding for labels
  float padding = 60;
  float graphX = area.x + padding;
  float graphY = area.y + 50;
  float graphWidth = area.width - padding * 2;
  float graphHeight = area.height - padding - 60;

  // Draw axes
  DrawLine(graphX, graphY + graphHeight, graphX + graphWidth, graphY + graphHeight, BLACK); // X-axis
  DrawLine(graphX, graphY, graphX, graphY + graphHeight, BLACK);                            // Y-axis

  // Find min and max values for scaling
  double maxTime = 0;
  int minN = data->results[0].n;
  int maxN = data->results[0].n;

  for (int i = 0; i < data->count; i++)
  {
    if (data->results[i].time_sec > maxTime)
      maxTime = data->results[i].time_sec;
    if (data->results[i].n < minN)
      minN = data->results[i].n;
    if (data->results[i].n > maxN)
      maxN = data->results[i].n;
  }

  // Add 10% padding to max for better visualization
  maxTime *= 1.1;

  // Draw horizontal grid lines and Y-axis labels
  int numYGridLines = 5;
  for (int i = 0; i <= numYGridLines; i++)
  {
    float y = graphY + graphHeight - (i * graphHeight / numYGridLines);
    DrawLine(graphX, y, graphX + graphWidth, y, LIGHTGRAY);

    double value = (i * maxTime / numYGridLines);
    char label[20];
    sprintf(label, "%.2f", value);

    if (useCustomFonts)
      DrawTextEx(regularFont, label, (Vector2){graphX - 50, y - 8}, 12, 1, DARKGRAY);
    else
      DrawText(label, graphX - 50, y - 8, 12, DARKGRAY);
  }

  // Draw X-axis labels (number of disks)
  for (int i = 0; i < data->count; i++)
  {
    float x = graphX + (i * graphWidth / (data->count - 1));
    char label[10];
    sprintf(label, "%d", data->results[i].n);

    if (useCustomFonts)
      DrawTextEx(regularFont, label, (Vector2){x - 10, graphY + graphHeight + 10}, 12, 1, DARKGRAY);
    else
      DrawText(label, x - 10, graphY + graphHeight + 10, 12, DARKGRAY);
  }

  // Axis labels (bold font if available)
  if (useCustomFonts)
  {
    DrawTextEx(boldFont, "Time (sec)", (Vector2){area.x + 5, area.y + 25}, 14, 1, DARKGRAY);
    DrawTextEx(boldFont, "Number of Disks", (Vector2){area.x + area.width / 2 - 60, area.y + area.height - 20}, 14, 1, DARKGRAY);
  }
  else
  {
    DrawText("Time (sec)", area.x + 5, area.y + 25, 14, DARKGRAY);
    DrawText("Number of Disks", area.x + area.width / 2 - 60, area.y + area.height - 20, 14, DARKGRAY);
  }

  // Plot data points and connecting lines
  for (int i = 0; i < data->count - 1; i++)
  {
    // Calculate positions for current and next points
    float x1 = graphX + (i * graphWidth / (data->count - 1));
    float y1 = graphY + graphHeight - (data->results[i].time_sec / maxTime * graphHeight);

    float x2 = graphX + ((i + 1) * graphWidth / (data->count - 1));
    float y2 = graphY + graphHeight - (data->results[i + 1].time_sec / maxTime * graphHeight);

    // Draw line connecting points
    DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 2, BLUE);

    // Draw data point
    DrawCircle(x1, y1, 5, RED);
  }

  // Draw the last data point
  float lastX = graphX + ((data->count - 1) * graphWidth / (data->count - 1));
  float lastY = graphY + graphHeight - (data->results[data->count - 1].time_sec / maxTime * graphHeight);
  DrawCircle(lastX, lastY, 5, RED);
}