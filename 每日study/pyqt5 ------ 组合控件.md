pyqt5 ------ 组合控件

实现目的：如果想添加删除某个类型的相似控件，直接从外部修改字典中的数据即可。

效果实例图：

![screenshot-20230223-110805](C:\Users\wanji\Desktop\temp\screenshot-20230223-110805.png)





源码：

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtCore import pyqtSignal, Qt, QRegExp
from PyQt5.QtGui import QIntValidator, QDoubleValidator, QRegExpValidator, QColor
import numpy as np
import copy

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
    1:  {"des": "单行道", "rgb": (10, 240, 10)},
    2:  {"des": "双车道右侧", "rgb": (255, 240, 10)},
    3:  {"des": "三车道", "rgb": (10, 255, 10)},
    4:  {"des": "双车道左侧", "rgb": (10, 240, 255)},
    5:  {"des": "借道", "rgb": (112, 128, 100)},
}

merge_dic = {
    1:  {"des": "默认", "rgb": (10, 240, 10)},
    2:  {"des": "切换SLAM", "rgb": (255, 240, 10)},
    3:  {"des": "左并道", "rgb": (10, 255, 10)},
    4:  {"des": "右并道", "rgb": (10, 240, 255)},
    5:  {"des": "结束并道", "rgb": (112, 128, 100)},
}

