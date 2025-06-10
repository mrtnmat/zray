#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdio.h>

typedef char *String;

const int SCREEN_WIDTH = 16 * 100;
const int SCREEN_HEIGHT = 9 * 100;
bool cursor_captured = true;

typedef struct {
  Camera3D camera;
  Vector3 *path;
  int path_length;
  float path_time;
  float speed;
  float yaw;   // Left/right rotation
  float pitch; // Up/down rotation
} RailsCamera;

RailsCamera rails_camera;

static const Vector3 main_path[] = {
    {0, 3, -10}, // Start behind
    {0, 3, 0},   // Move forward to center
    {10, 3, 0},  // Turn right
    {10, 3, 10}  // End forward
};

Camera3D initCamera() {
  Camera3D camera = {0};
  // -L/+R - -D/+U - -In/+Out
  camera.position = (Vector3){0, 0, 8};
  camera.target = (Vector3){0, 0, 1};
  camera.up = (Vector3){0, 1, 0};
  camera.fovy = 50.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  return camera;
}

RailsCamera initRailsCamera(Camera3D camera) {
  RailsCamera r_camera = {0};
  r_camera.camera = camera;
  r_camera.speed = 0.1f;
  return r_camera;
};

void lookDirection(RailsCamera r, Vector3 *v) {
  v->x = cosf(r.pitch) * cosf(r.yaw);
  v->y = sinf(r.pitch);
  v->z = cosf(r.pitch) * sinf(r.yaw);
}

void toggleCursor() {
  cursor_captured = !cursor_captured;
  if (cursor_captured)
    DisableCursor();
  else
    EnableCursor();
}

void drawAxis() {
  // Optional: Add some grid lines on the ground
  for (int i = -10; i <= 10; i++) {
    DrawLine3D((Vector3){i, 0, -10}, (Vector3){i, 0, 10}, LIGHTGRAY);
    DrawLine3D((Vector3){-10, 0, i}, (Vector3){10, 0, i}, LIGHTGRAY);
  }

  static const int l = 100;
  // X-axis (Red)
  DrawLine3D((Vector3){-l, 0, 0}, (Vector3){l, 0, 0}, RED);
  // Y-axis (Green)
  DrawLine3D((Vector3){0, -l, 0}, (Vector3){0, l, 0}, GREEN);
  // Z-axis (Blue)
  DrawLine3D((Vector3){0, 0, -l}, (Vector3){0, 0, l}, BLUE);
}

void drawMyCube() {
  DrawCube((Vector3){0.0f, 1.0f, 0.0f}, 2.0f, 2.0f, 2.0f, MAROON);
  DrawCube((Vector3){5.0f, 2.0f, 0.0f}, 2.0f, 1.0f, 1.0f, BLUE);
}

void draw3D(Camera3D c) {

  ClearBackground(WHITE);

  BeginMode3D(c);
  drawAxis();
  drawMyCube();

  EndMode3D();
}

// DEBUG

void debugCursorPosition(char (*buffer)[128], Vector2 pos) {
  const Vector2 mouse = GetMousePosition();
  snprintf(*buffer, sizeof(*buffer), "x: %.0f, y: %.0f", mouse.x, mouse.y);
  DrawText(*buffer, pos.x, pos.y, 22, BLACK);
}
void debugPathInfo(char (*buffer)[128], Vector2 pos) {
  snprintf(*buffer, sizeof(*buffer), "path_time: %.3f", rails_camera.path_time);
  DrawText(*buffer, pos.x, pos.y, 22, BLACK);
}
void debugCameraInfo(char (*buffer)[128], RailsCamera *c, Vector2 pos) {
  snprintf(*buffer, sizeof(*buffer), "x: %.3f, y: %.3f, z: %.3f",
           c->camera.position.x, c->camera.position.y, c->camera.position.z);
  DrawText(*buffer, pos.x, pos.y, 22, BLACK);

  snprintf(*buffer, sizeof(*buffer), "yaw: %.3f, pitch: %.3f, speed: %.3f",
           c->yaw, c->pitch, c->speed);
  DrawText(*buffer, pos.x, pos.y + 20, 22, BLACK);
}
void debugCursorLines() {
  const Vector2 mouse = GetMousePosition();
  DrawLine(0, mouse.y, SCREEN_WIDTH, mouse.y, LIGHTGRAY);
  DrawLine(mouse.x, 0, mouse.x, SCREEN_HEIGHT, LIGHTGRAY);
}

void debugInfo() {
  char buffer[128];
  // debugCursorPosition(&buffer, (Vector2){.x = 10, .y = 10});
  debugPathInfo(&buffer, (Vector2){.x = 10, .y = 10});
  debugCameraInfo(&buffer, &rails_camera, (Vector2){.x = 10, .y = 30});
  // debugCursorLines();
}

// EXECUTION

void initGame() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ZBoard");
  rails_camera = initRailsCamera(initCamera());
  SetTargetFPS(60);
  DisableCursor();
}

void updateFrame() {
  if (cursor_captured) {
    rails_camera.path_time += GetFrameTime() * rails_camera.speed;
    int path_step = (int)rails_camera.path_time % 4;
    float t = rails_camera.path_time - (int)rails_camera.path_time;

    // Handle mouse input for looking around
    Vector2 mouse_delta = GetMouseDelta();
    rails_camera.yaw += mouse_delta.x * 0.002f;   // Sensitivity
    rails_camera.pitch -= mouse_delta.y * 0.002f; // Negative for natural feel

    // Clamp pitch so you can't flip upside down
    rails_camera.pitch = Clamp(rails_camera.pitch, -1.5f, 1.5f);

    Vector3 new_dir = {0, 0, 1};
    lookDirection(rails_camera, &new_dir);

    rails_camera.camera.position =
        Vector3Lerp(main_path[path_step], main_path[(path_step + 1) % 4], t);
    rails_camera.camera.target =
        Vector3Add(rails_camera.camera.position, new_dir);
  }

  // To get your cursor back (maybe press ESC):
  if (IsKeyPressed(KEY_F9)) {
    toggleCursor();
  }

  // Draw the screen

  BeginDrawing();
  ClearBackground(RAYWHITE);

  draw3D(rails_camera.camera);

  debugInfo();

  EndDrawing();
}

int main() {
  initGame();

  while (!WindowShouldClose()) {
    updateFrame();
  }
}
