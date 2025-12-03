#include <stdio.h>
#include "hanoi_visualizer.h"

void DrawTextWithFallBack(Font font, const char *text, Vector2 position, int fontSize, float spacing, Color color)
{
  // check if custom font exist
  if (font.baseSize > 0)
    DrawTextEx(font, text, position, fontSize, spacing, color);
  else
    // use default font
    DrawText(text, (int)position.x, (int)position.y, fontSize, color);
}

void visualizeResults(BenchmarkData *data)
{
  // set window settings
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower of Hanoi");
  SetTargetFPS(60);

  // load fonts
  Font regularFont = FileExists("assets/Roboto-Regular.ttf") ? LoadFontEx("assets/Roboto-Regular.ttf", 32, 0, 0) : GetFontDefault();
  Font boldFont = FileExists("assets/Roboto-Bold.ttf") ? LoadFontEx("assets/Roboto-Bold.ttf", 32, 0, 0) : regularFont;

  // these are used to unload the fonts (free the memory)
  bool shouldUnloadRegular = (regularFont.texture.id != GetFontDefault().texture.id);
  bool shouldUnloadBold = (boldFont.texture.id != regularFont.texture.id) && (boldFont.texture.id != GetFontDefault().texture.id);

  // add textures to fonts so they don't look pixelated
  if (regularFont.texture.id != 0)
    SetTextureFilter(regularFont.texture, TEXTURE_FILTER_BILINEAR);
  if (boldFont.texture.id != 0 && boldFont.texture.id != regularFont.texture.id)
    SetTextureFilter(boldFont.texture, TEXTURE_FILTER_BILINEAR);

  // index where table start
  int tableScroll = 0;

  // screen loop
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // title and method
    DrawTextWithFallBack(boldFont, "Tower Of Hanoi", (Vector2){20, 20}, 24, 1, DARKBLUE);
    DrawTextWithFallBack(boldFont, data->method, (Vector2){20, 50}, 20, 1, DARKGRAY);

    // define areas
    Rectangle tableArea = {20, 90, 350, SCREEN_HEIGHT - 110};
    Rectangle graphArea = {390, 90, SCREEN_WIDTH - 410, SCREEN_HEIGHT - 110};

    tableScroll = drawTable(data, tableArea, regularFont, boldFont, tableScroll);
    // draw graph
    drawGraph(data, graphArea, regularFont, boldFont);

    EndDrawing();
  }

  // unload fonts
  if (shouldUnloadRegular)
    UnloadFont(regularFont);
  if (shouldUnloadBold)
    UnloadFont(boldFont);

  CloseWindow();
}

int drawTable(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont, int scroll)
{
  // set table background
  DrawRectangleRec(area, (Color){245, 245, 245, 255});
  DrawRectangleLinesEx(area, 2, DARKGRAY);

  // draw table title
  DrawTextWithFallBack(boldFont, "Results Table", (Vector2){area.x + 10, area.y + 10}, 18, 1, DARKBLUE);

  // draw table header
  int headerY = area.y + 45;
  DrawTextWithFallBack(boldFont, "Disks", (Vector2){area.x + 20, headerY}, 16, 1, BLACK);
  DrawTextWithFallBack(boldFont, "Time (sec)", (Vector2){area.x + 150, headerY}, 16, 1, BLACK);

  DrawLine(area.x + 10, headerY + 25, area.x + area.width - 10, headerY + 25, DARKGRAY);

  // calculate visible rows
  int rowHeight = 25;
  int rowY = headerY + 35;
  int rowsAreaY = rowY;
  int rowsAreaHeight = (area.y + area.height - 10) - rowsAreaY;
  if (rowsAreaHeight < rowHeight)
    rowsAreaHeight = rowHeight;

  int visibleRows = rowsAreaHeight / rowHeight;
  int maxScroll = (data->resultsLength > visibleRows) ? data->resultsLength - visibleRows : 0;

  // handle scrolling
  scroll -= (int)GetMouseWheelMove();
  if (IsKeyPressed(KEY_DOWN))
    scroll++;
  if (IsKeyPressed(KEY_UP))
    scroll--;
  if (IsKeyPressed(KEY_PAGE_DOWN))
    scroll += visibleRows;
  if (IsKeyPressed(KEY_PAGE_UP))
    scroll -= visibleRows;

  // define scroll bandwidth
  if (scroll < 0)
    scroll = 0;
  if (scroll > maxScroll)
    scroll = maxScroll;

  // enable scrolling
  BeginScissorMode((int)area.x + 10, rowsAreaY - 5, (int)area.width - 20, rowsAreaHeight + 10);

  // draw visible rows
  for (int row = 0; row < visibleRows; row++)
  {
    int i = scroll + row;
    if (i >= data->resultsLength)
      break;

    char diskStr[10];
    char timeStr[20];

    // format data
    sprintf(diskStr, "%d", data->results[i].n);

    // format time
    if (data->results[i].time_sec < 0.001)
      sprintf(timeStr, "< 0.001");
    else
      sprintf(timeStr, "%.4f", data->results[i].time_sec);

    // set color for row
    Color rowColor = (i % 2 == 0) ? (Color){255, 255, 255, 255} : (Color){240, 240, 240, 255};
    float y = rowY + row * rowHeight;
    DrawRectangle(area.x + 10, y - 5, area.width - 20, rowHeight - 2, rowColor);

    // draw row content
    DrawTextWithFallBack(regularFont, diskStr, (Vector2){area.x + 20, y}, 16, 1, DARKGRAY);
    DrawTextWithFallBack(regularFont, timeStr, (Vector2){area.x + 150, y}, 16, 1, DARKGRAY);
  }

  EndScissorMode();

  // draw scrollbar
  if (data->resultsLength > visibleRows)
  {
    int trackX = (int)(area.x + area.width - 12);
    int trackY = (int)(area.y + 80);
    int trackW = 8;
    int trackH = rowsAreaHeight;

    DrawRectangle(trackX, trackY, trackW, trackH, LIGHTGRAY);
    DrawRectangleLines(trackX, trackY, trackW, trackH, LIGHTGRAY);

    float ratio = (float)visibleRows / data->resultsLength;
    int thumbH = ratio * trackH;
    if (thumbH < 20)
      thumbH = 20;

    float posRatio = (float)scroll / maxScroll;
    int thumbY = trackY + (int)((trackH - thumbH) * posRatio);

    DrawRectangle(trackX + 1, thumbY, trackW - 2, thumbH, BLACK);
  }

  return scroll;
}

