#---------------------------------------------------------------------------------------
# @功能：       使用QSqlite数据库实现用户登录、后台管理用户功能
#              1、通过按键新建 空白数据行；
#              2、使用自增Key；
#              3、通过按键更新数据；
#              4、判断表是否存在，不存在则创建。
#              5、用户登录功能，默认创建超级管理员账号root
#              6、支持用户后台管理，通过后台创建、修改、删除用户
#              7、用户分为超级管理员、普通管理员、普通用户三个等级；
#              8、超级管理员有所有权限，可创建、修改、删除普通管理员、普通用户；
#              9、普通管理员可创建、修改、删除普通用户，可新建、修改、查询数据；
#              10、普通用户不可修改用户信息，不可新建、修改数据库信息，只可查询。
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-10-07 11:29:40
# @备注
#---------------------------------------------------------------------------------------
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    dialog.cpp \
    main.cpp \
    userbackstage.cpp \
    widget.cpp

HEADERS += \
    dialog.h \
    userbackstage.h \
    widget.h

FORMS += \
    dialog.ui \
    userbackstage.ui \
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
