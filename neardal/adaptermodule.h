#include <Python.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <glib.h>
#include <neardal.h>

typedef struct {
    PyObject_HEAD
    neardal_adapter adapter;
} Adapter;

int adapter_init(Adapter *self, PyObject *args, PyObject *kwds);

static PyObject* add_callback_adapter_added(Adapter* self, PyObject* args);
static PyObject* add_callback_adapter_removed(Adapter* self, PyObject* args);
static PyObject* add_callback_adapter_property_changed(Adapter* self, PyObject* args);

static PyObject* add_callback_tag_found(Adapter* self, PyObject* args);
static PyObject* add_callback_tag_lost(Adapter* self, PyObject* args);
static PyObject* add_callback_record_found(Adapter* self, PyObject* args);

static void call_adapter_added(const char* tagName, void* data);
static void call_adapter_removed(const char* tagName, void* data);
static void call_adapter_property_changed(char *adpName, char *propName, void *value, void *user_data);
static void call_tag_found(const char* tagName, void* data);
static void call_tag_lost(const char* tagName, void* data);
static void call_record_found(const char* tagName, void* data);

// extern PyObject *InitError;

extern PyMethodDef AdapterMethods[];
extern PyTypeObject AdapterType;
