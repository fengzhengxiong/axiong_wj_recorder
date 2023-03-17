#---------------------------------------------------------------------------------------
# @功能：       ffmpeg音视频开发Demo
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-09-15 14:30:43
# @备注       本项目所有Demo都是基于 ffmpeg n5.1.2版本进行开发
#---------------------------------------------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += VideoPlay       # 使用软解码实现的视频播放器
SUBDIRS += VideoPlayGL1    # 使用软解码实现的视频播放器 使用OpenGL显示RGB图像
SUBDIRS += VideoPlayGL2    # 使用软解码实现的视频播放器 使用OpenGL显示YUV图像
SUBDIRS += VideoPlayHW     # 使用硬件解码实现的视频播放器
SUBDIRS += VideoPlayHWGL   # 使用硬件解码实现的视频播放器 使用OpenGL显示YUV/NV12图像
SUBDIRS += VideoPlaySave   # 使用软解码实现的视频播放器，并将裸流保存到视频文件中（无需编码）
SUBDIRS += VideoCamera1    # FFmpeg打开本地摄像头简单示例（软解码+OpenGL）
SUBDIRS += VideoCamera2    # FFmpeg打开本地摄像头【录制视频】保存到本地示例（软解码+OpenGL）
SUBDIRS += Screencap       # FFmpeg实现录屏功能

SUBDIRS += AVIOReading     # 使用libavformat解复用器通过自定义AVIOContext读取回调访问媒体内容。
SUBDIRS += DecodeAudio     # 使用libavcodec API的音频解码示例（MP3转pcm）
