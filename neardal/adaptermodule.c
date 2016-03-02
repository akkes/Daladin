#include "adaptermodule.h"

GMainLoop	    *gMain_loop = NULL;
static neardal_record last_record;
static int is_record_modified;
static char adapter_name[30];
static sem_t record_avaible;

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

    // init semaphore
    sem_init(&record_avaible, 0, 0);

    return 0;
}

void* adapter_loop(int is_from_python_thread) {
    errorCode_t	ec;
    /* Start Discovery Loop*/
	printf("%s\n", adapter_name);
	ec = neardal_start_poll(adapter_name);
	if (ec != NEARDAL_SUCCESS && ec != NEARDAL_ERROR_POLLING_ALREADY_ACTIVE)
	{
		printf("Error starting discovery loop\n");
		return (void*)1;
	}

	return NULL;
}

PyObject* launch(Adapter* self, PyObject* args) {
    PyObject *result = NULL;
    memcpy(adapter_name, self->adpName, 30*sizeof(char));
    adapter_loop(1);

    Py_BEGIN_ALLOW_THREADS
	gMain_loop = g_main_loop_new(NULL, FALSE);
	if (gMain_loop) {
            puts("start loop");
            g_main_loop_run(gMain_loop);
            g_main_loop_unref(gMain_loop);
	} else
		return NULL;
    Py_END_ALLOW_THREADS

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
void call_adapter_added(const char* tagName, void* data) {
    puts("call_adapter_added");
}

// signals for action
void call_adapter_removed(const char* tagName, void* data) {
    puts("call_adapter_removed");
}

// signals for action
void call_adapter_property_changed(char *adpName, char *propName, void *value, void *user_data) {
    puts("call_adapter_property_changed");
}

// signals for action
void call_tag_found(const char* tagName, void* data) {
    puts("call_tag_found");
}

// signals for action
void call_tag_lost(const char* tagName, void* data) {
    puts("call_tag_lost");

    adapter_loop(0);
}

// signals for action
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

    errorCode_t	err;
	neardal_record* pRecord;
    printf("recordName: %s\n", recordName);

	err = neardal_get_record_properties(recordName, &pRecord);
	if(err != NEARDAL_SUCCESS)
	{
		g_warning("Error %d when reading record %s (%s)\r\n", err, recordName, neardal_error_get_text(err));
		return;
	}
    last_record = *pRecord;
    is_record_modified = 1;
    puts("got record properties");

	// Dump record's content
	dump_record(pRecord);

    // signal update
    sem_post(&record_avaible);
}

PyObject* get_last_record(Adapter* self, PyObject* args) {
    puts("get_record");
    PyObject* dict = NULL;
    is_record_modified = 0;
    neardal_record* pRecord = &last_record;

    //build dictionnary
    dict = Py_BuildValue("{s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s}",
                         "action",      pRecord->action,
                         "carrier",     pRecord->carrier,
                         "encoding",    pRecord->encoding,
                         "language",    pRecord->language,
                         "MIME",        pRecord->mime,
                         "name",        pRecord->name,
                         "representation", pRecord->representation,
                         "size",        pRecord->uriObjSize,
                         "type",        pRecord->type,
                         "SSID",        pRecord->ssid,
                         "passphrase",  pRecord->passphrase,
                         "authentication", pRecord->authentication,
                         "encryption",  pRecord->encryption,
                         "URI",         pRecord->uri);

    return dict;
}

PyObject* wait_record(Adapter* self, PyObject* args) {
    sem_wait(&record_avaible);

    Py_RETURN_NONE;
}

/**********************
 * Python definitions *
 **********************/

PyMethodDef AdapterMethods[] =
{
    {"say_hello", (PyCFunction)say_hello, METH_VARARGS, "Greet somebody."},
    {"launch", (PyCFunction)launch, METH_VARARGS, "launch adapter interaction"},
    {"stop", (PyCFunction)stop, METH_VARARGS, "stop adapter interaction"},
    {"get_last_record", (PyCFunction)get_last_record, METH_VARARGS, "get record record_name"},
    {"wait_record", (PyCFunction)wait_record, METH_NOARGS, "wait for a record"},
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