road_state_dic = {
    1:  {"des": "正常道路", "rgb": (10, 255, 10)},
    2:  {"des": "颠簸道路", "rgb": (255, 240, 10)},
    3:  {"des": "闸机口", "rgb": (10, 240, 255)},
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

class minor_view(QtWidgets.QWidget):
    click_signal = pyqtSignal(list)

    def __init__(self,label='title',mode='QSpinBox',parent=None,user_data=None):
        super(minor_view, self).__init__()
        if parent:
            self._parent=parent
            self.setParent(self._parent)
        wid_dic={
            'QLineEdit': QLineEdit(),
            'QComboBox': QComboBox(),
            'QSpinBox': QSpinBox()
        }
        self.data=user_data
        self.mode=mode
        self.label=QLabel(self)
        self.label.setText(label)
        self.edit=wid_dic.get(self.mode,QLineEdit())
        self.edit.setParent(self)
        self.button=QPushButton(self)
        self.button.setText('确认')

        self.__init_ui()
        self.__init_connect()

    def __init_ui(self):
        self.hbox=QHBoxLayout(self)
        self.hbox.setContentsMargins(2,2,2,2)
        self.hbox.setSpacing(6)
        self.hbox.addWidget(self.label)
        self.hbox.addWidget(self.edit)
        self.hbox.addWidget(self.button)
        self.hbox.setStretch(0,3)
        self.hbox.setStretch(1, 10)
        self.hbox.setStretch(2, 1)

    def __init_connect(self):
        self.button.clicked.connect(self.button_click)

    def button_click(self):
        res=[]
        if isinstance(self.edit,QLineEdit):
            res=[self.edit.text()]
        elif isinstance(self.edit,QComboBox):
            res=[self.edit.currentData(Qt.UserRole)]
        elif isinstance(self.edit, QSpinBox):
            res = [self.edit.value()]
        res=[self]+res
        print('res= ',res)
        self.click_signal.emit(res)

    def set_combo_attr(self, strlist, datalist=None, colorlist=None):
        """
        对于ComboBox控件设置下拉框内容和颜色
        :param strlist:
        :param datalist:
        :param colorlist:
        :return:
        """
        if isinstance(self.edit,QComboBox):
            model=QtGui.QStandardItemModel()
            self.edit.setModel(model)
            for i in range(len(strlist)):
                self.edit.addItem(strlist[i])
                if datalist:
                    model.item(i).setData(datalist[i],Qt.UserRole)
                if colorlist:
                    c=colorlist[i]
                    model.item(i).setBackground(QColor(c[0],c[1],c[2]))

    def set_user_data(self,user_data):
        self.data=user_data

class main_view(QtWidgets.QWidget):
    def __init__(self,parent=None):
        super(main_view,self).__init__()
        self._parent=parent
        self.__init_ui()

    def __init_ui(self):
        if self._parent:
            self.setParent(self._parent)
        # -------------
        self.wid_=QWidget()
        self.scrollArea=QScrollArea()
        self.scrollArea.setWidgetResizable(True) # 滑动条的自动调整
        self.scrollArea.setStyleSheet("QScrollArea{border:none; background:white;}") # 滑动条白色
        self.scrollArea.setWidget(self.wid_)

        # 开始布局 ------
        hbox=QHBoxLayout(self)
        hbox.addWidget(self.scrollArea)
        hbox.setContentsMargins(0,0,0,0)

        self.vbox=QVBoxLayout(self.wid_)
        self.vbox.setContentsMargins(2,2,2,2)
        self.vbox.setSpacing(3)

class main_controll(object):
    def __init__(self,view=None,model=None,*args,**kwargs):
        super(main_controll,self).__init__()
        if view is not None:
            self._view=view
        else:
            self._view=main_view()
        try:
            self.__init_view()
        except Exception as e:
            print(e)

    def __init_view(self):
        _map_txt_dic=copy.deepcopy(map_txt_dic)

        # 创建属性列表
        self.wid_list=[]
        for key,value in _map_txt_dic.items():
            label=value["ch"]
            if isinstance(value["val"],dict):
                mode='QComboBox'  # 设置控件类型是QComboBox
                ew=minor_view(label,mode,parent=self._view.wid_)
                strlist=[]
                datalist=[]
                colorlist=[]
                for a,b in value["val"].items():
                    strlist.append("%d - %s" % (a,b["des"]))
                    datalist.append(a)
                    if "rgb" in b:
                        colorlist.append(b["rgb"])
                ew.set_combo_attr(strlist,datalist,colorlist)

            elif value["val"] is None:
                mode='QSpinBox'     # 设置控件类型是QSpinBox
                ew = minor_view(label, mode, parent=self._view.wid_)

            else:
                mode = 'QLineEdit'  # 设置控件类型是QLineEdit
                ew = minor_view(label, mode, parent=self._view.wid_)

            ew.set_user_data(value["No"])
            self.wid_list.append(ew)

        for wid in self.wid_list:
            self._view.vbox.addWidget(wid)

    def show(self):
        self._view.show()


if __name__=="__main__":
    import sys
    app=QApplication(sys.argv)
    view=main_view()
    control=main_controll(view)
    control.show()
    sys.exit(app.exec_())
```





# -------------------------------------------------------------------------------------------------------------------------------

测试实例数据：

```python
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
    1:  {"des": "单行道", "rgb": (10, 240, 10)},
    2:  {"des": "双车道右侧", "rgb": (255, 240, 10)},
    3:  {"des": "三车道", "rgb": (10, 255, 10)},
    4:  {"des": "双车道左侧", "rgb": (10, 240, 255)},
    5:  {"des": "借道", "rgb": (112, 128, 100)},
}

merge_dic = {
    1:  {"des": "默认", "rgb": (10, 240, 10)},
    2:  {"des": "切换SLAM", "rgb": (255, 240, 10)},
    3:  {"des": "左并道", "rgb": (10, 255, 10)},
    4:  {"des": "右并道", "rgb": (10, 240, 255)},
    5:  {"des": "结束并道", "rgb": (112, 128, 100)},
}

road_state_dic = {
    1:  {"des": "正常道路", "rgb": (10, 255, 10)},
    2:  {"des": "颠簸道路", "rgb": (255, 240, 10)},
    3:  {"des": "闸机口", "rgb": (10, 240, 255)},
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
```



```
'QLineEdit': QLineEdit(),      ------ else
'QComboBox': QComboBox(),      ------ value["val"]是字典类型
'QSpinBox': QSpinBox(),        ------ value["val"]是None   
```

