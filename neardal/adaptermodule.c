#include "adaptermodule.h"

GMainLoop	    *gMain_loop = NULL;

int
Adapter_init(Adapter *self, PyObject *args, PyObject *kwds)
{
    puts("Adapter_init");
    errorCode_t	ec;
    char           **adpArray = NULL;
    int              adpLen;
    neardal_adapter	*adapter;
    static int	     power = 1;

    /* Look for available adapter */
    ec = neardal_get_adapters(&adpArray, &adpLen);
    if (ec == NEARDAL_SUCCESS) {
        printf(".. Adapter found at '%s'\n", adpArray[0]);
        memcpy(self->adpName, adpArray[0], sizeof(self->adpName));
        neardal_free_array(&adpArray);
    } else {
        printf("No adapter found (%s)\n", neardal_error_get_text(ec));
        return 1;
    }

    /* Power on first adapter found */
    ec = neardal_get_adapter_properties(self->adpName,&adapter);
    if (ec == NEARDAL_SUCCESS) {
        power=adapter->powered;
        neardal_free_adapter(adapter);
        if (!power) {
            power = 1;
            ec = neardal_set_adapter_property(self->adpName, NEARD_ADP_PROP_POWERED, GINT_TO_POINTER(power));
            if (ec != NEARDAL_SUCCESS) {
                printf("Error setting adapter properties\n");
                return 1;
            }
        }
    } else {
        printf("Error getting adapter properties\n");
        return 1;
    }

    neardal_set_cb_adapter_added(call_adapter_added, self);
    neardal_set_cb_adapter_removed(call_adapter_removed, self);
    neardal_set_cb_adapter_property_changed(call_adapter_property_changed, self);
    ec = neardal_set_cb_tag_found(call_tag_found, self);
    if (ec != NEARDAL_SUCCESS) {
        printf("Error registering Tag found callback\n");
        return 1;
    }
    neardal_set_cb_dev_found(call_tag_found, self);
    neardal_set_cb_tag_lost(call_tag_lost, self);
    neardal_set_cb_dev_lost(call_tag_lost, self);
    neardal_set_cb_record_found(call_record_found, self);

    return 0;
}

void* adapter_loop(void* arg) {
    errorCode_t	ec;
    Adapter* self = (Adapter*)arg;
    /* Start Discovery Loop*/
	printf("%s\n", self->adpName);
	ec = neardal_start_poll(self->adpName);
	if (ec != NEARDAL_SUCCESS && ec != NEARDAL_ERROR_POLLING_ALREADY_ACTIVE)
	{
		printf("Error starting discovery loop\n");
		return (void*)1;
	}

	gMain_loop = g_main_loop_new(NULL, FALSE);
	if (gMain_loop) {
Py_BEGIN_ALLOW_THREADS
		g_main_loop_run(gMain_loop);
		g_main_loop_unref(gMain_loop);
Py_END_ALLOW_THREADS
	} else
		return (void*)1;

	return (void*)0;
}

PyObject* launch(Adapter* self, PyObject* args) {
    PyObject *result = NULL;

    //pthread_create(&self->adapter_thread, 0, adapter_loop, self);
adapter_loop(self);

    Py_INCREF(Py_None);
    result = Py_None;

    return result;
}

PyObject* stop(Adapter* self, PyObject* args) {
    PyObject *result = NULL;

    g_main_loop_quit(gMain_loop);
    neardal_stop_poll(self->adpName);

    Py_INCREF(Py_None);
    result = Py_None;

    return result;
}

static PyObject*
say_hello(PyObject* self, PyObject* args)
{
    const char* name;

    if (!PyArg_ParseTuple(args, "s", &name))
    return NULL;

    printf("Hello %s!\n", name);

    Py_RETURN_NONE;
}

