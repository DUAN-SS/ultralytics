from pycocotools.coco import COCO
import os
import shutil
from tqdm import tqdm
import sys
import argparse
import os
import json
from lxml import etree

category_set = set()
image_set = set()
coco_bbox_nums = 0

images_nums = 0
category_nums = 0
voc_bbox_nums = 0

# 将类别名字和id建立索引
def catid2name(coco):
    classes = dict()
    for cat in coco.dataset['categories']:
        classes[cat['id']] = cat['name']
    return classes


# 将[xmin,ymin,xmax,ymax]转换为yolo格式[x_center, y_center, w, h](做归一化)
def coco_xyxy2xywhn(object, width, height):
    cat_id = object[0]
    xn = object[1] / width
    yn = object[2] / height
    wn = object[3] / width
    hn = object[4] / height
    out = "{} {:.5f} {:.5f} {:.5f} {:.5f}".format(cat_id, xn, yn, wn, hn)
    return out


def save_anno_to_txt(images_info, save_path):
    filename = images_info['filename']
    txt_name = filename[:-3] + "txt"
    with open(os.path.join(save_path, txt_name), "w") as f:
        for obj in images_info['objects']:
            line = coco_xyxy2xywhn(obj, images_info['width'], images_info['height'])
            f.write("{}\n".format(line))


# 利用cocoAPI从json中加载信息
def load_coco(anno_file, xml_save_path):
    if os.path.exists(xml_save_path):
        shutil.rmtree(xml_save_path)
    os.makedirs(xml_save_path)

    coco = COCO(anno_file)
    classes = catid2name(coco)
    imgIds = coco.getImgIds()
    classesIds = coco.getCatIds()

    with open(os.path.join(xml_save_path, "classes.txt"), 'w') as f:
        for id in classesIds:
            f.write("{}\n".format(classes[id]))

    for imgId in tqdm(imgIds):
        info = {}
        img = coco.loadImgs(imgId)[0]
        filename = img['file_name']
        width = img['width']
        height = img['height']
        info['filename'] = filename
        info['width'] = width
        info['height'] = height
        annIds = coco.getAnnIds(imgIds=img['id'], iscrowd=None)
        anns = coco.loadAnns(annIds)
        objs = []
        for ann in anns:
            object_name = classes[ann['category_id']]
            # bbox:[x,y,w,h]
            bbox = list(map(float, ann['bbox']))
            xc = bbox[0] + bbox[2] / 2.
            yc = bbox[1] + bbox[3] / 2.
            w = bbox[2]
            h = bbox[3]
            obj = [ann['category_id'], xc, yc, w, h]
            objs.append(obj)
        info['objects'] = objs
        save_anno_to_txt(info, xml_save_path)


def parseJsonFile(json_path, txt_save_path):
    assert os.path.exists(json_path), "json path:{} does not exists".format(json_path)
    if os.path.exists(txt_save_path):
        shutil.rmtree(txt_save_path)
    os.makedirs(txt_save_path)

    assert json_path.endswith('json'), "json file:{} It is not json file!".format(json_path)

    load_coco(json_path, txt_save_path)
    
def parse_xml_to_dict(xml):
    """
    将xml文件解析成字典形式，参考tensorflow的recursive_parse_xml_to_dict
    Args:
        xml: xml tree obtained by parsing XML file contents using lxml.etree

    Returns:
        Python dictionary holding XML contents.
    """
    if len(xml) == 0:  # 遍历到底层，直接返回tag对应的信息
        return {xml.tag: xml.text}

    result = {}
    for child in xml:
        child_result = parse_xml_to_dict(child)  # 递归遍历标签信息
        if child.tag != 'object':
            result[child.tag] = child_result[child.tag]
        else:
            if child.tag not in result:  # 因为object可能有多个，所以需要放入列表里
                result[child.tag] = []
            result[child.tag].append(child_result[child.tag])
    return {xml.tag: result}


def write_classIndices(category_set):
    class_indices = dict((k, v) for v, k in enumerate(category_set))
    json_str = json.dumps(dict((val, key) for key, val in class_indices.items()), indent=4)
    with open('class_indices.json', 'w') as json_file:
        json_file.write(json_str)


