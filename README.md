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

## UI/UX Design

The application interface was designed in Figma before being implemented with Qt Quick/QML.

- [View the Figma design](https://www.figma.com/design/MectWJqzZVeS06mNQWL5sg/LogViewer-UI?m=auto&t=6GnnNgyCTjl7fHjy-1)

## Technologies

- C++20
- Qt 6
- Qt Quick / QML
- CMake
- MAVLink `c_library_v2`
- Docker
- Figma
  
## Requirements

For a native build:

- CMake 3.21 or newer
- Qt 6.8 or newer
- C++20 compatible compiler
- An internet connection for loading OpenStreetMap map tiles

Alternatively, Docker can be used to build the application in a Linux environment.

> Note: The application can parse and display `.tlog` data without an internet connection, but the 2D map requires internet access to load OpenStreetMap tiles.

## Clone

This project uses MAVLink as a Git submodule.

Clone the repository together with its submodules:

```bash
git clone --recurse-submodules https://github.com/BerkayTutkunca/LogViewer.git
cd LogViewer
```

If the repository was cloned without submodules:

```bash
git submodule update --init --recursive
```

## Native build

Configure the project:

```bash
cmake -S . -B build
```

Build the application:

```bash
cmake --build build
```

## Docker build

The project can be built in a reproducible Linux environment using Docker.

Build the Docker image:

```bash
docker build -t logviewer .
```

Verify that the executable was generated successfully:

```bash
docker run --rm logviewer ls -lh /app/build/appLogViewer
```

The Docker image provides a Linux build environment containing the required Qt and build dependencies.

The graphical application itself is intended to run in a desktop environment with display support.

## Project structure
```text
LogViewer/
|-- qml/
|   |-- assets/
|   |-- components/
|   `-- pages/
|-- src/
|   |-- application/
|   |-- domain/
|   |-- infrastructure/
|   |-- models/
|   `-- playback/
|-- third_party/
|   `-- mavlink/
|-- CMakeLists.txt
|-- Dockerfile
|-- .dockerignore
`-- README.md