/* neardal_set_cb_adapter_added(cb_adapter_added, NULL);
 * neardal_set_cb_adapter_removed(cb_adapter_removed, NULL);
 * neardal_set_cb_adapter_property_changed(cb_adapter_prop_changed, NULL);
 * neardal_set_cb_tag_found(cb_tag_found, NULL);
 * neardal_set_cb_tag_lost(cb_tag_lost, NULL);
 * neardal_set_cb_record_found(cb_record_found, NULL);
 */

// signals for action
static PyObject* callback_adapter_added = NULL;
PyObject* add_callback_adapter_added(Adapter* self, PyObject* args) {
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

void call_adapter_added(const char* tagName, void* data) {
    puts("call_adapter_added");
}

// signals for action
static PyObject* callback_adapter_removed = NULL;
PyObject* add_callback_adapter_removed(Adapter* self, PyObject* args) {
    PyObject* result = NULL;
    PyObject* temp;

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

void call_adapter_removed(const char* tagName, void* data) {
    puts("call_adapter_removed");
}

// signals for action
static PyObject* callback_adapter_property_changed = NULL;
PyObject* add_callback_adapter_property_changed(Adapter* self, PyObject* args) {
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

void call_adapter_property_changed(char *adpName, char *propName, void *value, void *user_data) {
    puts("call_adapter_property_changed");
}

// signals for action
static PyObject* callback_tag_found = NULL;
PyObject* add_callback_tag_found(Adapter* self, PyObject* args) {
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

void call_tag_found(const char* tagName, void* data) {
    puts("call_tag_found");
}

// signals for action
static PyObject* callback_tag_lost = NULL;
PyObject* add_callback_tag_lost(Adapter* self, PyObject* args) {
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

void call_tag_lost(const char* tagName, void* data) {
    puts("call_tag_lost");
    Adapter* self = (Adapter*)data;

    pthread_create(&self->adapter_thread, 0, adapter_loop, self);
}

// signals for action
static PyObject* callback_record_found = NULL;
PyObject* add_callback_record_found(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O", &temp)) {
	puts("is an object");
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
	puts("object is a function and callable");
printf("Object: %0x\n", (unsigned int)callback_record_found);
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(callback_record_found);  /* Dispose of previous callback */
        callback_record_found = temp;       /* Remember new callback */
PyObject_Print(callback_record_found, stdout, Py_PRINT_RAW);
	puts("echange done");
printf("Object: %0x\n", (unsigned int)callback_record_found);
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void dump_record(neardal_record* pRecord) {
	if( pRecord->name != NULL )
	{
		printf("Found record %s\r\n", pRecord->name);
	}
	else
	{
		printf("Found record\r\n");
	}

	if( pRecord->type != NULL )
	{
		printf("Record type: \t%s\r\n", pRecord->type);
	}
	else
	{
		printf("Unknown record type\r\n");
	}

	//Dump fields that are set
	if( pRecord->uri != NULL )
	{
		printf("URI: \t%s\r\n", pRecord->uri);
	}

	if( pRecord->representation != NULL )
	{
		printf("Title: \t%s\r\n", pRecord->representation);
	}

	if( pRecord->action != NULL )
	{
		printf("Action: \t%s\r\n", pRecord->action);
	}

	if( pRecord->language != NULL )
	{
		printf("Language: \t%s\r\n", pRecord->language);
	}

	if( pRecord->encoding != NULL )
	{
		printf("Encoding: \t%s\r\n", pRecord->encoding);
	}

	if( pRecord->mime != NULL )
	{
		printf("MIME type: \t%s\r\n", pRecord->mime);
	}

	if( pRecord->uriObjSize > 0 )
	{
		printf("URI object size: \t%u\r\n", pRecord->uriObjSize);
	}

	if( pRecord->carrier != NULL )
	{
		printf("Carrier: \t%s\r\n", pRecord->carrier);
	}

	if( pRecord->ssid != NULL )
	{
		printf("SSID: \t%s\r\n", pRecord->ssid);
	}

	if( pRecord->passphrase != NULL )
	{
		printf("Passphrase: \t%s\r\n", pRecord->passphrase);
	}

	if( pRecord->encryption != NULL )
	{
		printf("Encryption: \t%s\r\n", pRecord->encryption);
	}

	if( pRecord->authentication != NULL )
	{
		printf("Authentication: \t%s\r\n", pRecord->authentication);
	}
}

void call_record_found(const char* recordName, void* data) {
    puts("call_record_found");
    PyObject* stringName = NULL;

    errorCode_t	err;
	neardal_record* pRecord;
printf("%s\n", recordName);

	err = neardal_get_record_properties(recordName, &pRecord);
	if(err != NEARDAL_SUCCESS)
	{
		g_warning("Error %d when reading record %s (%s)\r\n", err, recordName, neardal_error_get_text(err));
		return;
	}

	// Dump record's content
	// dump_record(pRecord);

    puts("buildValue");
    stringName = Py_BuildValue("s", recordName);
    puts("callObject");
printf("Object: %0x\n", (unsigned int)callback_record_found);
PyObject_Print(callback_record_found, stdout, Py_PRINT_RAW);
	puts("echange done");
        if (!PyCallable_Check(callback_record_found)) {
		puts("isCallable");
            PyObject_CallObject(callback_record_found, stringName);
        }
    puts("end reached");
}

PyObject* get_record(Adapter* self, PyObject* args) {
    puts("get_record");
    PyObject* dict = NULL;
    const char* recordName;
    neardal_record* pRecord;
    errorCode_t err;

    puts("parseTuple");
    if (PyArg_ParseTuple(args, "s", recordName)) {
        puts("tuple is a string");
        err = neardal_get_record_properties(recordName, &pRecord);
    	if(err != NEARDAL_SUCCESS)
    	{
    		g_warning("Error %d when reading record %s (%s)\r\n", err, recordName, neardal_error_get_text(err));
    		return NULL;
    	}
        puts("got record properties");

        //build dictionnary
        dict = Py_BuildValue("{s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s}",
                      "action", pRecord->action,
                      "carrier", pRecord->carrier,
                      "encoding", pRecord->encoding,
                      "language", pRecord->language,
                      "MIME", pRecord->mime,
                      "name", pRecord->name,
                      "representation", pRecord->representation,
                      "size", pRecord->uriObjSize,
                      "type", pRecord->type,
                      "SSID", pRecord->ssid,
                      "passphrase", pRecord->passphrase,
                      "authentication", pRecord->authentication,
                      "encryption", pRecord->encryption,
                      "URI", pRecord->uri);

	Py_INCREF(dict);
        return dict;
    }

    return NULL;
}

/**********************
 * Python definitions *
 **********************/

PyMethodDef AdapterMethods[] =
{
    {"say_hello", (PyCFunction)say_hello, METH_VARARGS, "Greet somebody."},
    {"launch", (PyCFunction)launch, METH_VARARGS, "launch adapter interaction"},
    {"stop", (PyCFunction)stop, METH_VARARGS, "stop adapter interaction"},
    {"add_callback_adapter_added", (PyCFunction)add_callback_adapter_added, METH_VARARGS, "add callback for action"},
    {"add_callback_adapter_removed", (PyCFunction)add_callback_adapter_removed, METH_VARARGS, "add callback for action"},
    {"add_callback_adapter_property_changed", (PyCFunction)add_callback_adapter_property_changed, METH_VARARGS, "add callback for action"},
    {"add_callback_tag_found", (PyCFunction)add_callback_tag_found, METH_VARARGS, "add callback for action"},
    {"add_callback_tag_lost", (PyCFunction)add_callback_tag_lost, METH_VARARGS, "add callback for action"},
    {"add_callback_record_found", (PyCFunction)add_callback_record_found, METH_VARARGS, "add callback for action"},
    {"get_record", (PyCFunction)get_record, METH_VARARGS, "get record record_name"},
    {NULL, NULL, 0, NULL}
};

PyTypeObject AdapterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "neardal.Adapter",             /* tp_name */
    sizeof(Adapter), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "adapter object",          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    AdapterMethods,            /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Adapter_init,      /* tp_init */
};
