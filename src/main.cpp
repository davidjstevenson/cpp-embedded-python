#include "wrap_python.h"
#include "pyobject_wrapper.h"

#include <iostream>
#include <fstream>


/*
    inject the path for our local python script folder into python sys.path
    Note - this works while developing locally but is not a good option for
    a deployed application as the value of PYTHON_SCRIPT_FOLDER cannot be
    known in advance (for a well behaved installer).
*/
void python_setup_path() {
    PyObject_Borrowed syspath = PySys_GetObject("path");
    PyObject_Owned python_path = PyUnicode_FromString(PYTHON_SCRIPT_FOLDER);
    PyList_Insert(*syspath, 0, *python_path);
}

/*
    wrapper that converts a 'PyObject *' to a std:string - note that the PyObject *
    MUST be a python string instance.
 */
std::string get_string(PyObject* obj) {
    PyObject_Owned repr = PyObject_Repr(obj);
    PyObject_Owned str = PyUnicode_AsEncodedString(*repr, "utf-8", nullptr);
    return PyBytes_AS_STRING(*str);
}

/*
    python main runs the script located in src/python/main.py
*/
void python_main() {
    PyObject_Owned main = PyImport_ImportModule("__main__");
    PyObject_Borrowed globals = PyModule_GetDict(*main);
    PyObject_Owned locals = PyDict_New();

    PyRun_String("import main", Py_file_input, *globals, *locals);

    PyObject_Borrowed err = PyErr_Occurred();
    if (*err) {
        PyErr_Print();
    }
}

/*
    inspect and print some python information through the C-API
*/
void print_python_version() {
    std::cout << "==== python info from c++ ===\n";

    PyObject_Borrowed sys = PyImport_ImportModule("sys");
    PyObject_Borrowed version = PyObject_GetAttrString(*sys, "version");
    std::string version_str = get_string(*version);

    PyObject_Borrowed base_prefix = PyObject_GetAttrString(*sys, "base_prefix");
    std::string base_prefix_str = get_string(*base_prefix);

    // note - Py_GetPath returns an address to static memory
    const wchar_t* path = Py_GetPath();

    std::cout << "sys.version:     " << version_str << "\n";
    std::cout << "sys.base_prefix: " << base_prefix_str << "\n";
    std::wcout << "sys.path:        " << path << L"\n";

    std::cout << "==== ==================== ===\n\n";
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // Run Python in isolated mode. In isolated mode sys.path contains neither the script's
    /// directory nor the user's site-packages directory.
    Py_IsolatedFlag = 1;

    // Disable the import of the module site and the site-dependent manipulations of
    // sys.path that it entails. Also disable these manipulations if site is explicitly
    // imported later (call site.main() if you want them to be triggered)
    Py_NoSiteFlag = 1;

    // setup python
    Py_Initialize();

    python_setup_path();
    print_python_version();
    python_main();

    // teardown python
    Py_Finalize();

    return 0;
}
