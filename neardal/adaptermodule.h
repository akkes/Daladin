#include <Python.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <glib.h>
#include <pthread.h>
#include <neardal.h>

typedef struct {
    PyObject_HEAD
    neardal_adapter adapter;
    char 		adpName[30];
    pthread_t         adapter_thread;
    pthread_cond_t  stop_condition;
} Adapter;

int adapter_init(Adapter *self, PyObject *args, PyObject *kwds);

PyObject* add_callback_adapter_added(Adapter* self, PyObject* args);
PyObject* add_callback_adapter_removed(Adapter* self, PyObject* args);
PyObject* add_callback_adapter_property_changed(Adapter* self, PyObject* args);
PyObject* add_callback_tag_found(Adapter* self, PyObject* args);
PyObject* add_callback_tag_lost(Adapter* self, PyObject* args);
PyObject* add_callback_record_found(Adapter* self, PyObject* args);

void call_adapter_added(const char* tagName, void* data);
void call_adapter_removed(const char* tagName, void* data);
void call_adapter_property_changed(char *adpName, char *propName, void *value, void *user_data);
void call_tag_found(const char* tagName, void* data);
void call_tag_lost(const char* tagName, void* data);
void call_record_found(const char* tagName, void* data);

// extern PyObject *InitError;

extern PyMethodDef AdapterMethods[];
extern PyTypeObject AdapterType;
