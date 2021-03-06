# -*- coding: utf-8 -*-
"""
Created on Mon May  2 18:22:47 2022

@author: Shahir
"""

import os
import glob
import subprocess

from project_6857.utils import get_rel_pkg_path

system_name = input("Enter system name (default raspi_zero_w):").strip()
if system_name.strip() == "":
    system_name = "raspi_zero_w"

search_glob = os.path.join(
    get_rel_pkg_path("raspi_tests/"), "*", "*.cpp")
raspi_test_fnames = glob.glob(search_glob, recursive=True)
test_infos = []
for raspi_test_fname in raspi_test_fnames:
    fname_split = raspi_test_fname.split(os.path.sep)
    test_type_dir = fname_split[-2]
    test_exec_name = fname_split[-1].rpartition(".")[0]
    raspi_test_exec_fname = os.path.join(
        get_rel_pkg_path("bin/"), test_type_dir, test_exec_name)
    output_fname = os.path.join(
        get_rel_pkg_path("outputs/"), system_name, test_type_dir, test_exec_name)
    os.makedirs(output_fname.rpartition(os.path.sep)[0], exist_ok=True)
    cmd = "{} > {}.txt".format(raspi_test_exec_fname, output_fname)
    print("Running {}".format(test_exec_name))
    proc = subprocess.run([cmd], shell=True)
