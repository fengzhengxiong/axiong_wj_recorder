pyqt5工作笔记：编辑区域or属性区域常用模板

这两个例子使用于很多工程里的属性编辑区。

demo1：author：axiong

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-


from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import *


def fill_grid_layout(lay, wids):
    """
    填充网格布局
    :param lay: QGridLayout
    :param wids: 二维列表，元素为元组=(控件，占行，占列)
    :return:
    """
    for i in range(len(wids)):
        for j in range(len(wids[i])):
            # 第i行 j 列
            w = wids[i][j]
            if w is None:
                continue
            wid, a, b = w
            lay.addWidget(wid, i, j, a, b)


class AdditionWorkView(QtWidgets.QWidget):
    """
    按钮控制区
    """
    def __init__(self, parent=None):
        super(AdditionWorkView, self).__init__()
        self._parent = parent
        self.__init_ui()

    def __init_ui(self):
        if self._parent:
            self.setParent(self._parent)

        # 核心按钮-------------------
        self.lbl_name0=QLabel('选中颜色: ')
        self.lbl_name1=QLabel('单独改变地图颜色:  ')
        self.lbl_name2 = QLabel('取消单独改变地图颜色:  ')

        self.btn_color_change_ok = QPushButton('ok')
        self.btn_color_change_esc = QPushButton('esc')

        self.combobox=QComboBox(self)
        self.model=QtGui.QStandardItemModel()
        self.combobox.setModel(self.model)
        self.strlist=['普通道路','路口','环岛',]
        self.colorlist=[[10,240,10],[255,255,10],[65,105,200]]
        for i in range(len(self.colorlist)):
            self.combobox.addItem(self.strlist[i])
            c=self.colorlist[i]
            self.model.item(i).setBackground(QColor(c[0],c[1],c[2]))
        # 布局--------------------------
        gridLayout=QGridLayout(self)
        gridLayout.setContentsMargins(1,1,1,1)
        gridLayout.setHorizontalSpacing(2)
        gridLayout.setVerticalSpacing(2)
        gridLayout.setObjectName("gridLayout")
        grid_wids=[
            [(self.lbl_name1,1,1),(self.combobox,1,1),(self.btn_color_change_ok,1,1)],
            [(self.lbl_name2,1,1),None,(self.btn_color_change_esc,1,1)]
        ]
        # 控件填充------------------------
        fill_grid_layout(gridLayout,grid_wids)

if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    _view = AdditionWorkView()
    _view.show()
    sys.exit(app.exec_())

