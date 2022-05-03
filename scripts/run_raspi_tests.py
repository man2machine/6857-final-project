# -*- coding: utf-8 -*-
"""
Created on Mon May  2 18:22:47 2022

@author: Shahir
"""

import os
import glob
import subprocess

from project_6857.utils import get_rel_pkg_path

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
        get_rel_pkg_path("outputs/raspi/"), test_type_dir, test_exec_name)
    cmd = "./{} > {}.txt".format(raspi_test_exec_fname, output_fname)
    proc = subprocess.run([cmd], shell=True)
