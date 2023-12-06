cmake_minimum_required(VERSION 3.16)

project(FilesWordRating VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    wordcount.h
    fileworker.h
    fileworker.cpp
    wordcount.cpp
    main.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/boost)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup()

qt_add_executable(appFilesWordRating
    ${PROJECT_SOURCES}
)

qt_add_qml_module(appFilesWordRating
    URI FilesWordRating
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES wordcount.h wordcount.cpp
    SOURCES fileworker.h fileworker.cpp
    SOURCES executor.h executor.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appFilesWordRating PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appFilesWordRating
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appFilesWordRating PRIVATE
    Qt6::Quick
    Qt6::Widgets
    Boost::multi_index
    Boost::container
)

include(GNUInstallDirs)
install(TARGETS appFilesWordRating
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
