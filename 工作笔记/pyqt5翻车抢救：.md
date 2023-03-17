pyqt5翻车抢救：


![image-20230216151450602](C:\Users\wanji\AppData\Roaming\Typora\typora-user-images\image-20230216151450602.png)

加了这个closeEvent就可以了

```
def closeEvent(self, event):
    self.vtkWidget.Finalize()
    super(VTK_View, self).closeEvent(event)
```

是线程问题，关闭主线程的时候有些子界面ui没关掉



