import sys

def print_python_system_info(locals_, globals_):
    print("=== python system info ===")
    for path in sys.path:
        print(f"sys.path: {path}")

    def print_vars(name, vars):
        print(f"{name}:")
        for k, v in vars.items():
            if isinstance(v, (dict, list)):
                print(f"    {k} = {type(v)}")
            else:
                print(f"    {k} = {v}")

    print_vars("globals", globals_)
    print_vars("locals", locals_)
    print("=== ================== ===\n")
