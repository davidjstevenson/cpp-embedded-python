from some_file import some_file
from test_module.test_module_main import test_module_main

from utils import print_python_system_info
print_python_system_info(locals(), globals())


def main():
    print(f"main.py, __name__: {__name__}")

    some_file()
    test_module_main()

if __name__ == "__main__":
    main()
