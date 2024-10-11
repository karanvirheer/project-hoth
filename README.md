# Project Hoth

A 3D "Star Wars Planet Hoth" style game, where walker bots attack an interactive defensive cannon. This project combines ideas and code from earlier assignments to create an engaging game simulation in OpenGL. The game involves managing enemy walker bots and controlling a defensive cannon to protect against their attacks.

## Features

### 1. Enemy Walker Bots Attacking
- Utilizes robots from Assignment 1, with multiple instances created.
- Robots walk toward the defensive cannon and fire projectiles with random accuracy.
- If hit by a projectile from the cannon, robots disappear (animated for bonus marks).
- Custom mesh parts from Assignment 2 are integrated into robots.
- All robot parts are texture-mapped for enhanced visuals using a shader.

### 2. Defensive Cannon
- Cannon is controlled by mouse movements (or arrow keys as a fallback).
- Cannon fires projectiles at incoming robots (triggered by the spacebar key).
- Texture mapping is applied to all parts of the cannon.
- Camera is positioned above and behind the cannon for optimal viewing.
- Cannon is disabled when hit by a robot’s projectile, and for teams of two, an animation shows the cannon in a "broken state."

### 3. Shaders
- A GPU vertex shader and fragment shader are used for lighting, shading, and texture mapping.
- Simplified shaders from class are employed for lighting and shading.

### 4. Collision Detection
- Collision between projectiles and robots or the cannon is detected using bounding boxes or distance thresholds.

### 5. Game Controls
- Mouse/Arrow keys: Aim the cannon.
- Spacebar: Fire projectiles.

## Bonus Features
- **Laser Cannon**: The defensive cannon can fire a laser instead of projectiles.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/karanvirheer/project-hoth.git
   ```
