#---------------------------------------------------------
# 功能：       windows下串口、鼠标、键盘热插拔检测模块热插拔监测demo
# 编译器：
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/03/27
# @备注
#---------------------------------------------------------
QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comchange.cpp \
    main.cpp \
    mousekeytest.cpp \
    porttest.cpp \
    widget.cpp

HEADERS += \
    comchange.h \               # 串口热插拔监测
    mousekeytest.h \            # 鼠标、键盘热插拔监测
    porttest.h \
    widget.h

FORMS += \
    mousekeytest.ui \
    porttest.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#  定义程序版本号
VERSION = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

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
