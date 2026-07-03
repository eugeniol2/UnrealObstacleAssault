# Obstacle Assault - Unreal Engine 5 Project

This is an **Unreal Engine 5** project focused on creating modular platforming systems and trap mechanics for 3D platformers. The project leverages a blend of **C++** for core logic and **Blueprints** for visual extensibility.

## 🚀 Key Features

### 1. Modular Moving Platforms
*   **Looping Movement:** Platforms that automatically move between two configurable points (Point A to Point B).
*   **Dynamic Configuration:** Speed, movement distance, and rotation are adjustable directly via the Unreal Engine *Details* panel.
*   **Relative Displacement:** Platforms operate independently of their world position, making assets highly reusable across different levels.

### 2. Intelligent Trap System
*   **Proximity Activation:** Triggers that detect player presence and activate specific actions (e.g., tilting the platform).
*   **Hysteresis (Stability):** Implementation of a safety margin to prevent rapid on/off toggling when the player is at the edge of the detection radius.
*   **Activation Modes:** Support for both *Trigger Once* and continuous *Proximity* activation.

## 🛠️ Technical Highlights (C++)

*   **Gimbal Lock Resolution:** Utilized **Quaternions (`FQuat`)** and `QInterpTo` to ensure smooth and stable 100-degree rotations, eliminating the erratic "spinning top" behavior common with Euler angles.
*   **Performance Optimization:** Optimized distance detection logic to minimize unnecessary calculations within the `Tick` function.
*   **Real-time Performance Monitoring:** Integrated a C++ based real-time FPS counter using UMG Widgets, functional even in *Shipping* builds.

## 📂 Project Structure

*   `/Source`: C++ Source code.
*   `/Content/MyStuff`: Custom Blueprints, Widgets, and Materials.
*   `/Content/Characters`: Character assets and movement logic.

## 🎮 Getting Started

1. Clone the repository.
2. Right-click the `.uproject` file and select **Generate Visual Studio project files**.
3. Open the `.sln` file and compile the project in Visual Studio.
4. Open the project in Unreal Engine 5 and press **Play**.

---
*Developed as part of advanced studies in game logic and applied mathematics within Unreal Engine.*
