# -*- encoding: utf-8 -*-
import os
import re

files_path = 'E:/project/mydata/trains/images_backup/'
files = os.listdir(files_path)
files.sort(key=len)

for f in files:
    old_file = os.path.join(files_path, f)
    new_file_name = files.index(f) + 1
    file_suffix = os.path.splitext(f)[1]
    new_file = os.path.join(files_path, '{}{}'.format(new_file_name, file_suffix))
    os.rename(old_file, new_file)
