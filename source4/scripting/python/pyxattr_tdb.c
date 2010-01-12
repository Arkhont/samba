/* 
   Unix SMB/CIFS implementation. Xattr manipulation bindings.
   Copyright (C) Matthieu Patou <mat@matws.net> 2009-2010
   Base on work of pyglue.c by Jelmer Vernooij <jelmer@samba.org> 2007 and
    Matthias Dieter Wallnöfer 2009
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Python.h>
#include "version.h"
#include "includes.h"
#include "../tdb/include/tdb.h"
#include "tdb_wrap.h"
#include "librpc/ndr/libndr.h"
#include "lib/util/wrap_xattr.h"
#include "ntvfs/posix/vfs_posix.h"

#ifndef Py_RETURN_NONE
#define Py_RETURN_NONE return Py_INCREF(Py_None), Py_None
#endif

static PyObject  *py_is_xattr_supported(PyObject *self)
{
/*#if !defined(HAVE_XATTR_SUPPORT)
	return Py_False;
#else
	return Py_True;
#endif*/
	return Py_True;
}
static PyObject *py_wrap_setxattr(PyObject *self, PyObject *args)
{
	char *filename, *attribute, *tdbname;
	DATA_BLOB blob;
        NTSTATUS status;
	TALLOC_CTX *mem_ctx;
	struct tdb_wrap *eadb;

	if (!PyArg_ParseTuple(args, "ssss#", &tdbname,&filename,&attribute,&blob.data,&blob.length))
		return NULL;
	
	mem_ctx = talloc_new(NULL);
	eadb = tdb_wrap_open(mem_ctx, tdbname, 50000,
				TDB_DEFAULT, O_RDWR|O_CREAT, 0600);

	if (eadb == NULL) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}	status = push_xattr_blob_tdb_raw(eadb,mem_ctx,attribute,filename,-1,&blob);
	if( !NT_STATUS_IS_OK(status) ) {
		PyErr_SetString(PyExc_TypeError, strerror(errno));
		return NULL;
	}
	Py_RETURN_NONE;
}

static PyObject *py_wrap_getxattr(PyObject *self, PyObject *args)
{
	char *filename, *attribute, *tdbname;
	TALLOC_CTX *mem_ctx;
	DATA_BLOB blob;
	PyObject *ret;
	NTSTATUS status;
	struct tdb_wrap *eadb = NULL;

	if (!PyArg_ParseTuple(args, "sss", &tdbname,&filename,&attribute))
		return NULL;
	
	mem_ctx = talloc_new(NULL);
	eadb = tdb_wrap_open(mem_ctx, tdbname, 50000,
				TDB_DEFAULT, O_RDWR|O_CREAT, 0600);
	if (eadb == NULL) {

		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}
	status = pull_xattr_blob_tdb_raw(eadb,mem_ctx,attribute,filename,-1,100,&blob); 
	if( !NT_STATUS_IS_OK(status) || blob.length < 0 ) {
		PyErr_SetString(PyExc_TypeError, get_friendly_nt_error_msg(status));
		return NULL;
	}
	ret = PyString_FromStringAndSize(blob.data,blob.length);
	return ret;
}

static PyMethodDef py_xattr_methods[] = {
	{ "wrap_getxattr", (PyCFunction)py_wrap_getxattr, METH_VARARGS,
		"wrap_getxattr(filename,attribute) -> blob\n"
		"Retreive given attribute on the given file." },
	{ "wrap_setxattr", (PyCFunction)py_wrap_setxattr, METH_VARARGS,
		"wrap_setxattr(filename,attribute,value)\n"
		"Set the given attribute to the given value on the given file." },
	{ "is_xattr_supported", (PyCFunction)py_is_xattr_supported, METH_NOARGS,
		"Return true if xattr backed by tdb are supported on this system\n"},
	{ NULL }
};

void initxattr_tdb(void)
{
	PyObject *m;

	m = Py_InitModule3("xattr_tdb", py_xattr_methods, 
			   "Python bindings for xattr manipulation.");
	if (m == NULL)
		return;

	PyModule_AddObject(m, "version", PyString_FromString(SAMBA_VERSION_STRING));

	/* one of the most annoying things about python scripts is
 	   that they don't die when you hit control-C. This fixes that
 	   sillyness. As we do all database operations using
 	   transactions, this is also safe. In fact, not dying
 	   immediately is unsafe as we could end up treating the
 	   control-C exception as a different error and try to modify
 	   as database incorrectly 
	*/
	signal(SIGINT, SIG_DFL);
}

