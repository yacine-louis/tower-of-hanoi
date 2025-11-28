#include <stdio.h>
#include "hanoi_visualizer.h"

void visualizeResults(BenchmarkData *data)
{
  // Reduce Raylib console output: show warnings/errors only (hide INFO)
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower of Hanoi - Results Visualization");
  SetTargetFPS(60);
  int tableStart = 0; // start index for visible rows (scroll)

  // Try to load Roboto Regular and Roboto Bold from the local `font/` folder.
  // Fall back to Raylib's default font when a file is missing.
  Font regularFont = {0};
  Font boldFont = {0};
  bool regularLoaded = false;
  bool boldLoaded = false;
  bool regularLoadedFromDisk = false;
  bool boldLoadedFromDisk = false;

  const char *regularPath = "assets/Roboto-Regular.ttf";
  const char *boldPath = "assets/Roboto-Bold.ttf";

  if (FileExists(regularPath))
  {
    regularFont = LoadFontEx(regularPath, 32, 0, 0);
    if (regularFont.texture.id != 0)
    {
      regularLoaded = true;
      regularLoadedFromDisk = true;
      SetTextureFilter(regularFont.texture, TEXTURE_FILTER_BILINEAR);
    }
  }

  if (FileExists(boldPath))
  {
    boldFont = LoadFontEx(boldPath, 32, 0, 0);
    if (boldFont.texture.id != 0)
    {
      boldLoaded = true;
      boldLoadedFromDisk = true;
      SetTextureFilter(boldFont.texture, TEXTURE_FILTER_BILINEAR);
    }
  }

  if (!regularLoaded)
  {
    regularFont = GetFontDefault();
    regularLoaded = true;
    SetTextureFilter(regularFont.texture, TEXTURE_FILTER_BILINEAR);
  }

  if (!boldLoaded)
  {
    // Use the regular/default font as a fallback for bold text
    boldFont = regularFont;
    boldLoaded = false;
  }

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Title (bold) and method (bold)
    DrawTextEx(boldFont, "TOWER OF HANOI - BENCHMARK RESULTS", (Vector2){20, 20}, 24, 1, DARKBLUE);
    char methodText[50];
    sprintf(methodText, "Method: %s", data->method);
    DrawTextEx(boldFont, methodText, (Vector2){20, 50}, 20, 1, DARKGRAY);

    // Define areas
    Rectangle tableArea = {20, 90, 350, SCREEN_HEIGHT - 110};
    Rectangle graphArea = {390, 90, SCREEN_WIDTH - 410, SCREEN_HEIGHT - 110};

    // Handle table scrolling (mouse wheel / keys)
    // compute visible rows to clamp scrolling
    int rowHeight_local = 25;
    int headerY_local = tableArea.y + 45;
    int rowsAreaY = headerY_local + 35;
    int rowsAreaHeight = (int)(tableArea.y + tableArea.height - rowsAreaY - 10);
    if (rowsAreaHeight < rowHeight_local)
      rowsAreaHeight = rowHeight_local;
    int maxRows_local = rowsAreaHeight / rowHeight_local;

    float wheel = GetMouseWheelMove();
    if (wheel != 0)
      tableStart -= (int)wheel; // wheel >0 scroll up -> move view up
    if (IsKeyPressed(KEY_DOWN))
      tableStart++;
    if (IsKeyPressed(KEY_UP))
      tableStart--;
    if (IsKeyPressed(KEY_PAGE_DOWN))
      tableStart += maxRows_local;
    if (IsKeyPressed(KEY_PAGE_UP))
      tableStart -= maxRows_local;

    if (tableStart < 0)
      tableStart = 0;
    if (tableStart > data->count - maxRows_local)
      tableStart = (data->count - maxRows_local) > 0 ? data->count - maxRows_local : 0;

    // Draw table/graph using the loaded fonts (regular + bold)
    drawTable(data, tableArea, regularFont, boldFont, tableStart);
    drawGraph(data, graphArea, regularFont, boldFont);

    // Draw scrollbar for table
    if (data->count > 0)
    {
      // track area (inside tableArea, right side)
      float trackX = tableArea.x + tableArea.width - 12;
      float trackY = rowsAreaY - 5;
      float trackW = 8;
      float trackH = rowsAreaHeight + 10;

      // draw track
      DrawRectangle(trackX, trackY, trackW, trackH, (Color){230, 230, 230, 255});
      DrawRectangleLines((int)trackX, (int)trackY, (int)trackW, (int)trackH, (Color){200, 200, 200, 255});

      // compute thumb size/position
      if (data->count <= maxRows_local)
      {
        // all rows visible -> thumb full
        DrawRectangle(trackX + 1, trackY + 1, trackW - 2, trackH - 2, (Color){180, 180, 180, 255});
      }
      else
      {
        float visibleRatio = (float)maxRows_local / (float)data->count;
        float thumbH = visibleRatio * trackH;
        if (thumbH < 12)
          thumbH = 12; // minimum thumb size

        int maxScroll = data->count - maxRows_local;
        float scrollRatio = (maxScroll > 0) ? (float)tableStart / (float)maxScroll : 0.0f;
        if (scrollRatio < 0)
          scrollRatio = 0;
        if (scrollRatio > 1)
          scrollRatio = 1;

        float thumbY = trackY + (trackH - thumbH) * scrollRatio;

        // draw thumb
        DrawRectangle(trackX + 1, thumbY, trackW - 2, thumbH, (Color){160, 160, 160, 255});
        DrawRectangleLines((int)(trackX + 1), (int)thumbY, (int)(trackW - 2), (int)thumbH, (Color){120, 120, 120, 255});
      }
    }

    // Instructions (top-right) using Roboto regular (fallback to default)
    {
      const char *escText = "Press ESC to close";
      float escFontSize = 16.0f;
      float escSpacing = 1.0f;
      Vector2 escSize = MeasureTextEx(regularFont, escText, escFontSize, escSpacing);
      float escX = SCREEN_WIDTH - escSize.x - 20.0f; // 20 px margin from right
      float escY = 12.0f;                            // near top
      DrawTextEx(regularFont, escText, (Vector2){escX, escY}, escFontSize, escSpacing, GRAY);
    }

    EndDrawing();
  }

  if (regularLoadedFromDisk)
  {
    UnloadFont(regularFont);
  }
  if (boldLoadedFromDisk)
  {
    // Only unload bold separately if it was loaded from disk (not the same texture)
    UnloadFont(boldFont);
  }

  CloseWindow();
}

/**
 * Draws a scrollable table displaying benchmark results
 *
 * @param data - Benchmark data to display
 * @param area - Rectangle defining the table's position and size
 * @param regularFont - Font for regular text (pass {0} or default font to use Raylib's default)
 * @param boldFont - Font for headers and titles (pass {0} or default font to use Raylib's default)
 * @param startIndex - Index of first result to display (for scrolling)
 */
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

/**
 * Draws a graph showing execution time vs number of disks
 *
 * @param data - Benchmark data to plot
 * @param area - Rectangle defining the graph's position and size
 * @param regularFont - Font for numeric labels (pass {0} to use Raylib's default)
 * @param boldFont - Font for titles and axis labels (pass {0} to use Raylib's default)
 */
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