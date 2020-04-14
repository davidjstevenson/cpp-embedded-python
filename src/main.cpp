#include "wrap_python.h"

#include <iostream>
#include <fstream>


void python_setup_path() {
    // syspath does not need (MUST NOT BE) DECREF'ed
    PyObject* syspath = PySys_GetObject("path");
    PyObject* python_path = PyUnicode_FromString(PYTHON_SCRIPT_FOLDER);
    PyList_Insert(syspath, 0, python_path);
    Py_DECREF(python_path);
}

std::string get_string(PyObject* obj) {
    PyObject* repr = PyObject_Repr(obj);
    PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
    std::string std_str = PyBytes_AS_STRING(str);

    Py_DECREF(repr);
    Py_DECREF(str);

    return std_str;
}

void python_main() {
    PyObject* main = PyImport_ImportModule("__main__");
    PyObject* globals = PyModule_GetDict(main);
    PyObject* locals = PyDict_New();

    PyRun_String("import main", Py_file_input, globals, locals);

    Py_DECREF(main);
    Py_DECREF(globals);
    Py_DECREF(locals);

    PyObject* err = PyErr_Occurred();
    if (err) {
        PyErr_Print();
    }
}

void print_python_version() {
    std::cout << "==== python info from c++ ===\n";

    PyObject *sys = PyImport_ImportModule("sys");
    PyObject* version = PyObject_GetAttrString(sys, "version");
    std::string version_str = get_string(version);

    PyObject* base_prefix = PyObject_GetAttrString(sys, "base_prefix");
    std::string base_prefix_str = get_string(base_prefix);
    Py_DECREF(version);
    Py_DECREF(base_prefix);
    Py_DECREF(sys);

    const wchar_t* path = Py_GetPath();

    std::cout << "sys.version:     " << version_str << "\n";
    std::cout << "sys.base_prefix: " << base_prefix_str << "\n";
    std::wcout << "sys.path:        " << path << L"\n";

    std::cout << "==== ==================== ===\n\n";
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // Run Python in isolated mode. In isolated mode sys.path contains neither the script�s
    /// directory nor the user�s site-packages directory.
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
