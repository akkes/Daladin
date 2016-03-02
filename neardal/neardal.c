#include "adaptermodule.h"

PyMethodDef nxppy_methods[] = {
  {NULL, NULL},
};

void initneardal() {
    puts("initneardal");
    PyObject *module;

    module = Py_InitModule("neardal", AdapterMethods);

    Py_INCREF(&AdapterType);
    PyModule_AddObject(module, "Adapter", (PyObject *)&AdapterType);
}
