'''
Descripttion: 划分自己的数据集文件夹
'''
import os, shutil, random
from tqdm import tqdm
import argparse

def split_img(img_path, label_path, split_list):
    # 创建数据集文件夹
    Data = '../DataSet'
    if os.path.exists(Data) == False:
        os.mkdir(Data)

    train_img_dir = Data + '/images/train'
    val_img_dir = Data + '/images/val'
    test_img_dir = Data + '/images/test'

    train_label_dir = Data + '/labels/train'
    val_label_dir = Data + '/labels/val'
    test_label_dir = Data + '/labels/test'

    # 创建文件夹
    if os.path.exists(train_img_dir) == False:
        os.makedirs(train_img_dir)
    if os.path.exists(train_label_dir) == False:
        os.makedirs(train_label_dir)
    if os.path.exists(val_img_dir) == False:
        os.makedirs(val_img_dir)
    if os.path.exists(val_label_dir) == False:
        os.makedirs(val_label_dir)
    if os.path.exists(test_img_dir) == False:
        os.makedirs(test_img_dir)
    if os.path.exists(test_label_dir) == False:
        os.makedirs(test_label_dir)
    
    train, val, test = split_list
    all_img = os.listdir(img_path)
    # 获取所有的图片路径
    all_img_path = [os.path.join(img_path, img) for img in all_img]
    # 获取train分到的图片
    train_img = random.sample(all_img_path, int(train * len(all_img_path)))
    # 拷贝的train图片路径
    train_img_copy = [os.path.join(train_img_dir, img.split('\\')[-1]) for img in train_img]
    # 获取train分到的标签
    train_label = [toLabelPath(img, label_path) for img in train_img]
    # 拷贝的train标签路径
    train_label_copy = [os.path.join(train_label_dir, label.split('\\')[-1]) for label in train_label]
    #拷贝图片和标签到train目录下
    for i in tqdm(range(len(train_img)), desc='train ', ncols=80, unit='img'):
        _copy(train_img[i], train_img_copy[i])
        _copy(train_label[i], train_label_copy[i])
        all_img_path.remove(train_img[i])
        
        
    val_img = random.sample(all_img_path, int(val / (val + test) * len(all_img_path)))
    val_label = [toLabelPath(img, label_path) for img in val_img]
    val_img_copy = [os.path.join(val_img_dir, img.split('\\')[-1]) for img in val_img]
    val_label_copy = [os.path.join(val_label_dir, label.split('\\')[-1]) for label in val_label]
    #拷贝文件到val目录下
    for i in tqdm(range(len(val_img)), desc='val ', ncols=80, unit='img'):
        _copy(val_img[i], val_img_copy[i])
        _copy(val_label[i], val_label_copy[i])
        all_img_path.remove(val_img[i])
        
    test_img = all_img_path
    test_label = [toLabelPath(img, label_path) for img in test_img]
    test_img_copy = [os.path.join(test_img_dir, img.split('\\')[-1]) for img in test_img]
    test_label_copy = [os.path.join(test_label_dir, label.split('\\')[-1]) for label in test_label]
    #拷贝文件到test目录下
    for i in tqdm(range(len(test_img)), desc='test ', ncols=80, unit='img'):
        _copy(test_img[i], test_img_copy[i])
        _copy(test_label[i], test_label_copy[i])



def _copy(from_path, to_path):
    #拷贝之前先删除
    if os.path.exists(to_path):
        os.remove(to_path)
    shutil.copy(from_path, to_path)

def toLabelPath(img_path, label_path):
    img = img_path.split('\\')[-1]
    label = img.split('.jpg')[0] + '.txt'
    return os.path.join(label_path, label)

def parse_opt():
    parser = argparse.ArgumentParser()
    parser.add_argument('--img',
                        type=str,
                        default='',
                        help='数据集图片路径')
    parser.add_argument('--label', type=str, default='', help='标签路径')
    opt = parser.parse_args()
    return opt

def main(opt):
    img_path = opt.img
    label_path = opt.label
    #img_path = r'C:\Users\KL192\Desktop\LABEL200_408\images'
    #label_path = r'C:\Users\KL192\Desktop\LABEL200_408\labels'
    print('img_path:', img_path, 'label_path:', label_path)
    split_list = [0.7, 0.2, 0.1]	# 数据集划分比例[train:val:test]
    split_img(img_path, label_path, split_list)

if __name__ == '__main__':
    opt = parse_opt()
    main(opt)