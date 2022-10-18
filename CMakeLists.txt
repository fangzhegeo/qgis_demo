cmake_minimum_required(VERSION 3.23)
project(qgis_demo_demo)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 将这里设置为OSGeo4W64的安装路径
set(OSGEO4W_PATH "D:/GIS/OSGeo4W")

set(QT5_DIR "${OSGEO4W_PATH}/apps/Qt5")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Xml
        REQUIRED)

add_executable(qgis_demo WIN32
        main.cpp
        mainwindow.cpp
        mainwindow.h
        ${ui_files} ${qrc_files} ${moc_files})

target_link_libraries(qgis_demo
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Xml
        )

set(CMAKE_MODULE_PATH ${OSGEO4W_PATH}/apps/qgis)

#ui files
qt5_wrap_ui(ui_FILES xxx.ui)

qt5_add_resources(qrc_FILES xxx.qrc)

find_package(QGIS COMPONENTS
        core
        gui
        REQUIRED)

target_include_directories(
        qgis_demo
        PUBLIC
        ${QGIS_INCLUDE_DIR}
        ${CMAKE_PREFIX_PATH}/include/QtXml
        ${CMAKE_PREFIX_PATH}/include/QtGui
        ${CMAKE_PREFIX_PATH}/include/QtCore
        ${CMAKE_PREFIX_PATH}/include/QtWidgets
)


target_link_libraries(
        qgis_demo
        ${QGIS_CORE_LIBRARY}
        ${QGIS_GUI_LIBRARY}
)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${ui_FILES} ${qrc_FILES})

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
install(TARGETS qgis_demo DESTINATION bin)