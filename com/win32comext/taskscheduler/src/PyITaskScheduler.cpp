

// This file implements the ITaskScheduler Interface for Python.
// Generated by makegw.py

#include "PyITaskScheduler.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

BOOL wchar_input_noneok(PyObject *ob, WCHAR **ppwchar)
{
	return PyWinObject_AsWCHAR(ob,ppwchar,TRUE);
}
BOOL wchar_input_nonenotok(PyObject *ob, WCHAR **ppwchar)
{
	return PyWinObject_AsWCHAR(ob,ppwchar,FALSE);
}

PyITaskScheduler::PyITaskScheduler(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyITaskScheduler::~PyITaskScheduler()
{
}

/* static */ ITaskScheduler *PyITaskScheduler::GetI(PyObject *self)
{
	return (ITaskScheduler *)PyIUnknown::GetI(self);
}

// @pymethod |PyITaskScheduler|SetTargetComputer|Connect to another machine to manage its tasks
// @comm Leading backslashes are required.  Call will succeed without them, but no other methods will work.
PyObject *PyITaskScheduler::SetTargetComputer(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	// @pyparm <o unicode>|Computer||Name of system to connect to

	LPWSTR Computer;
	if ( !PyArg_ParseTuple(args, "O&:PyITaskScheduler::SetTargetComputer", wchar_input_nonenotok, &Computer) )
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITS->SetTargetComputer(Computer);
	PyWinObject_FreeWCHAR(Computer);
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler );
	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod <o unicode>|PyITaskScheduler|GetTargetComputer|Returns name of computer that the Task Scheduler is connected to
PyObject *PyITaskScheduler::GetTargetComputer(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	PyObject *ret=NULL;
	HRESULT hr = NULL;
	WCHAR *Computer=NULL;
	if ( !PyArg_ParseTuple(args, ":PyITaskScheduler::GetTargetComputer"))
		return NULL;

	PY_INTERFACE_PRECALL;
	hr = pITS->GetTargetComputer(&Computer);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler );
	else
		ret=PyWinObject_FromWCHAR(Computer);

	if (Computer!=NULL)
		CoTaskMemFree(Computer);
	return ret;
}

// @pymethod <o PyUnicode>,...|PyITaskScheduler|Enum|Retrieve list of task names
PyObject *PyITaskScheduler::Enum(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	if (!PyArg_ParseTuple(args, ":PyITaskScheduler::Enum"))
		return NULL;
	PyObject *task_name_obj=NULL, *ret_list=NULL;

	HRESULT hr = S_OK;
	IEnumWorkItems *pIEnumWorkItems;
	ULONG ulTasksToGet=10, ulActualTasksRetrieved=0, task_ind=0;
	LPWSTR *task_names=NULL;

	PY_INTERFACE_PRECALL;
	// create an enumerator
	hr = pITS->Enum(&pIEnumWorkItems);
	PY_INTERFACE_POSTCALL;
	if (FAILED(hr)){
		PyWin_SetAPIError("PyITaskScheduler::Enum",hr);
		return NULL;
	}

	ret_list = PyList_New(0);
	if (ret_list==NULL)
		return NULL;

	// loop over all tasks
	do{
		PY_INTERFACE_PRECALL;
		hr = pIEnumWorkItems->Next(ulTasksToGet, &task_names, &ulActualTasksRetrieved);
		PY_INTERFACE_POSTCALL;
		if (FAILED(hr)){
			PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler );
			Py_DECREF(ret_list);
			ret_list=NULL;
			break;
			}

		for (task_ind=0;task_ind<ulActualTasksRetrieved;task_ind++){
			task_name_obj = PyWinObject_FromWCHAR(task_names[task_ind]);
			PyList_Append(ret_list,task_name_obj);
			Py_DECREF(task_name_obj);
			CoTaskMemFree(task_names[task_ind]);
			}
        CoTaskMemFree(task_names);
	} while(hr==S_OK);

	if(pIEnumWorkItems){
		PY_INTERFACE_PRECALL;
		pIEnumWorkItems->Release();
		PY_INTERFACE_POSTCALL;
		pIEnumWorkItems = NULL;
		}

	return ret_list;
}

// @pymethod <o PyITask>|PyITaskScheduler|Activate|Opens the specified task and returns an ITask interface for it
PyObject *PyITaskScheduler::Activate(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	// @pyparm <o unicode>|Name||Name of task to retreive
	// @pyparm <o PyIID>|riid|IID_ITask|IID to return, currently only IID_ITask accepted
	PyObject *obtask_name=NULL, *obriid=NULL;
	LPWSTR task_name;
	IID riid=IID_ITask;
	IUnknown* pUnk;
	if (!PyArg_ParseTuple(args, "O|O:PyITaskScheduler::Activate", &obtask_name, &obriid) )
		return NULL;
	if (obriid!=NULL)
		if (!PyWinObject_AsIID(obriid, &riid))
			return NULL;
	if (!PyWinObject_AsWCHAR(obtask_name, &task_name, FALSE))
		return NULL;

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITS->Activate(task_name, riid, &pUnk);
	PyWinObject_FreeWCHAR(task_name);
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler );
	return PyCom_PyObjectFromIUnknown(pUnk, riid, FALSE);
	}

// @pymethod |PyITaskScheduler|Delete|Delete task by name
PyObject *PyITaskScheduler::Delete(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	PyObject *ret=NULL;
	HRESULT hr = NULL;
	LPWSTR task_name=NULL;
	// @pyparm <o unicode>|TaskName||Name of task to delete
	if (!PyArg_ParseTuple(args,"O&:PyITaskScheduler::Delete", wchar_input_nonenotok, &task_name))
		return NULL;

	PY_INTERFACE_PRECALL;
	hr = pITS->Delete(task_name);
	PyWinObject_FreeWCHAR(task_name);
	PY_INTERFACE_POSTCALL;
	if (FAILED(hr))
		PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler);
	else{
		Py_INCREF(Py_None);
		ret=Py_None;
		}
	return ret;
}


