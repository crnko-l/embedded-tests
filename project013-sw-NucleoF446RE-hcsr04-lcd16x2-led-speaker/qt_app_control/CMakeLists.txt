cmake_minimum_required(VERSION 3.16)
project(STM32QuickSerial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find Qt packages ---
find_package(Qt6 REQUIRED COMPONENTS Quick SerialPort)

qt_standard_project_setup()

# --- Define executable target ---
qt_add_executable(appSTM32QuickSerial
    main.cpp
    serialmanager.cpp
    serialmanager.h
)

# --- QML module ---
qt_add_qml_module(appSTM32QuickSerial
    URI STM32QuickSerial
    VERSION 1.0
    QML_FILES
        Main.qml
)

# --- Link Qt libraries ---
target_link_libraries(appSTM32QuickSerial
    PRIVATE Qt6::Quick Qt6::SerialPort
)