```

用到的控件主要还是Qlabel和Qpushbutton

除此之外，还用到了QCombobox控件，它是一个下滑框，填充内部item时还会设置背景色

此demo比较有用的地方：QCombobox控件使用，fill_grid_layout函数使用



#----------------------------------------------------------------------------------------------------------------

demo2：author：anhaiyang

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-


from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtCore import pyqtSignal, Qt, QRegExp
from PyQt5.QtGui import QIntValidator, QDoubleValidator, QRegExpValidator, QColor

class EditWidget_(QtWidgets.QWidget):
    """
    自定义组合控件
    """
    " 按钮触发 ，LINE 字符串  COMBO 发当前index  SPIN 发出值"
    click_signal = pyqtSignal(list)

    def __init__(self, label='title', mode='QSpinBox', parent=None, user_data=None):
        super(EditWidget_, self).__init__()

        if parent:
            self._parent = parent
            self.setParent(self._parent)

        self.data = user_data  # 控件存储的数据

        wid_dic = {
            'QLineEdit': QLineEdit(),
            'QComboBox': QComboBox(),
            'QSpinBox': QSpinBox(),
            'QDoubleSpinBox': QDoubleSpinBox(),
        }
        self._mode = mode

        self.label = QLabel(self)
        self.label.setText(label)
        self.edit = wid_dic.get(self._mode, QLineEdit())
        self.edit.setParent(self)

        self.button = QPushButton(self)
        self.button.setText('确认')

        self.__init_ui()
        self.__init_connect()


    def __init_ui(self):
        # 布局
        self.lay = QHBoxLayout(self)
        self.lay.setContentsMargins(2, 2, 2, 2)
        self.lay.setSpacing(6)

        self.lay.addWidget(self.label)
        self.lay.addWidget(self.edit)
        self.lay.addWidget(self.button)
        self.lay.setStretch(0, 3)
        self.lay.setStretch(1, 10)
        self.lay.setStretch(2, 1)

    def __init_connect(self):
        self.button.clicked.connect(self.button_click)

    def button_click(self):
        """
        按钮触发
        :return:
        """
        res = []
        if isinstance(self.edit, QLineEdit):
            res = [self.edit.text()]
        elif isinstance(self.edit, QComboBox):
            res = [self.edit.currentData(Qt.UserRole)]
        elif isinstance(self.edit, (QSpinBox, QDoubleSpinBox)):
            res = [self.edit.value()]
        res = [self] + res
        print('res = ', res)
        self.click_signal.emit(res)

    def get_mode(self):
        return self._mode

    def set_label(self, text):
        self.label.setText(str(text))

    def get_label(self):
        return self.label.text()

    def set_user_data(self, user_data):
        self.data = user_data

    def get_user_data(self):
        return self.data

    def set_lineedit_number(self):
        """
        只允许输入数字
        :return:
        """
        if not isinstance(self.edit, QLineEdit):
            return

        self.edit.setValidator(QRegExpValidator(QRegExp("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)"), self))

    def set_lineedit_attr(self, isInt, range=(0, 99)):
        """ 设置输入控件 输入项"""
        if not isinstance(self.edit, QLineEdit):
            return

        if isInt:
            intValidator = QIntValidator()
            intValidator.setRange(range[0], range[1])
            self.edit.setValidator(intValidator)

        else:
            doubleValidator = QDoubleValidator()
            doubleValidator.setRange(range[0], range[1])
            doubleValidator.setDecimals(3)
            doubleValidator.setNotation(QDoubleValidator.StandardNotation)

            self.edit.setValidator(doubleValidator)

        self.edit.update()

    def set_spin_attr(self, step=1, range=(0, 99)):
        """设置spin 输入属性"""
        if isinstance(self.edit, QDoubleSpinBox):
            self.edit.setDecimals(2)
            self.edit.setSingleStep(step)
            self.edit.setRange(range[0], range[1])

        elif isinstance(self.edit, QSpinBox):
            try:
                _step = max(1, int(step))
            except Exception:
                _step = 1

            self.edit.setSingleStep(_step)
            self.edit.setRange(range[0], range[1])

    def set_combo_attr(self, strlist, datalist=None, colorlist=None):
        """
        设置下拉框内容
        :param strlist:
        :return:
        """

        if isinstance(self.edit, QComboBox):
            model = QtGui.QStandardItemModel()
            self.edit.setModel(model)
            for i in range(len(strlist)):
                self.edit.addItem(strlist[i])
                if datalist:
                    model.item(i).setData(datalist[i], Qt.UserRole)
                if colorlist:
                    c = colorlist[i]
                    model.item(i).setBackground(QColor(c[0], c[1], c[2]))


if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    _view = EditWidget_(label='axiong',mode='QComboBox')
    _view.set_combo_attr(strlist=['a', 'b', 'c'],colorlist=[[0,0,255],[0,255,0],[255,0,0]])
    _view.show()
    sys.exit(app.exec_())
```