void drawGraph(BenchmarkData *data, Rectangle area, Font regularFont, Font boldFont)
{
  // set graph background
  DrawRectangleRec(area, (Color){245, 245, 245, 255});
  DrawRectangleLinesEx(area, 2, DARKGRAY);

  // draw graph title
  DrawTextWithFallBack(boldFont, "Execution Time vs. Number of Disks", (Vector2){area.x + 0.75 * area.x, area.y + 10}, 18, 1, DARKBLUE);

  // assure that least 2 points to draw graph
  if (data->resultsLength < 2)
    return;

  // define graph area
  float padding = 60;
  float graphX = area.x + padding;
  float graphY = area.y + 50;
  float graphWidth = area.width - padding * 2;
  float graphHeight = area.height - padding - 60;

  // draw x-axis and y-axis
  DrawLine(graphX, graphY + graphHeight, graphX + graphWidth, graphY + graphHeight, BLACK);
  DrawLine(graphX, graphY, graphX, graphY + graphHeight, BLACK);

  // find min and max values
  double maxTime = 0;
  int minN = data->results[0].n;
  int maxN = data->results[0].n;

  for (int i = 0; i < data->resultsLength; i++)
  {
    if (data->results[i].time_sec > maxTime)
      maxTime = data->results[i].time_sec;
    if (data->results[i].n < minN)
      minN = data->results[i].n;
    if (data->results[i].n > maxN)
      maxN = data->results[i].n;
  }

  if (maxTime < 0.001)
    maxTime = 0.001;

  // draw graph lines and the time in y-axis
  int numYGridLines = 5;
  for (int i = 0; i <= numYGridLines; i++)
  {
    float y = graphY + graphHeight - (i * graphHeight / numYGridLines);
    DrawLine(graphX, y, graphX + graphWidth, y, LIGHTGRAY);

    double value = (i * maxTime / numYGridLines);
    char label[20];
    sprintf(label, "%.2f", value);

    DrawTextWithFallBack(regularFont, label, (Vector2){graphX - 50, y - 8}, 14, 1, DARKGRAY);
  }

  // draw number of disks in x_axis
  for (int i = 0; i < data->resultsLength; i++)
  {
    float x = graphX + (i * graphWidth / (data->resultsLength - 1));
    char label[10];
    sprintf(label, "%d", data->results[i].n);

    DrawTextWithFallBack(regularFont, label, (Vector2){x - 10, graphY + graphHeight + 10}, 14, 1, DARKGRAY);
  }

  // draw labels from x-axis and y-axis
  DrawTextWithFallBack(boldFont, "Time (sec)", (Vector2){area.x + 5, area.y + 25}, 14, 1, DARKGRAY);
  DrawTextWithFallBack(boldFont, "Number of Disks", (Vector2){area.x + area.width / 2 - 60, area.y + area.height - 20}, 14, 1, DARKGRAY);

  // draw graph points and connect them
  for (int i = 0; i < data->resultsLength - 1; i++)
  {
    // calculate positions for current and next points
    float x1 = graphX + (i * graphWidth / (data->resultsLength - 1));
    float y1 = graphY + graphHeight - (data->results[i].time_sec / maxTime * graphHeight);

    float x2 = graphX + ((i + 1) * graphWidth / (data->resultsLength - 1));
    float y2 = graphY + graphHeight - (data->results[i + 1].time_sec / maxTime * graphHeight);

    // draw line connecting the 2 points
    DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 2, BLUE);

    // draw data point
    DrawCircle(x1, y1, 5, RED);
  }

  // draw the last data point
  float lastX = graphX + ((data->resultsLength - 1) * graphWidth / (data->resultsLength - 1));
  float lastY = graphY + graphHeight - (data->results[data->resultsLength - 1].time_sec / maxTime * graphHeight);
  DrawCircle(lastX, lastY, 5, RED);
}