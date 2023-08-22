# -*- coding: utf-8 -*-
# 通过对图像进行一些处理扩展数据集的数量 1->20

import os
import time
from PIL import Image,ImageChops,ImageEnhance
import numpy as np
import cv2

def image_reversal(img,savefilepath,save_filename):
    """ 图像翻转"""
    lr=img.transpose(Image.FLIP_LEFT_RIGHT) # 左右翻转
    ud=img.transpose(Image.FLIP_TOP_BOTTOM) # 上下翻转
    lr.save(os.path.join(savefilepath, save_filename[0]))
    ud.save(os.path.join(savefilepath, save_filename[1]))


def image_rotation(img,savefilepath,save_filename):
    """图像旋转"""
    out1=img.rotate(30) # 旋转20度
    out2=img.rotate(60) # 旋转30度
    out3=img.rotate(80) # 旋转30度
    out4=img.rotate(100) # 旋转30度
    out5=img.rotate(120) # 旋转30度
    out1.save(os.path.join(savefilepath, save_filename[0]))
    out2.save(os.path.join(savefilepath, save_filename[1]))
    out3.save(os.path.join(savefilepath, save_filename[2]))
    out4.save(os.path.join(savefilepath, save_filename[3]))
    out5.save(os.path.join(savefilepath, save_filename[4]))


def image_translation(img,savefilepath,save_filename):
    """图像平移"""
    out3=ImageChops.offset(img,20,0) # 只沿X轴平移
    out4=ImageChops.offset(img,0,20) # 只沿y轴平移
    out3.save(os.path.join(savefilepath, save_filename[0]))
    out4.save(os.path.join(savefilepath, save_filename[1]))


def image_brightness(img,savefilepath,save_filename):
    """亮度调整"""
    bri=ImageEnhance.Brightness(img)
    bri_img1=bri.enhance(0.8) # 小于1为减弱
    bri_img2=bri.enhance(1.2) # 大于1为增强
    bri_img1.save(os.path.join(savefilepath, save_filename[0]))
    bri_img2.save(os.path.join(savefilepath, save_filename[1]))


def image_chroma(img,savefilepath,save_filename):
    """色度调整"""
    col = ImageEnhance.Color(img)
    col_img1 = col.enhance(0.7) # 色度减弱
    col_img2 = col.enhance(1.3) # 色度增强
    col_img1.save(os.path.join(savefilepath, save_filename[0]))
    col_img2.save(os.path.join(savefilepath, save_filename[1]))


def image_contrast(img,savefilepath,save_filename):
    """对比度调整"""
    con=ImageEnhance.Contrast(img)
    con_img1=con.enhance(0.7) # 对比度减弱
    con_img2=con.enhance(1.3) # 对比度增强
    con_img1.save(os.path.join(savefilepath, save_filename[0]))
    con_img2.save(os.path.join(savefilepath, save_filename[1]))



def image_sharpness(img,savefilepath,save_filename):
    """锐度调整"""
    sha = ImageEnhance.Sharpness(img)
    sha_img1 = sha.enhance(0.5) # 锐度减弱
    sha_img2 = sha.enhance(1.5) # 锐度增强
    sha_img1.save(os.path.join(savefilepath, save_filename[0]))
    sha_img2.save(os.path.join(savefilepath, save_filename[1]))
    
def image_zoom(img,savefilepath,save_filename):
    """图像缩放"""
    zoom_img = cv2.imread(img)
    save1 = cv2.resize(zoom_img, (0, 0), fx = 0.5, fy = 0.5) # 图像长宽缩1倍
    save2 = cv2.resize(zoom_img, (0, 0), fx = 2, fy = 2) # 图像长宽缩1倍
    # 保存图片
    cv2.imwrite(os.path.join(savefilepath, save_filename[0]), save1)
    cv2.imwrite(os.path.join(savefilepath, save_filename[1]), save2)
    
def image_affine(img,savefilepath,save_filename):
    """图像仿射"""
    # 对图像进行变换（三点得到一个变换矩阵）
    # 我们知道三点确定一个平面，我们也可以通过确定三个点的关系来得到转换矩阵
    # 然后再通过warpAffine来进行变换
    affine_img = cv2.imread(img)
    rows, cols = affine_img.shape[:2]
    h, w = affine_img.shape[:2]
    point1=np.float32([[50,50],[300,50],[50,200]])
    point2=np.float32([[10,100],[300,50],[100,250]])

    M = cv2.getAffineTransform(point1, point2)
    save = cv2.warpAffine(affine_img, M, (cols,rows), borderValue=(255, 255, 255))

    # 保存图片
    cv2.imwrite(os.path.join(savefilepath, save_filename[0]), save)

# 定义扩充图片函数
def image_expansion(filepath,savefilepath):
    """
    :param filepath: 图片路径
    :param savefilepath: 扩充保存图片路径
    :param save_prefix: 图片前缀
    :return: 图片扩充数据集
    """
    i = 1
    for parent, dirnames, filenames in os.walk(filepath):
        for filename in filenames:
            image_path = os.path.join(filepath, filename)
            print('正在扩充图片：%s' %filename)
            try:
                img=Image.open(image_path)
                if img.mode == "P":
                    img = img.convert('RGB')
                image_reversal(img,savefilepath,save_filename=[str(i) + '.jpg',str(i + 1) + '.jpg'])
                i += 2
                image_rotation(img,savefilepath,save_filename=[str(i)+'.jpg', str(i+1)+'.jpg', str(i+2)+'.jpg', str(i+3)+'.jpg', str(i+4)+'.jpg'])
                i += 5
                image_translation(img,savefilepath,save_filename=[str(i) + '.jpg',str(i + 1) + '.jpg'])
                i += 2
                image_brightness(img,savefilepath,save_filename=[str(i) + '.jpg',str(i + 1) + '.jpg'])
                i += 2
                image_chroma(img,savefilepath,save_filename=[str(i) + '.jpg',str(i + 1) + '.jpg'])
                i += 2
                image_contrast(img,savefilepath,save_filename=[str(i) + '.jpg',str(i + 1) + '.jpg'])
                i += 2
                image_sharpness(img,savefilepath,save_filename=[str(i) + '.jpg',str(i + 1) + '.jpg'])
                i += 2
                image_zoom(image_path,savefilepath,save_filename=[str(i) + '.jpg',str(i + 1) + '.jpg'])
                i += 2
                image_affine(image_path,savefilepath,save_filename=[str(i)+'.jpg'])

            except Exception as e:
                print(e)
                pass



if __name__ == '__main__':
    # 设置图片路径
    filepath = r'C:\Users\KL192\Desktop\LABEL200_408\images'

    # 设置扩充保存图片路径
    savefilepath = r'C:\Users\KL192\Desktop\LABEL200_408\expend'

    time1 = time.time()
    image_expansion(filepath, savefilepath)
    time2 = time.time()
    print('总共耗时：' + str(time2 - time1) + 's')
