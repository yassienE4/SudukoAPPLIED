QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    SudokuBoard.cpp \
    SudokuSolver.cpp \
    $$files(data-structures/*.cpp) \
    viewer.cpp

HEADERS += \
    mainwindow.h \
    player.h \
    sudokuboard.h \
    SudokuSolver.h \
    $$files(data-structures/*.h) \
    viewer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
