# KbGrafikoak - 3D Object Viewer and Transformer

This repository contains **KbGrafikoak**, a C-based 3D graphics application built with OpenGL and GLUT. It allows users to load, visualize, and interactively transform 3D models (Wavefront `.obj` format) through a variety of geometric operations and advanced lighting/camera configurations.

## Features

* **3D Model Support**: Reads and parses Wavefront `.obj` files, calculating face and vertex normals automatically for proper shading.
* **Comprehensive Transformations**:
    * **Translation, Rotation, and Scaling**.
    * **Reflection (Mirroring)** and **Shearing (Zizailaketa)**.
    * Support for both **Global** and **Local** reference systems.
* **Advanced Camera System**:
    * Support for multiple cameras with easy switching.
    * **Analysis Mode**: Camera rotates around the selected object.
    * **Flight Mode**: Manual navigation through the 3D space.
    * Support for both **Perspective (Frustum)** and **Parallel (Orthographic)** projections.
* **Dynamic Lighting Engine**:
    * **Bulb (Point Light)**: Positionable light source with attenuation.
    * **Sun (Directional Light)**: Distant light source with global direction.
    * **Spotlights (Fokoak)**: Two distinct spotlights—one attached to the active camera and one attached to the selected object.
    * Real-time toggling and parameter adjustment (intensity, angle, attenuation).
* **Rendering Modes**:
    * Toggle between **Wireframe** and **Solid (Fill)** modes.
    * Support for **Flat** and **Smooth (Gouraud)** shading models.
* **Undo/Redo System**: Maintain a stack of transformations for objects, cameras, and lights to easily revert or reapply changes.

## Requirements

* **C Compiler** (e.g., GCC).
* **OpenGL** development libraries.
* **GLU** (OpenGL Utility Library).
* **GLUT** (FreeGLUT recommended).
* **Math Library** (`-lm`).

## Building and Running

A shell script is provided for easy compilation on Linux systems.

1.  **Compile**:
    ```bash
    chmod +x konpilatu.sh
    ./konpilatu.sh
    ```
2.  **Execute**:
    The script automatically runs the binary `./KbGprograma` after a successful build.

## Controls

The application is heavily driven by keyboard input. Below is a summary of the primary controls:

### General & Selection
* **`F / f`**: Load a new `.obj` model (prompts for file path in terminal).
* **`TAB`**: Cycle through loaded objects.
* **`DEL`**: Delete the currently selected object.
* **`I / i`**: Display information about the selected object in the terminal.
* **`ESC`**: Exit the program.

### Transformation Modes
* **`T / t`**: Enable **Translation**.
* **`R / r`**: Enable **Rotation**.
* **`E / e`**: Enable **Scaling**.
* **`S / s`**: Enable **Reflection**.
* **`P / p`**: Enable **Shearing**.

### Reference Systems & Context
* **`G / g`**: Set transformation to **Global** system (or Camera Analysis mode).
* **`L / l`**: Set transformation to **Local** system (or Camera Flight mode).
* **`O / o`**: Target transformations at the **Object**.
* **`C / c`**: Target transformations at the **Camera**.
* **`A / a`**: Target transformations at the **Lights**.

### Interaction (Arrow Keys / Page Up & Down)
* **Up/Down**: $\pm Y$ translation, $X$ rotation, $Y$ scale, etc.
* **Left/Right**: $\pm X$ translation, $Y$ rotation, $X$ scale, etc.
* **PgUp/PgDn**: $\pm Z$ translation, $Z$ rotation, $Z$ scale, etc.

### Lighting Controls
* **`1, 2, 3, 4`**: Select Bulb, Sun, Object Spotlight, or Camera Spotlight respectively.
* **`F1 - F4`**: Toggle the selected light source ON/OFF.
* **`F9`**: Enable/Disable global lighting.
* **`+ / -`**: Increase/Decrease spotlight cutoff angle.

## Project Structure

* **`definitions.h`**: Data structures for points, vectors, vertices, faces, objects, and lights.
* **`main.c`**: Initializes the OpenGL window, global states, and default cameras/lights.
* **`display.c`**: Contains the core rendering loop and coordinate math.
* **`io.c`**: Handles keyboard input and the logic for all transformations.
* **`load_obj.c`**: Logic for reading and parsing Wavefront `.obj` files.
* **`objektuak/`**: Directory containing sample `.obj` models (teapot, skyscraper, cube, etc.).