def voc_xyxy2xywhn(bbox, size):
    bbox = list(map(float, bbox))
    size = list(map(float, size))
    xc = (bbox[0] + (bbox[2] - bbox[0]) / 2.) / size[0]
    yc = (bbox[1] + (bbox[3] - bbox[1]) / 2.) / size[1]
    wn = (bbox[2] - bbox[0]) / size[0]
    hn = (bbox[3] - bbox[1]) / size[1]
    return (xc, yc, wn, hn)


def parser_info(info: dict, only_cat=True, class_indices=None):
    filename = info['annotation']['filename']
    image_set.add(filename)
    objects = []
    width = int(info['annotation']['size']['width'])
    height = int(info['annotation']['size']['height'])
    for obj in info['annotation']['object']:
        obj_name = obj['name']
        category_set.add(obj_name)
        if only_cat:
            continue
        xmin = int(obj['bndbox']['xmin'])
        ymin = int(obj['bndbox']['ymin'])
        xmax = int(obj['bndbox']['xmax'])
        ymax = int(obj['bndbox']['ymax'])
        bbox = voc_xyxy2xywhn((xmin, ymin, xmax, ymax), (width, height))
        if class_indices is not None:
            obj_category = class_indices[obj_name]
            object = [obj_category, bbox]
            objects.append(object)

    return filename, objects


def parseXmlFilse(voc_dir, save_dir):
    assert os.path.exists(voc_dir), "ERROR {} does not exists".format(voc_dir)
    if os.path.exists(save_dir):
        shutil.rmtree(save_dir)
    os.makedirs(save_dir)

    xml_files = [os.path.join(voc_dir, i) for i in os.listdir(voc_dir) if os.path.splitext(i)[-1] == '.xml']
    for xml_file in xml_files:
        with open(xml_file) as fid:
            xml_str = fid.read()
        xml = etree.fromstring(xml_str)
        info_dict = parse_xml_to_dict(xml)
        parser_info(info_dict, only_cat=True)

    with open(save_dir + "/classes.txt", 'w') as classes_file:
        for cat in sorted(category_set):
            classes_file.write("{}\n".format(cat))

    class_indices = dict((v, k) for k, v in enumerate(sorted(category_set)))

    xml_files = tqdm(xml_files)
    for xml_file in xml_files:
        with open(xml_file) as fid:
            xml_str = fid.read()
        xml = etree.fromstring(xml_str)
        info_dict = parse_xml_to_dict(xml)
        filename, objects = parser_info(info_dict, only_cat=False, class_indices=class_indices)
        if len(objects) != 0:
            global voc_bbox_nums
            voc_bbox_nums += len(objects)
            with open(save_dir + "/" + filename.split(".")[0] + ".txt", 'w') as f:
                for obj in objects:
                    f.write(
                        "{} {:.5f} {:.5f} {:.5f} {:.5f}\n".format(obj[0], obj[1][0], obj[1][1], obj[1][2], obj[1][3]))


if __name__ == '__main__':
    """
    脚本说明：
        该脚本用于将coco、voc格式的标签文件转换为yolo格式的txt文件
    参数说明：
        format：转化文件的格式xml、json
        src:转化文件的路径
        dst:txt保存的路径
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('-format', '--format', type=str, default='./data/labels/coco/train.json', help='format')
    parser.add_argument('-src', '--src', type=str, default='./data/labels/coco/train.json', help='json path')
    parser.add_argument('-dst', '--dst', type=str, default='./data/convert/yolo', help='txt save path')
    opt = parser.parse_args()

    if (opt.format == 'coco'):
        json_path = './data/labels/coco/train.json'  # r'D:\practice\compete\goodsDec\data\train\train.json'
        txt_save_path = './data/convert/yolo'
        parseJsonFile(json_path, txt_save_path)
    else:
        parseXmlFilse(opt.src, opt.dst)
        print("image nums: {}".format(len(image_set)))
        print("category nums: {}".format(len(category_set)))
        print("bbox nums: {}".format(voc_bbox_nums))
        
