cmake_minimum_required(VERSION 3.21)

project(
    LogViewer
    VERSION 0.1.0
    LANGUAGES CXX
)

set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(
    Qt6 REQUIRED
    COMPONENTS
        Quick
        Location
        Positioning
)

qt_standard_project_setup(
    REQUIRES 6.8
)

# ---------------------------------------------------------
# MAVLink
# ---------------------------------------------------------

add_library(mavlink INTERFACE)

target_include_directories(
    mavlink
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mavlink
)

target_compile_definitions(
    mavlink
    INTERFACE
        MAVLINK_USE_MESSAGE_INFO
)

# ---------------------------------------------------------
# Application sources
# ---------------------------------------------------------

set(SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(APP_SOURCES
    ${SRC_DIR}/main.cpp

    ${SRC_DIR}/application/AppController.h
    ${SRC_DIR}/application/AppController.cpp

    ${SRC_DIR}/domain/LogEntry.h
    ${SRC_DIR}/domain/LogEntry.cpp

    ${SRC_DIR}/models/RawDataModel.h
    ${SRC_DIR}/models/RawDataModel.cpp

    ${SRC_DIR}/playback/PlaybackController.h
    ${SRC_DIR}/playback/PlaybackController.cpp

    ${SRC_DIR}/infrastructure/mavlink/TlogParser.h
    ${SRC_DIR}/infrastructure/mavlink/TlogParser.cpp
)

qt_add_executable(
    appLogViewer
    ${APP_SOURCES}
)

target_include_directories(
    appLogViewer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(
    appLogViewer
    PRIVATE
        cxx_std_20
)

set_target_properties(
    appLogViewer
    PROPERTIES
        CXX_EXTENSIONS OFF
)

# ---------------------------------------------------------
# QML
# ---------------------------------------------------------

set(APP_QML_FILES
    qml/Main.qml

    qml/components/PrimaryButton.qml
    qml/components/FileSelectCard.qml
    qml/components/TabButton.qml
    qml/components/PositionInfoCard.qml
    qml/components/TimelineControl.qml
    qml/components/RawDataRow.qml

    qml/pages/FileOpenPage.qml
    qml/pages/MapPage.qml
    qml/pages/RawDataPage.qml
)

# All components are exposed at the root of the LogViewer
# QML module, regardless of their physical directory.
foreach(qml_file IN LISTS APP_QML_FILES)
    get_filename_component(qml_name "${qml_file}" NAME)

    set_source_files_properties(
        "${qml_file}"
        PROPERTIES
            QT_RESOURCE_ALIAS "${qml_name}"
    )
endforeach()

qt_add_qml_module(
    appLogViewer

    URI LogViewer
    VERSION 1.0

    QML_FILES
        ${APP_QML_FILES}
)

# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------

target_link_libraries(
    appLogViewer
    PRIVATE
        Qt6::Quick
        Qt6::Location
        Qt6::Positioning
        mavlink
)

# ---------------------------------------------------------
# Platform / deployment
# ---------------------------------------------------------

set_target_properties(
    appLogViewer
    PROPERTIES
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE

        MACOSX_BUNDLE_BUNDLE_VERSION
            ${PROJECT_VERSION}

        MACOSX_BUNDLE_SHORT_VERSION_STRING
            ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

include(GNUInstallDirs)

install(
    TARGETS appLogViewer

    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)