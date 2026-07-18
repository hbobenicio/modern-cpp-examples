#!/usr/bin/env python3
import sys
import os
from pathlib import Path
import argparse

def readable_dir(path: str):
    """
    Checks if a path is a readable directory.
    """
    if not os.path.isdir(path):
        raise argparse.ArgumentTypeError(f"\"{path}\" is not a valid directory.")
    if not os.access(path, os.R_OK):
        raise argparse.ArgumentTypeError(f"\"{path}\" is not a readable directory.")
    return path

def main():
    parser = argparse.ArgumentParser(description='An utility to generate C++ modules')
    parser.add_argument('BASE_DIR', type=readable_dir, help='Base directory where files will be generated')
    parser.add_argument('MODULE_NAME', type=str, help='Name of the module to be created')
    args = parser.parse_args()
    
    base_dir = Path(args.BASE_DIR)
    module_name: str = args.MODULE_NAME
    cpp_module_path: Path = base_dir / Path(module_name).with_suffix('.cpp')
    hpp_module_path: Path = base_dir / Path(module_name).with_suffix('.hpp')
    
    print(cpp_module_path)
    print(hpp_module_path)
    
    #TODO create templates
    #TODO render templates to each path

if __name__ == '__main__':
    main()
