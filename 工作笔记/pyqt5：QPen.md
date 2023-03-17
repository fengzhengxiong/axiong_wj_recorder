pyqt5：QPen

[(48条消息) 实战PyQt5: 115-画笔类QPen_seniorwizard的博客-CSDN博客_setbrush](https://blog.csdn.net/seniorwizard/article/details/111199575)

import sys, math
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt, QPoint, QPointF
from PyQt5.QtGui import QColor,QPen, QPainter, QPainterPath, QPolygonF
from PyQt5.QtWidgets import (QApplication, QWidget, QHBoxLayout, QFormLayout, 
                             QLabel, QFrame, QSizePolicy, QSpinBox, QPushButton,
                             QColorDialog, QComboBox)

class MyFrame(QFrame):
    def __init__(self, parent = None):
        super(MyFrame, self).__init__(parent)
        self.setFrameShape(QFrame.Box)
        self.setFrameShadow(QFrame.Plain)
        self.setLineWidth(1)
        self.setMidLineWidth(0)
        self.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        
        self.penColor = Qt.black
        self.lineWidth = 4
        self.penStyle = Qt.SolidLine
        self.capStyle = Qt.FlatCap
        self.joinStyle = Qt.MiterJoin
        
    def setPenColor(self, color):
        self.penColor = color
        self.update()
        
    def setLineWidth(self, width):
        self.lineWidth = width
        self.update()
        
    def setPenStyle(self, style):
        self.penStyle = style
        self.update()
        
    def setCapStyle(self, style):
        self.capStyle = style
        self.update()
        
    def setJoinStyle(self, style):
        self.joinStyle = style
        self.update()
        
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing, True)
        
        #绘制边框线
        painter.drawRect(self.rect())
        
        pen = QPen()
        pen.setColor(self.penColor)
        pen.setWidth(self.lineWidth)
        pen.setStyle(self.penStyle)
        pen.setCapStyle(self.capStyle)
        pen.setJoinStyle(self.joinStyle)
        
        painter.setPen(pen)
        
        #绘制曲线
        start_pos = QPoint(400, 20)
        end_pos = QPoint(40, 200)   
        path = QPainterPath(start_pos)
        
        c1 = QPoint((start_pos.x() + end_pos.x()) // 2, start_pos.y())
        c2 = QPoint((start_pos.x() + end_pos.x()) // 2, end_pos.y())
        path.cubicTo(c1, c2, end_pos)
        painter.drawPath(path)
        
        #绘制五角星
        #添加一个多边形(五角星)
        #外点：x=Rcos(72°*k)  y=Rsin(72°*k)   k=0,1,2,3,4
        #内点：r=Rsin18°/sin36°  x=rcos(72°*k+36°)  y=rsin(72°*k+36°)   k=0,1,2,3,4
        deg_18 = 18 * math.pi / 180
        deg_36 = 36 * math.pi / 180
        deg_72 = 72 * math.pi / 180
        r_out = 100 #半径
        r_inner = r_out * math.sin(deg_18) / math.sin(deg_36)
        polygon = QPolygonF()
        for i in range (5) :   
            #外点
            out_angle = deg_72 * i - deg_18   
            polygon.append(QPointF(r_out * math.cos(out_angle), r_out * math.sin(out_angle))) 
            #内点
            in_angle =  deg_72 * i + deg_18
            polygon.append(QPointF(r_inner * math.cos(in_angle), r_inner * math.sin(in_angle)))
        
        painter.translate(260, 260)
        painter.drawPolygon(polygon)        

    
    class DemoPen(QWidget):
        def __init__(self, parent=None):
            super(DemoPen, self).__init__(parent)   
            
            # 设置窗口标题
            self.setWindowTitle('实战PyQt5: QPen演示') 
            
            #设置尺寸
            self.resize(720, 400)    
                  
            self.initUi()
            
        def initUi(self):
            layout = QHBoxLayout()
            
            self.canvas = MyFrame()
            
            #线宽
            lineWidth = QSpinBox()
            lineWidth.setRange(1, 40)
            lineWidth.setValue(4)
            lineWidth.valueChanged.connect(self.canvas.setLineWidth)
            
            #颜色设置
            btnSetColor = QPushButton('设置颜色')
            btnSetColor.clicked.connect(self.onSetPenColor)
            
            #线型设置
            cmbPenStyle = QComboBox()
            cmbPenStyle.addItems(['SolidLine', 'DashLine', 'DotLine', 'DashDotLine', 'DashDotDotLine'])
            cmbPenStyle.currentTextChanged.connect(self.onPenStyleChanged)
            
            #笔帽样式设置
            cmbCapStyle = QComboBox()
            cmbCapStyle.addItems(['FlatCap', 'SquareCap', 'RoundCap'])
            cmbCapStyle.currentTextChanged.connect(self.onCapStyleChanged)
            
            #连接样式设置
            cmbJoinStyle = QComboBox()
            cmbJoinStyle.addItems(['MiterJoin','BevelJoin','RoundJoin'])
            cmbJoinStyle.currentTextChanged.connect(self.onJoinStyleChanged)
            
            fLayout = QFormLayout()
            fLayout.setContentsMargins(0,0,0,0)
            fLayout.addRow('线宽: ', lineWidth)
            fLayout.addRow('颜色: ', btnSetColor)
            fLayout.addRow('线型: ', cmbPenStyle)
            fLayout.addRow('笔帽样式: ', cmbCapStyle)
            fLayout.addRow('连接样式: ', cmbJoinStyle)
            
            wid_left = QWidget()
            wid_left.setMaximumWidth(200)
            wid_left.setLayout(fLayout)
                      
            layout.addWidget(wid_left)
            layout.addWidget(self.canvas)
            
            self.setLayout(layout)
            
        def onSetPenColor(self):
           color = QColorDialog.getColor()
           self.canvas.setPenColor(color)
           
        def onPenStyleChanged(self, style):
            if style == 'SolidLine':
                self.canvas.setPenStyle(Qt.SolidLine)
            elif style == 'DashLine':
                self.canvas.setPenStyle(Qt.DashLine)
            elif style == 'DotLine':
                self.canvas.setPenStyle(Qt.DotLine)
            elif style == 'DashDotLine':
                self.canvas.setPenStyle(Qt.DashDotLine)
            elif style == 'DashDotDotLine':
                self.canvas.setPenStyle(Qt.DashDotDotLine)
            else:
                self.canvas.setPenStyle(Qt.NoPen)
                
        def onCapStyleChanged(self, style):
            if style == 'FlatCap':
                self.canvas.setCapStyle(Qt.FlatCap)
            elif style == 'SquareCap':
                self.canvas.setCapStyle(Qt.SquareCap)
            elif style == 'RoundCap':
                self.canvas.setCapStyle(Qt.RoundCap)
            else:
                self.canvas.setCapStyle(Qt.FlatCap)
                
        def onJoinStyleChanged(self, style):
            if style == 'MiterJoin':
                self.canvas.setJoinStyle(Qt.MiterJoin)
            elif style == 'BevelJoin':
                self.canvas.setJoinStyle(Qt.BevelJoin)
            elif style == 'RoundJoin':
                self.canvas.setJoinStyle(Qt.RoundJoin)
            else:
                self.canvas.setJoinStyle(Qt.MiterJoin)
    

![](C:\Users\wanji\Desktop\axiong\b5adea44674b132b83f92b1a08e80418.gif)