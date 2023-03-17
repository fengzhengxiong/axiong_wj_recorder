python中用cv保存图片：

```
for i in range(8):
    pix=self.imgWgtList[i].pixmap
    if pix is not None:
        child_dir = 'save_pic_%d' % (i + 1)
        save_path = osp.join(self.fileDir,child_dir)
        if not osp.exists(save_path):
            os.makedirs(save_path)
        csv_name=self.fileName.split('\\')[-1]
        csv_name=csv_name.replace('pcd','png')
        save_path=osp.join(save_path,csv_name)
        img=pix.toImage()
        size = img.size()
        s = img.bits().asstring(size.width() * size.height() * img.depth() // 8)  # format 0xffRRGGBB
        arr = np.fromstring(s, dtype=np.uint8).reshape((size.height(), size.width(), img.depth() // 8))
        cv2.imwrite(save_path,arr)
```

