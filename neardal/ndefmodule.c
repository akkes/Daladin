#include <Python.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <glib.h>
#include <neardal.h>

static PyObject*
say_hello(PyObject* self, PyObject* args)
{
    const char* name;

    if (!PyArg_ParseTuple(args, "s", &name))
    return NULL;

    printf("Hello %s!\n", name);

    Py_RETURN_NONE;
}

neardal_set_cb_adapter_added(cb_adapter_added, NULL);
neardal_set_cb_adapter_removed(cb_adapter_removed, NULL);
neardal_set_cb_adapter_property_changed(cb_adapter_prop_changed, NULL);
neardal_set_cb_tag_found(cb_tag_found, NULL);
neardal_set_cb_tag_lost(cb_tag_lost, NULL);
neardal_set_cb_record_found(cb_record_found, NULL);

// signals for action
static PyObject* callback_adapter_added = NULL;
static PyObject* add_callback_adapter_added(PyObject* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_adapter_added);  /* Dispose of previous callback */
        callback_adapter_added = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void call_adapter_added(const char* tagName, void* data) {
    puts("call_adapter_added");
}

// signals for action
static PyObject* callback_adapter_removed = NULL;
static PyObject* add_callback_adapter_removed(PyObject* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_adapter_removed);  /* Dispose of previous callback */
        callback_adapter_removed = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void call_adapter_removed(const char* tagName, void* data) {
    puts("call_adapter_removed");
}

// signals for action
static PyObject* callback_adapter_property_changed = NULL;
static PyObject* add_callback_adapter_property_changed(PyObject* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_adapter_property_changed);  /* Dispose of previous callback */
        callback_adapter_property_changed = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void call_adapter_property_changed(const char* tagName, void* data) {
    puts("call_adapter_property_changed");
}

// signals for action
static PyObject* callback_tag_found = NULL;
static PyObject* add_callback_tag_found(PyObject* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_tag_found);  /* Dispose of previous callback */
        callback_tag_found = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void call_tag_found(const char* tagName, void* data) {
    puts("call_tag_found");
}

// signals for action
static PyObject* callback_tag_lost = NULL;
static PyObject* add_callback_tag_lost(PyObject* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_tag_lost);  /* Dispose of previous callback */
        callback_tag_lost = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void call_tag_lost(const char* tagName, void* data) {
    puts("call_tag_lost");
}

// signals for action
static PyObject* callback_record_found = NULL;
static PyObject* add_callback_record_found(PyObject* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_record_found);  /* Dispose of previous callback */
        callback_record_found = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void call_record_found(const char* tagName, void* data) {
    puts("call_record_found");
}

static PyObject* launch(PyObject* self, PyObject* args) {
    /* Start Discovery Loop*/
	ec = neardal_start_poll(adpName);
	if (ec != NEARDAL_SUCCESS && ec != NEARDAL_ERROR_POLLING_ALREADY_ACTIVE)
	{
		printf("Error starting discovery loop\n");
		return 1;
	}

	gMain_loop = g_main_loop_new(NULL, FALSE);
	if (gMain_loop) {
		g_main_loop_run(gMain_loop);
		g_main_loop_unref(gMain_loop);
	} else
		return 1;

    Py_INCREF(Py_None);
    result = Py_None;
}


static PyMethodDef ndefMethods[] =
{
    {"say_hello", say_hello, METH_VARARGS, "Greet somebody."},
    {"add_callback_adapter_added", add_callback_adapter_added, METH_VARARGS, "add callback for action"},
    {"add_callback_adapter_removed", add_callback_adapter_removed, METH_VARARGS, "add callback for action"},
    {"add_callback_adapter_property_changed", add_callback_adapter_property_changed, METH_VARARGS, "add callback for action"},
    {"add_callback_tag_found", add_callback_tag_found, METH_VARARGS, "add callback for action"},
    {"add_callback_tag_lost", add_callback_tag_lost, METH_VARARGS, "add callback for action"},
    {"add_callback_record_found", add_callback_record_found, METH_VARARGS, "add callback for action"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initndef(void)
{
    (void) Py_InitModule("ndef", ndefMethods);

    errorCode_t	ec;
    char           **adpArray = NULL;
    int              adpLen;
    char             adpName[30];
    neardal_adapter	*adapter;
    static int	     power = 1;

    (void) argc; /* Remove warning */
    (void) argv; /* Remove warning */

    /* Look for available adapter */
    ec = neardal_get_adapters(&adpArray, &adpLen);
    if (ec == NEARDAL_SUCCESS)
    {
        printf(".. Adapter found at '%s'\n", adpArray[0]);
        memcpy(adpName, adpArray[0], sizeof(adpName));
        neardal_free_array(&adpArray);
    } else
    {
        printf("No adapter found (%s)\n", neardal_error_get_text(ec));
        return 1;
    }

    /* Power on first adapter found */
    ec = neardal_get_adapter_properties(adpName,&adapter);
    if (ec == NEARDAL_SUCCESS)
    {
        power=adapter->powered;
        neardal_free_adapter(adapter);
        if (!power)
        {
            power = 1;
            ec = neardal_set_adapter_property(adpName, NEARD_ADP_PROP_POWERED, GINT_TO_POINTER(power));
            if (ec != NEARDAL_SUCCESS) {
                printf("Error setting adapter properties\n");
                return 1;
            }
        }
    } else
    {
        printf("Error getting adapter properties\n");
        return 1;
    }

    neardal_set_cb_adapter_added(callback_adapter_added, NULL);
    neardal_set_cb_adapter_removed(callback_adapter_removed, NULL);
    neardal_set_cb_adapter_property_changed(callback_adapter_prop_changed, NULL);
    ec = neardal_set_cb_tag_found(callback_tag_found, NULL);
    if (ec != NEARDAL_SUCCESS)
    {
        printf("Error registering Tag found callback\n");
        return 1;
    }
    neardal_set_cb_tag_lost(callback_tag_lost, NULL);
    neardal_set_cb_record_found(callback_record_found, NULL);
}
