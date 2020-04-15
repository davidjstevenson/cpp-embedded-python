#pragma once

class PyObject_Owned {
public:
    PyObject_Owned(PyObject* py_object = nullptr) : object(py_object) {}
    ~PyObject_Owned() {
        if (this->object) {
            Py_DECREF(this->object);
        }
    }
    PyObject_Owned(const PyObject_Owned& other) = delete;
    PyObject_Owned& operator=(const PyObject_Owned& other) = delete;

    PyObject_Owned& operator=(PyObject* py_object) {
        if (this->object) {
            Py_DECREF(this->object);
            this->object = py_object;
        }
    }

    PyObject* operator*() const {
        return this->object;
    }

private:
    PyObject* object;
};

class PyObject_Borrowed {
public:
    PyObject_Borrowed(PyObject* py_object = nullptr) : object(py_object) {}
    ~PyObject_Borrowed() {}

    PyObject* operator*() const {
        return this->object;
    }

private:
    PyObject* object;
};