```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-


from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtCore import pyqtSignal, Qt, QRegExp
from PyQt5.QtGui import QIntValidator, QDoubleValidator, QRegExpValidator, QColor

# ------------------------------------------------------
# 自定义控件
class EditWidget_(QtWidgets.QWidget):
    """
    自定义组合控件
    """
    " 按钮触发 ，LINE 字符串  COMBO 发当前index  SPIN 发出值"
    click_signal = pyqtSignal(list)

    def __init__(self, label='title', mode='QSpinBox', parent=None, user_data=None):
        super(EditWidget_, self).__init__()

        if parent:
            self._parent = parent
            self.setParent(self._parent)

        self.data = user_data  # 控件存储的数据

        wid_dic = {
            'QLineEdit': QLineEdit(),
            'QComboBox': QComboBox(),
            'QSpinBox': QSpinBox(),
            'QDoubleSpinBox': QDoubleSpinBox(),
        }
        self._mode = mode

        self.label = QLabel(self)
        self.label.setText(label)
        self.edit = wid_dic.get(self._mode, QLineEdit())
        self.edit.setParent(self)

        self.button = QPushButton(self)
        self.button.setText('确认')

        self.__init_ui()
        self.__init_connect()


    def __init_ui(self):
        # 布局
        self.lay = QHBoxLayout(self)
        self.lay.setContentsMargins(2, 2, 2, 2)
        self.lay.setSpacing(6)

        self.lay.addWidget(self.label)
        self.lay.addWidget(self.edit)
        self.lay.addWidget(self.button)
        self.lay.setStretch(0, 3)
        self.lay.setStretch(1, 10)
        self.lay.setStretch(2, 1)

    def __init_connect(self):
        self.button.clicked.connect(self.button_click)

    def button_click(self):
        """
        按钮触发
        :return:
        """
        res = []
        if isinstance(self.edit, QLineEdit):
            res = [self.edit.text()]
        elif isinstance(self.edit, QComboBox):
            res = [self.edit.currentData(Qt.UserRole)]
        elif isinstance(self.edit, (QSpinBox, QDoubleSpinBox)):
            res = [self.edit.value()]
        res = [self] + res
        print('res = ', res)
        self.click_signal.emit(res)

    def get_mode(self):
        return self._mode

    def set_label(self, text):
        self.label.setText(str(text))

    def get_label(self):
        return self.label.text()

    def set_user_data(self, user_data):
        self.data = user_data

    def get_user_data(self):
        return self.data

    def set_lineedit_number(self):
        """
        只允许输入数字
        :return:
        """
        if not isinstance(self.edit, QLineEdit):
            return

        self.edit.setValidator(QRegExpValidator(QRegExp("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)"), self))

    def set_lineedit_attr(self, isInt, range=(0, 99)):
        """ 设置输入控件 输入项"""
        if not isinstance(self.edit, QLineEdit):
            return

        if isInt:
            intValidator = QIntValidator()
            intValidator.setRange(range[0], range[1])
            self.edit.setValidator(intValidator)

        else:
            doubleValidator = QDoubleValidator()
            doubleValidator.setRange(range[0], range[1])
            doubleValidator.setDecimals(3)
            doubleValidator.setNotation(QDoubleValidator.StandardNotation)

            self.edit.setValidator(doubleValidator)

        self.edit.update()

    def set_spin_attr(self, step=1, range=(0, 99)):
        """设置spin 输入属性"""
        if isinstance(self.edit, QDoubleSpinBox):
            self.edit.setDecimals(2)
            self.edit.setSingleStep(step)
            self.edit.setRange(range[0], range[1])

        elif isinstance(self.edit, QSpinBox):
            try:
                _step = max(1, int(step))
            except Exception:
                _step = 1

            self.edit.setSingleStep(_step)
            self.edit.setRange(range[0], range[1])

    def set_combo_attr(self, strlist, datalist=None, colorlist=None):
        """
        设置下拉框内容
        :param strlist:
        :return:
        """

        if isinstance(self.edit, QComboBox):
            model = QtGui.QStandardItemModel()
            self.edit.setModel(model)
            for i in range(len(strlist)):
                self.edit.addItem(strlist[i])
                if datalist:
                    model.item(i).setData(datalist[i], Qt.UserRole)
                if colorlist:
                    c = colorlist[i]
                    model.item(i).setBackground(QColor(c[0], c[1], c[2]))

# ------------------------------------------------------
# 只是new一个Qwidget界面，方便下一步整合
class ParaEditAreaView(QtWidgets.QWidget):
    """
    编辑区
    """
    def __init__(self, parent=None):
        super(ParaEditAreaView, self).__init__()
        self._parent = parent
        self.__init_ui()

    def __init_ui(self):
        if self._parent:
            self.setParent(self._parent)
        # 布局 ------------------
        self.wid_ = QWidget()
        self.scrollArea = QScrollArea(self)
        self.scrollArea.setWidgetResizable(True)  # 设置为true，则滚动区域部件将自动调整，以避免可以不显示的滚动条，或者利用额外的空间；
        self.scrollArea.setStyleSheet("QScrollArea{border:none; background:white;}")
        self.scrollArea.setWidget(self.wid_)

        lay = QHBoxLayout(self)
        lay.addWidget(self.scrollArea)
        lay.setContentsMargins(0, 0, 0, 0)

        self.vbox = QVBoxLayout(self.wid_)
        self.vbox.setContentsMargins(2, 2, 2, 2)
        self.vbox.setSpacing(3)

# ------------------------------------------------------
# 开始实例化整个大的界面
import copy
import numpy as np
from PyQt5.QtWidgets import QApplication

tra_property_dic = {
    1:  {"des": "普通道路", "rgb": (10, 240, 10)},
    2:  {"des": "路口", "rgb": (255, 255, 10)},
    3:  {"des": "环岛", "rgb": (65, 105, 200)},
    4:  {"des": "匝道", "rgb": (10, 250, 230)},
    5:  {"des": "水平泊车", "rgb": (127, 250, 200)},
    6:  {"des": "垂直泊车", "rgb": (160, 32, 230)},
    7:  {"des": "隧道", "rgb": (244, 164, 98)},
    8:  {"des": "路口中转", "rgb": (255, 0, 10)},
    9:  {"des": "路口后段", "rgb": (255, 97, 3)},
    10: {"des": "预留", "rgb": (112, 128, 100)},
}

tra_lane_dic = {
    1:  {"des": "单行道"},
    2:  {"des": "双车道右侧"},
    3:  {"des": "三车道"},
    4:  {"des": "双车道左侧"},
    5:  {"des": "借道"},
}

merge_dic = {
    1:  {"des": "默认"},
    2:  {"des": "切换SLAM"},
    3:  {"des": "左并道"},
    4:  {"des": "右并道"},
    5:  {"des": "结束并道"},
}

road_state_dic = {
    1:  {"des": "正常道路"},
    2:  {"des": "颠簸道路"},
    3:  {"des": "闸机口"},
}

turn_light_dic = {
    0:  {"des": "无需打灯"},
    1:  {"des": "左转灯"},
    2:  {"des": "右转灯"},
    3:  {"des": "双闪"},
}

map_txt_dic = {
    "property":     {"ch": "地图属性", "No": 2, "describe": "", "val": tra_property_dic},
    "speed":        {"ch": "设定速度", "No": 3, "describe": "", "val": None},
    "laneState":    {"ch": "车道状态", "No": 4, "describe": "", "val": tra_lane_dic},
    "merge":        {"ch": "并道属性", "No": 5, "describe": "", "val": merge_dic},
    "road":         {"ch": "道路状态", "No": 6, "describe": "", "val": road_state_dic},
    "turnLight":    {"ch": "转向灯状态", "No": 7, "describe": "", "val": turn_light_dic},
    "rightOffset":    {"ch": "右侧临时停车偏移距离", "No": 8, "describe": "", "val": None},
    "thisWidth":    {"ch": "自车道宽度", "No": 9, "describe": "", "val": None},
    "leftWidth":    {"ch": "左车道宽度", "No": 10, "describe": "", "val": None},
    "rightWidth":    {"ch": "右车道宽度", "No": 11, "describe": "", "val": None},
    "leftSafeDis":    {"ch": "轨迹左侧安全距离", "No": 12, "describe": "", "val": None},
    "rightSafeDis":    {"ch": "轨迹右侧安全距离", "No": 13, "describe": "", "val": None},
    "yaw":          {"ch": "航向角", "No": 14, "describe": "", "val": float()},
    "gpsTime":      {"ch": "GPS时间", "No": 15, "describe": "", "val": int()},
    "satelliteCnt":    {"ch": "卫星个数", "No": 16, "describe": "", "val": np.array([1., 32.3])},
}

map_key_list = [
    "property",
    "speed",
    "laneState",
    "merge",
    "road",
    "turnLight",
    "rightOffset",
    "thisWidth",
    "leftWidth",
    "rightWidth",
    "leftSafeDis",
    "rightSafeDis",
    "yaw",
    "gpsTime",
    "satelliteCnt",
]

class ParaEditAreaController(object):
    def __init__(self, view=None, model=None, *args, **kwargs):
        super(ParaEditAreaController, self).__init__()
        if view is not None:
            self._view = view
        else:
            self._view = ParaEditAreaView()
        try:
            self.init_view()
            pass
        except Exception as e:
            print(e)

    def init_view(self):
        """
        在原有视图基础上进行编辑
        :return:
        """
        _map_dic = copy.deepcopy(map_txt_dic)
        _map_list = copy.deepcopy(map_key_list)  # python 2.7 ,for order search

        # 创建属性列表
        self.wid_list = []

        for k in _map_list:
            v = _map_dic[k]
            label = v["ch"]
            if isinstance(v["val"], dict):
                mode = 'QComboBox'
                ew = EditWidget_(label, mode, parent=self._view.wid_)  # 加上parent 按钮显示正常
                strlist = []
                datalist = []
                colorlist = []
                for a, b in v["val"].items():  #   1:  {"des": "普通道路", "rgb": (10, 240, 10)},
                    strlist.append("%d - %s" % (a, b["des"]))
                    datalist.append(a)
                    if "rgb" in b:
                        colorlist.append(b["rgb"])
                ew.set_combo_attr(strlist, datalist, colorlist)

            else:
                mode = "QLineEdit"
                ew = EditWidget_(label, mode, parent=self._view.wid_)  # 加上parent 按钮显示正常
                ew.set_lineedit_number()

            ew.set_user_data(v["No"])  # 把当前项在txt所属列号，保存下来。
            self.wid_list.append(ew)

        for wid in self.wid_list:
            self._view.vbox.addWidget(wid)



    def __init_connect(self):

        pass

    def set_view(self, view):
        if isinstance(view, ParaEditAreaView):
            self._view = view

    def get_view(self):
        return self._view

    def set_model(self, model):
        pass


    def run(self):
        self._app = QApplication(sys.argv)
        self._view.show()
        return self._app.exec_()

    def show(self):
        self._view.show()


if __name__ == "__main__":
    import sys

    app = QApplication(sys.argv)
    _view = ParaEditAreaView()
    c = ParaEditAreaController(_view)
    c.show()
    sys.exit(app.exec_())
```

