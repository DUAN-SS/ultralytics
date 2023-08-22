# encoding:utf-8
import os
from PIL import Image
path = r"E:\project\mydata\trains\images_backup"
# 该文件夹下所有的文件（包括文件夹）
filelist = os.listdir(path)
count = 1
for file in filelist:
    print(file)
for file in filelist:   # 遍历所有文件
    Olddir = os.path.join(path, file)   # 原来的文件路径
    # 如果是文件夹则跳过
    if os.path.isdir(Olddir):
        continue
    # 文件名
    str = file.rsplit(".", 1)
    # 转换后的格式
    output = str[0] + ".jpg"
    print(output)
    im = Image.open(Olddir)
    im = im.convert("RGB")
    # 保存图片
    im.save(output)