#include <Python.h>
#include "Web_Server.hpp"
#include "../Logger/Logger.hpp"

#define PY_SSIZE_T_CLEAN
#define PY_MODULE_NAME "web_server"
#define PY_FUNC_NAME "serve"

namespace Web_Server
{

void *serve(void *port)
{
	PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
	long ret;

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
			PyTuple_SetItem(pArgs, 0, PyLong_FromLong(*(int*)port));
			
			pValue = PyObject_CallObject(pFunc, pArgs);
			if (pValue == NULL) throw WS_Err("Call to function failed");
			ret = PyLong_AsLong(pValue);
			
			switch(ret)
			{
				case 0:
				{
					Logger:log(Logger::LOG_INFO, "Web server process ended correctly");
					break;
				}
				case 1:
				{
					Logger:log(Logger::LOG_ERROR, "Port parameter is invalid");
					break;
				}
				case 2:
				{
					Logger:log(Logger::LOG_ERROR, "Exception raised in web server");
					break;
				}
				default:
				{
					Logger:log(Logger::LOG_WARN, "Unknown value returned by process");
				}
			}
		}
	}
	catch(WS_Err& e)
	{
		if(PyErr_Occurred()) PyErr_Print();
		Logger::log(Logger::LOG_ERROR, e.what());
		ret = -1;
	}
	
	Py_DECREF(pArgs);
	Py_DECREF(pValue);
	Py_XDECREF(pFunc);
	Py_DECREF(pModule);
    Py_FinalizeEx();

    pthread_exit(&ret);
}

} // end namespace Web_Server
