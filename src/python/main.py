from some_file import some_file
from test_module.test_module_main import test_module_main
import json

try:
    import requests
except ImportError:
    print("cannot import requests")


from utils import print_python_system_info
print_python_system_info(locals(), globals())


def main():
    print(f"main.py, __name__: {__name__}")

    try:
        result = requests.get('https://jsonplaceholder.typicode.com/todos/1')
        print(json.dumps(result.json(), indent=4))
    except:
        print('request failed')

    some_file()
    test_module_main()

if __name__ in ["__main__", "main"]:
    main()
