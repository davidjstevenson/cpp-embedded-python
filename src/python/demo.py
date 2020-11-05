from some_file import some_file as calling_function_from_another_file
from test_module.test_module_main import test_module_main as calling_function_from_another_module
import json
from traceback import print_last

from utils import print_python_system_info
print_python_system_info(locals(), globals())


def main():
    print(f"{__file__}, __name__: {__name__}")
    calling_function_from_another_file()
    calling_function_from_another_module()

if __name__ in ["__main__", "main"]:
    main()
