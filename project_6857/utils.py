# -*- coding: utf-8 -*-
"""
Created on Thu Apr 28 16:14:00 2022

@author: Shahir
"""

import os
import abc
import datetime
import json

import project_6857

class JSONDictSerializable(metaclass=abc.ABCMeta):
    def __str__(self):
        return str(self.to_dict())
    
    def __repr__(self):
        return str(self.to_dict())
    
    @abc.abstractmethod
    def to_dict(self):
        pass
    
    @classmethod
    @abc.abstractmethod
    def from_dict(cls, dct):
        pass

    def to_json(self):
        return json.dumps(self.to_dict())

    @classmethod
    def from_json(cls, data):
        return cls.from_dict(json.loads(data))

    def to_bytes(self):
        return self.to_json().encode()

    @classmethod
    def from_bytes(cls, data):
        return cls.from_json(data.decode())

def number_menu(option_list):
    print("-"*60)
    
    for n in range(len(option_list)):
        print(n, ": " , option_list[n])
    
    choice = input("Choose the number corresponding to your choice: ")
    for n in range(5):        
        try: 
            choice = int(choice)
            if choice < 0 or choice > len(option_list)-1:
                raise ValueError    
            print("-"*60 + "\n")
            return choice, option_list[choice]
        except ValueError: 
            choice = input("Invalid input, choose again: ")
    
    raise ValueError("Not recieving a valid input")

def get_rel_pkg_path(path):
    return os.path.abspath(os.path.join(os.path.dirname(project_6857.__file__), "..", path))

def load_rel_config_json(fname):
    fname = get_rel_pkg_path(fname)
    with open(fname, 'r') as f:
        data = json.load(f)
    return data

def get_timestamp_str(include_seconds=True):
    if include_seconds:
        return datetime.datetime.now().strftime("%m-%d-%Y %I-%M-%S %p")
    else:
        return datetime.datetime.now().strftime("%m-%d-%Y %I-%M %p")