// @pymethod <o PyITask>|PyITaskScheduler|NewWorkItem|Creates a new task
PyObject *PyITaskScheduler::NewWorkItem(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	// @pyparm <o unicode>|TaskName||Name of new task
	// @pyparm <o PyIID>|rclsid|CLSID_CTask|Class id of work item, currently only CLSID_CTask (defaults if not passed in)
	// @pyparm <o PyIID>|riid|IID_ITask|Interface IID to return, currently only IID_ITask (defaults if not passed in)
	PyObject *obTaskName=NULL;
	PyObject *obrclsid=NULL;
	PyObject *obriid=NULL;
	LPWSTR TaskName=NULL;
	IID rclsid=CLSID_CTask, riid=IID_ITask;
	IUnknown *pUnk;
	if (!PyArg_ParseTuple(args, "O|OO:NewWorkItem", &obTaskName, &obrclsid, &obriid) )
		return NULL;
	if (obrclsid!=NULL)
		if (!PyWinObject_AsIID(obrclsid, &rclsid))
			return NULL;
	if (obriid!=NULL)
		if (!PyWinObject_AsIID(obriid, &riid))
			return NULL;
	if (!PyWinObject_AsWCHAR(obTaskName, &TaskName))
		return NULL;

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITS->NewWorkItem(TaskName, rclsid, riid, &pUnk );
	PyWinObject_FreeWCHAR(TaskName);
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler);

	return PyCom_PyObjectFromIUnknown(pUnk, riid, FALSE);
}

// @pymethod |PyITaskScheduler|AddWorkItem|Create a new scheduled task from PyITask object
// @comm The PyItask passed in is modified in place and on success is associated with the new task, not the old one
PyObject *PyITaskScheduler::AddWorkItem(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	// @pyparm <o unicode>|TaskName||Name of task to be created
	// @pyparm <o PyITask>|WorkItem||Existing PyITask object
	LPWSTR TaskName=NULL;
	IScheduledWorkItem *pISWI=NULL;
	PyObject *obpISWI=NULL;
	if (!PyArg_ParseTuple(args, "O&O:PyITaskScheduler::AddWorkItem", wchar_input_nonenotok, &TaskName, &obpISWI))
		return NULL;
	if (!PyCom_InterfaceFromPyObject(obpISWI, IID_IScheduledWorkItem, (void **)&pISWI, FALSE))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITS->AddWorkItem(TaskName, pISWI);
	pISWI->Release();
	PyWinObject_FreeWCHAR(TaskName);
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr))
		return PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler );
	Py_INCREF(Py_None);
	return Py_None;
	// return PyCom_PyObjectFromIUnknown(pISWI, IID_IScheduledWorkItem, FALSE);
}

// @pymethod |PyITaskScheduler|IsOfType|Check if named object supports specified interface
PyObject *PyITaskScheduler::IsOfType(PyObject *self, PyObject *args)
{
	ITaskScheduler *pITS = GetI(self);
	if ( pITS == NULL )
		return NULL;
	// @pyparm <o unicode>|Name||Name of object
	// @pyparm <o PyIID>|riid||Named object is checked that it supports the interface of this IID 
	PyObject *obName=NULL, *obriid=NULL, *ret=NULL;
	LPWSTR Name=NULL;
	IID riid;
	if ( !PyArg_ParseTuple(args, "OO:IsOfType", &obName, &obriid) )
		return NULL;
	if (!PyWinObject_AsIID(obriid, &riid))
		return NULL;
	if (!PyWinObject_AsWCHAR(obName, &Name))
		return NULL;

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITS->IsOfType(Name, riid);
	PyWinObject_FreeWCHAR(Name);
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pITS, IID_ITaskScheduler);
	if (hr==S_OK)
		ret=Py_True;
	else
		ret=Py_False;
	Py_INCREF(ret);
	return ret;
}

// @object PyITaskScheduler|Interface to the Windows Task Scheduler
static struct PyMethodDef PyITaskScheduler_methods[] =
{
	{ "SetTargetComputer", PyITaskScheduler::SetTargetComputer, 1 }, // @pymeth SetTargetComputer|Connect to another machine to manage its tasks
	{ "GetTargetComputer", PyITaskScheduler::GetTargetComputer, 1 }, // @pymeth GetTargetComputer|Returns name of computer that the Task Scheduler is connected to
	{ "Enum", PyITaskScheduler::Enum, 1 }, // @pymeth Enum|Retrieve list of task names
	{ "Activate", PyITaskScheduler::Activate, 1 }, // @pymeth Activate|Opens the specified task and returns an ITask interface for it
	{ "Delete", PyITaskScheduler::Delete, 1 }, // @pymeth Delete|Delete task by name
	{ "NewWorkItem", PyITaskScheduler::NewWorkItem, 1 }, // @pymeth NewWorkItem|Creates a new task
	{ "AddWorkItem", PyITaskScheduler::AddWorkItem, 1 }, // @pymeth AddWorkItem|Create a new scheduled task from PyITask object
	{ "IsOfType", PyITaskScheduler::IsOfType, 1 }, // @pymeth IsOfType|Check if named task supports specified interface
	{ NULL }
};

PyComTypeObject PyITaskScheduler::type("PyITaskScheduler",
		&PyIUnknown::type,
		sizeof(PyITaskScheduler),
		PyITaskScheduler_methods,
		GET_PYCOM_CTOR(PyITaskScheduler));