# Oblique Throw Simulation

A dynamic simulation of oblique projectile motion with an interactive and customizable user experience.

This project was independently developed as a final project to complete the **Introduction to Computer Science** course ahead of schedule during my studies.

## 📄 Project Presentation

You can view the original project presentation by clicking the link below:

<h1 align="center">
  <a href="./project_presentation.pdf" target="_blank">📄 View Presentation</a>
</h1>

## Features

### 🎨 Interactive GUI
- Manual configuration of simulation parameters:
  - Initial speed
  - Launch angle
  - Initial height
  - Gravity acceleration
- Real-time display of position and velocity components (`Vx`, `Vy`)
- Visualization of the projectile's trajectory

### 🎛 Simulation Controls
- Pause, resume, or reset the simulation
- Toggle between a ball-following or static view
- Dynamically adjust zoom levels

### ⚙️ Advanced Customization
- Modify appearance via a configuration file:
  - Background, ball, and ground colors
  - Ball radius and tracer size
  - Name of the statistics output file

### 📊 Statistics & Analysis
- Displays final flight parameters:
  - Range, max height, and time to max height
- Saves simulation statistics to a specified file

## 🛠 Config Values

- Set colors using `uint32` numbers (max value for white: **4294967295**)

## Controls

- **S** → Start movement
- **G** → Pause movement
- **R** → Restart the simulation
- **Z** → Zoom in
- **X** → Zoom out
- **C** → Center view on the ball
- **V** → Toggle ball-following view

## Configuration

Configuration file can be changed in config.txt file

#### Example "Dark Mode" Config:
```
stats_file=stats.txt
color_ball=252645375
color_background=2305343
color_ground=5980671
unit_to_px=50
ball_radius=20
view_change=50
tracer_radius=10
```

#### Default Config:
```
stats_file=stats.txt
color_ball=
color_background=
color_ground=
unit_to_px=
ball_radius=
view_change=
tracer_radius=
```
