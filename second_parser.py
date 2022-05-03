# -*- coding: utf-8 -*-
"""second_parser.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1_8E1Z5ty0pIhpcf-gdXaZlXZMJze3985
"""

import re
import pandas as pd
import json
from google.colab import files

uploaded = files.upload()

def second_parser(name):
  dict = {}
  
  with open(name + '.txt') as f:
      lines = f.readlines()
  
  for line in lines:
    if len(line) > 1 and line[-2] == ':':
      name = line[:-2]
      dict[name] = {}

    if 'second' in line:
      nums = re.findall('\d+[.]?\d+', line)
      words = re.findall('[A-Za-z]+', line)
      var = words[0] + ' ' + nums[0]
      function = var +' bytes' if 'packets' not in line else var + ' byte packets'
      num_1 = nums[-2] + ' us/byte' 
      num_2 = nums[-1] + ' bytes/sec'
      result = num_1, num_2
      dict[name][function] = result + (nums[1]+'x',) if len(nums) == 4 else result
  return dict

second_parser('sample_857')