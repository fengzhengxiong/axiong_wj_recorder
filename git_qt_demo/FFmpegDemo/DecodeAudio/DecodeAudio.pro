#---------------------------------------------------------------------------------------
# @功能：      使用libavcodec API的音频解码示例
#             1.将.mp3文件解码转换为.pcm文件；（PCM数据时最原始的音频数据）
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-10-23 19:27:48
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#  定义程序版本号
VERSION = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
TARGET  = DecodeAudio

contains(QT_ARCH, i386){        # 使用32位编译器
DESTDIR = $$PWD/../bin          # 程序输出路径
}else{
DESTDIR = $$PWD/../bin64        # 使用64位编译器
}

# msvc  编译器使用utf-8编码
msvc {
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
}

# 加载库，ffmpeg n5.1.2版本
win32{
LIBS += -LE:/lib/ffmpeg5-1-2/lib/ -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += E:/lib/ffmpeg5-1-2/include
DEPENDPATH += E:/lib/ffmpeg5-1-2/include
}

unix:!macx{
LIBS += -L/home/mhf/lib/ffmpeg/ffmpeg-5-1-2/lib -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
INCLUDEPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
DEPENDPATH += /home/mhf/lib/ffmpeg/ffmpeg-5-1-2/include
}
