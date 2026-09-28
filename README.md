# LogViewer

LogViewer is a desktop application for parsing and visualizing MAVLink `.tlog` flight logs.

The application is built with C++ and Qt Quick/QML. It provides timeline-based navigation through telemetry data and synchronized inspection of MAVLink messages.

## Features

- Open and parse MAVLink `.tlog` files
- MAVLink 1 and MAVLink 2 message parsing
- Human-readable MAVLink message names and fields
- Raw telemetry data viewer
- Timeline-based packet navigation
- Synchronized active packet state
- Position extraction from MAVLink telemetry
- 2D OpenStreetMap visualization
- Timeline-synchronized vehicle marker
- Progressive flight path visualization
- Map pan and zoom controls

## Technologies

- C++20
- Qt 6
- Qt Quick / QML
- CMake
- MAVLink `c_library_v2`

## Requirements

- CMake 3.21 or newer
- Qt 6.8 or newer
- C++20 compatible compiler

## Clone

This project uses MAVLink as a Git submodule.

Clone the repository with submodules:

```bash
git clone --recurse-submodules <repository-url>
cd LogViewer