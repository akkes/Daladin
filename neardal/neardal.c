#include <Python.h>
#include "adaptermodule.h"

PyMethodDef neardal_methods[] = {
  {NULL, NULL},
};

void initneardal() {
    puts("initneardal");
    PyObject *module;

    AdapterType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&AdapterType) < 0) {
        return;
    }

    module = Py_InitModule("neardal", neardal_methods);

    if (module == NULL) {
        puts("init error");
        return;
    }

    Py_INCREF(&AdapterType);
    PyModule_AddObject(module, "Adapter", (PyObject *)&AdapterType);
}
