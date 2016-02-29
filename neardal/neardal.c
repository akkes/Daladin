#include "adaptermodule.h"

PyMethodDef nxppy_methods[] = {
  {NULL, NULL},
};

void initneardal() {
    PyObject *module;

    module = Py_InitModule("neardal", ndefMethods);

    Py_INCREF(&AdapterType);
    PyModule_AddObject(module, "Adapter", (PyObject *)&AdapterType);
}
