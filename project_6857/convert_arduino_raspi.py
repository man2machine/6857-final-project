# -*- coding: utf-8 -*-
"""
Created on Thu Apr 28 16:14:19 2022

@author: Shahir
"""

INCLUDE_FIND = "#include [^\n]+"
INITIAL_INCLUDE = """
#include <Piduino.h>
#define Serial Console

"""
PGMSPACE_FIND = """#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif"""
PGMSPACE_REPLACE = """#if defined(ESP8266) || defined(ESP32)
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif"""
LOOP_FIND = r"void loop\(\)\s+{"
LOOP_REPLACE = """void loop()
{
    exit(0);"""

import os
import glob
import re

from project_6857.utils import get_rel_pkg_path

def get_raspi_convert_info():
    search_glob = os.path.join(
        get_rel_pkg_path("external_arduino/"), "*", "tests", "*", "*.ino")
    arduino_test_fnames = glob.glob(search_glob, recursive=True)
    test_infos = []
    for arduino_test_fname in arduino_test_fnames:
        fname_split = arduino_test_fname.split(os.path.sep)
        test_type_dir = fname_split[-4]
        test_cpp_name = fname_split[-1].rpartition(".")[0] + ".cpp"
        raspi_test_fname = os.path.join(
            get_rel_pkg_path("raspi_tests/"), test_type_dir, test_cpp_name)
        test_infos.append((arduino_test_fname, raspi_test_fname))
    return test_infos

def convert_arduino_to_raspi(arduino_test_fname, raspi_test_fname):
    with open(arduino_test_fname, 'r') as f: 
        arduino_file_data = f.read()
    raspi_test_file_data = arduino_file_data
    
    raspi_test_file_data = re.sub(LOOP_FIND, LOOP_REPLACE, raspi_test_file_data)
    raspi_test_file_data = raspi_test_file_data.replace(PGMSPACE_FIND, PGMSPACE_REPLACE)
    
    include_locs = re.finditer(INCLUDE_FIND, raspi_test_file_data)
    first_include_start = next(include_locs).start()
    raspi_test_file_data = raspi_test_file_data[:first_include_start] + \
        INITIAL_INCLUDE + \
        raspi_test_file_data[first_include_start:]
    
    raspi_test_dir = raspi_test_fname.rpartition(os.path.sep)[0]
    os.makedirs(raspi_test_dir, exist_ok=True)
    with open(raspi_test_fname, 'w') as f: 
        f.write(raspi_test_file_data)

def convert_all_raspi():
    info = get_raspi_convert_info()
    for test_pair in info:
        convert_arduino_to_raspi(*test_pair)
