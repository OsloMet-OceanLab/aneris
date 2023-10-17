#include <Python.h>
#include "Web_Server.hpp"
#include "../Logger/Logger.hpp"

#define PY_SSIZE_T_CLEAN
#ifndef PY_MODULE_NAME
#	define PY_MODULE_NAME "web_server"
#endif
#ifndef PY_FUNC_NAME
#	define PY_FUNC_NAME "serve"
#endif

namespace Web_Server
{

static void cleanup_handler(void *ret);

PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;

void *serve(ws_t *param)
{
	long ret = 0;

	pthread_cleanup_push(&cleanup_handler, &ret);

	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault(PY_MODULE_NAME);
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	
	try
	{
		if(pModule == NULL) throw WS_Err("Couldn't import python module");
		
		pFunc = PyObject_GetAttrString(pModule, PY_FUNC_NAME);

		if (!pFunc || !PyCallable_Check(pFunc)) throw WS_Err("Couldn't find function");
		{
			pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, PyLong_FromLong(param->port));
			PyTuple_SetItem(pArgs, 1, PyString_FromString(param->mode));
			
			pValue = PyObject_CallObject(pFunc, pArgs);
			if (pValue == NULL) throw WS_Err("Call to function failed");
			ret = PyLong_AsLong(pValue);
		}
	}
	catch(WS_Err& e)
	{
		if(PyErr_Occurred()) PyErr_Print();
		Logger::log(Logger::LOG_ERROR, e.what());
		ret = -1;
	}

	pthread_cleanup_pop(1);

	pthread_exit(&ret);
}

static void cleanup_handler(void *ret)
{
	switch(*(int*)ret)
	{
		case 0:
		{
			Logger::log(Logger::LOG_INFO, "Web server process ended correctly");
			break;
		}
		case 1:
		{
			Logger::log(Logger::LOG_ERROR, "Port parameter is invalid");
			break;
		}
		case 2:
		{
			Logger::log(Logger::LOG_ERROR, "Exception raised in web server");
			break;
		}
		default:
		{
			Logger::log(Logger::LOG_WARN, "Unknown value returned by process");
		}
	}
	Py_DECREF(pArgs);
	Py_DECREF(pValue);
	Py_XDECREF(pFunc);
	Py_DECREF(pModule);
	Py_FinalizeEx();
	
	return;
}

} // end namespace Web_Server
