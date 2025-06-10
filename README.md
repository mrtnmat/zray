# ZRail

A 3D rail camera system built with C and Raylib, featuring smooth path interpolation and mouse look controls.

## Overview

ZRail demonstrates a camera system that follows a predefined 3D path while allowing free-look mouse controls. The camera smoothly interpolates between waypoints using spline-based movement, creating a rail-like experience similar to scripted sequences in games or architectural walkthroughs.

## Features

- **Rail Camera System**: Automatic movement along a predefined 3D path
- **Mouse Look Controls**: Free-look camera rotation with configurable sensitivity
- **Smooth Interpolation**: Vector3 lerping for seamless path transitions
- **Cross-Platform**: Native Windows builds and WebAssembly for browsers
- **Real-time Debug Info**: On-screen display of camera state and timing

## Controls

- **Mouse**: Look around (yaw and pitch rotation)
- **F9**: Toggle cursor capture on/off

## Building

### Windows (Native)

Requirements:
- [Zig](https://ziglang.org/) compiler
- PowerShell

```bash
powershell -ExecutionPolicy Bypass -File build.ps1
```

This produces `raydemo.exe` ready to run.

### Web (WASM)

Requirements:
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)

```bash
powershell -ExecutionPolicy Bypass -File wasm.ps1
```

This generates `web/index.html` along with the necessary `.wasm` and `.js` files. Serve the `web/` directory with any HTTP server.

## Architecture

The application uses a single-file architecture (`main.c`) with these key components:

- **RailsCamera**: Custom camera structure combining Raylib's Camera3D with path timing and rotation state
- **Path System**: Array of Vector3 waypoints with time-based interpolation
- **Input Handling**: Mouse delta processing for smooth look controls
- **Rendering Loop**: Standard Raylib render cycle with 3D scene and debug overlay

The camera position automatically progresses through the path waypoints while the target is calculated from the current position plus the look direction vector.

## Dependencies

- [Raylib](https://www.raylib.com/) 4.x - Graphics and windowing
- Zig CC (Windows builds) or Emscripten (Web builds)

## License

This project is provided as-is for educational and demonstration purposes.