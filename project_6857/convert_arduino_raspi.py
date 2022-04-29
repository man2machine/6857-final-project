# -*- coding: utf-8 -*-
"""
Created on Thu Apr 28 16:14:19 2022

@author: Shahir
"""

INCLUDE_FIND = "#include [^\n]+"
INITIAL_INCLUDE = """
#include <iostream>
#include <chrono>

unsigned long micros()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

#include <Piduino.h>

"""
SERIAL_BEGIN_FIND = r"Serial.begin\(9600\);"
SERIAL_PRINT_FIND = r"Serial.print\(([^;]*)\);"
SERIAL_PRINT_REPLACE = r"std::cout << \1;"
SERIAL_PRINTLN_FIND = r"Serial.println\(([^;]+)\);"
SERIAL_PRINTLN_REPLACE = r"std::cout << \1 << std::endl;"
SERIAL_PRINTLN_EMTPY_FIND = r"Serial.println\(\);"
SERIAL_PRINTLN_EMTPY_REPLACE = r"std::cout << std::endl;"

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
    
    raspi_test_file_data = re.sub(SERIAL_BEGIN_FIND, "", raspi_test_file_data)
    raspi_test_file_data = re.sub(SERIAL_PRINT_FIND, SERIAL_PRINT_REPLACE, raspi_test_file_data)
    raspi_test_file_data = re.sub(SERIAL_PRINTLN_FIND, SERIAL_PRINTLN_REPLACE, raspi_test_file_data)
    raspi_test_file_data = re.sub(SERIAL_PRINTLN_EMTPY_FIND, SERIAL_PRINTLN_EMTPY_REPLACE, raspi_test_file_data)
    
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
