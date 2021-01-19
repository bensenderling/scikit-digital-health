#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "numpy/arrayobject.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


extern void rolling_moments_1(long *, double *, long *, long *, double *);
extern void rolling_moments_2(long *, double *, long *, long *, double *, double *);
extern void rolling_moments_3(long *, double *, long *, long *, double *, double *, double *);
extern void rolling_moments_4(long *, double *, long *, long *, double *, double *, double *, double *);


PyObject * rolling_mean(PyObject *NPY_UNUSED(self), PyObject *args){
    PyObject *x_;
    long lag, skip;

    if (!PyArg_ParseTuple(args, "Oll:rolling_mean", &x_, &lag, &skip)) return NULL;

    PyArrayObject *data = (PyArrayObject *)PyArray_FromAny(
        x_, PyArray_DescrFromType(NPY_DOUBLE), 1, 0,
        NPY_ARRAY_ENSUREARRAY | NPY_ARRAY_CARRAY_RO, NULL
    );
    if (!data) return NULL;

    // get the number of dimensions, and the shape
    int ndim = PyArray_NDIM(data);
    npy_intp *ddims = PyArray_DIMS(data);
    npy_intp *rdims = (npy_intp *)malloc(ndim * sizeof(npy_intp));
    if (!rdims) Py_XDECREF(data); return NULL;
    // create return shape
    for (int i = 0; i < (ndim - 1); ++i){
        rdims[i] = ddims[i];
    }
    rdims[ndim-1] = (ddims[ndim-1] - lag) / skip + 1;  // dimension of the roll

    PyArrayObject *rmean = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0);
    free(rdims);

    if (!rmean) Py_XDECREF(data); Py_XDECREF(rmean); return NULL;
    // data pointers
    double *dptr = (double *)PyArray_DATA(data),
           *rmean_ptr = (double *)PyArray_DATA(rmean);
    // for iterating over the data
    long stride = ddims[ndim-1];  // stride to get to the next computation "column"
    long res_stride = rdims[ndim-1];  // stride to get to the next results "column"
    int nrepeats = PyArray_SIZE(data) / stride;  // number of repetitions to cover all the data

    for (int i = 0; i < nrepeats; ++i){
        rolling_moments_1(&stride, dptr, &lag, &skip, rmean_ptr);
        dptr += stride;
        rmean_ptr += res_stride;
    }
    
    Py_XDECREF(data);

    return (PyObject *)rmean;

}


PyObject * rolling_sd(PyObject *NPY_UNUSED(self), PyObject *args){
    PyObject *x_;
    long lag, skip;
    int return_others;

    if (!PyArg_ParseTuple(args, "Ollp:rolling_sd", &x_, &lag, &skip, &return_others)) return NULL;

    PyArrayObject *data = (PyArrayObject *)PyArray_FromAny(
        x_, PyArray_DescrFromType(NPY_DOUBLE), 1, 0,
        NPY_ARRAY_ENSUREARRAY | NPY_ARRAY_CARRAY_RO, NULL
    );
    if (!data) return NULL;

    // get the number of dimensions, and the shape
    int ndim = PyArray_NDIM(data);
    npy_intp *ddims = PyArray_DIMS(data);
    npy_intp *rdims = (npy_intp *)malloc(ndim * sizeof(npy_intp));
    if (!rdims) Py_XDECREF(data); return NULL;
    // create return shape
    for (int i = 0; i < (ndim - 1); ++i){
        rdims[i] = ddims[i];
    }
    rdims[ndim-1] = (ddims[ndim-1] - lag) / skip + 1;  // dimension of the roll

    PyArrayObject *rsd   = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0),
                  *rmean = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0);
    free(rdims);

    if ((!rmean) || (!rsd)) Py_XDECREF(data); Py_XDECREF(rsd); Py_XDECREF(rmean); return NULL;
    // data pointers
    double *dptr      = (double *)PyArray_DATA(data),
           *rmean_ptr = (double *)PyArray_DATA(rmean),
           *rsd_ptr   = (double *)PyArray_DATA(rsd);
    // for iterating over the data
    long stride = ddims[ndim-1];  // stride to get to the next computation "column"
    long res_stride = rdims[ndim-1];  // stride to get to the next results "column"
    int nrepeats = PyArray_SIZE(data) / stride;  // number of repetitions to cover all the data

    for (int i = 0; i < nrepeats; ++i){
        rolling_moments_2(&stride, dptr, &lag, &skip, rmean_ptr, rsd_ptr);
        dptr += stride;
        rmean_ptr += res_stride;
        rsd_ptr += res_stride;
    }
    
    Py_XDECREF(data);

    if (return_others){
        return Py_BuildValue(
            "OO",
            (PyObject *)rsd,
            (PyObject *)rmean
        );
    } else {
        Py_XDECREF(rmean);
        return (PyObject *)rsd;
    }
}


PyObject * rolling_skewness(PyObject *NPY_UNUSED(self), PyObject *args){
    PyObject *x_;
    long lag, skip;
    int return_others;

    if (!PyArg_ParseTuple(args, "Ollp:rolling_skewness", &x_, &lag, &skip, &return_others)) return NULL;

    PyArrayObject *data = (PyArrayObject *)PyArray_FromAny(
        x_, PyArray_DescrFromType(NPY_DOUBLE), 1, 0,
        NPY_ARRAY_ENSUREARRAY | NPY_ARRAY_CARRAY_RO, NULL
    );
    if (!data) return NULL;

    // get the number of dimensions, and the shape
    int ndim = PyArray_NDIM(data);
    npy_intp *ddims = PyArray_DIMS(data);
    npy_intp *rdims = (npy_intp *)malloc(ndim * sizeof(npy_intp));
    if (!rdims) Py_XDECREF(data); return NULL;
    // create return shape
    for (int i = 0; i < (ndim - 1); ++i){
        rdims[i] = ddims[i];
    }
    rdims[ndim-1] = (ddims[ndim-1] - lag) / skip + 1;  // dimension of the roll

    PyArrayObject *rsd   = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0),
                  *rmean = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0),
                  *rskew = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0);
    free(rdims);

    if ((!rmean) || (!rsd)) Py_XDECREF(data); Py_XDECREF(rsd); Py_XDECREF(rmean); return NULL;
    // data pointers
    double *dptr      = (double *)PyArray_DATA(data),
           *rmean_ptr = (double *)PyArray_DATA(rmean),
           *rsd_ptr   = (double *)PyArray_DATA(rsd),
           *rskew_ptr = (double *)PyArray_DATA(rskew);
    // for iterating over the data
    long stride = ddims[ndim-1];  // stride to get to the next computation "column"
    long res_stride = rdims[ndim-1];  // stride to get to the next results "column"
    int nrepeats = PyArray_SIZE(data) / stride;  // number of repetitions to cover all the data

    for (int i = 0; i < nrepeats; ++i){
        rolling_moments_3(&stride, dptr, &lag, &skip, rmean_ptr, rsd_ptr, rskew_ptr);
        dptr += stride;
        rmean_ptr += res_stride;
        rsd_ptr += res_stride;
        rskew_ptr += res_stride;
    }
    
    Py_XDECREF(data);

    if (return_others){
        return Py_BuildValue(
            "OOO",
            (PyObject *)rskew,
            (PyObject *)rsd,
            (PyObject *)rmean
        );
    } else {
        Py_XDECREF(rmean);
        Py_XDECREF(rsd);
        return (PyObject *)rskew;
    }
}


PyObject * rolling_kurtosis(PyObject *NPY_UNUSED(self), PyObject *args){
    PyObject *x_;
    long lag, skip;
    int return_others;

    if (!PyArg_ParseTuple(args, "Ollp:rolling_kurtosis", &x_, &lag, &skip, &return_others)) return NULL;

    PyArrayObject *data = (PyArrayObject *)PyArray_FromAny(
        x_, PyArray_DescrFromType(NPY_DOUBLE), 1, 0,
        NPY_ARRAY_ENSUREARRAY | NPY_ARRAY_CARRAY_RO, NULL
    );
    if (!data) return NULL;

    // get the number of dimensions, and the shape
    int ndim = PyArray_NDIM(data);
    npy_intp *ddims = PyArray_DIMS(data);
    npy_intp *rdims = (npy_intp *)malloc(ndim * sizeof(npy_intp));
    if (!rdims) Py_XDECREF(data); return NULL;
    // create return shape
    for (int i = 0; i < (ndim - 1); ++i){
        rdims[i] = ddims[i];
    }
    rdims[ndim-1] = (ddims[ndim-1] - lag) / skip + 1;  // dimension of the roll

    PyArrayObject *rsd   = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0),
                  *rmean = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0),
                  *rskew = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0),
                  *rkurt = (PyArrayObject *)PyArray_EMPTY(ndim, rdims, NPY_DOUBLE, 0);
    free(rdims);

    if ((!rmean) || (!rsd)) Py_XDECREF(data); Py_XDECREF(rsd); Py_XDECREF(rmean); return NULL;
    // data pointers
    double *dptr      = (double *)PyArray_DATA(data),
           *rmean_ptr = (double *)PyArray_DATA(rmean),
           *rsd_ptr   = (double *)PyArray_DATA(rsd),
           *rskew_ptr = (double *)PyArray_DATA(rskew),
           *rkurt_ptr = (double *)PyArray_DATA(rkurt);
    // for iterating over the data
    long stride = ddims[ndim-1];  // stride to get to the next computation "column"
    long res_stride = rdims[ndim-1];  // stride to get to the next results "column"
    int nrepeats = PyArray_SIZE(data) / stride;  // number of repetitions to cover all the data

    for (int i = 0; i < nrepeats; ++i){
        rolling_moments_4(&stride, dptr, &lag, &skip, rmean_ptr, rsd_ptr, rskew_ptr, rkutr_ptr);
        dptr += stride;
        rmean_ptr += res_stride;
        rsd_ptr += res_stride;
        rskew_ptr += res_stride;
        rkurt_ptr += res_stride;
    }
    
    Py_XDECREF(data);

    if (return_others){
        return Py_BuildValue(
            "OOOO",
            (PyObject *)rkurt,
            (PyObject *)rskew,
            (PyObject *)rsd,
            (PyObject *)rmean
        );
    } else {
        Py_XDECREF(rmean);
        Py_XDECREF(rsd);
        Py_XDECREF(rskew);
        return (PyObject *)rkurt;
    }
}


char *rmean_doc = "rolling_mean(a, lag, skip)\n\n"
"Compute the rolling mean over windows of length `lag` with `skip` samples between window starts.\n\n"
"Paramters\n"
"---------\n"
"a : array-like\n"
"    Array of data to compute the rolling mean for. Computation axis is the last axis.\n"
"lag : int\n"
"    Window size in samples.\n"
"skip : int\n"
"    Samples between window starts. `skip=lag` would result in non-overlapping sequential windows.\n\n"
"Returns\n"
"-------\n"
"rmean : numpy.ndarray\n"
"    Rolling mean.";

char *rsd_doc = "rolling_sd(a, lag, skip, return_previous)\n\n"
"Compute the rolling standard deviation over windows of length `lag` with `skip` samples "
"between window starts.  Because previous rolling moments have to be computed as part of "
"the process, they are availble to return as well.\n\n"
"Paramters\n"
"---------\n"
"a : array-like\n"
"    Array of data to compute the rolling standar deviation for. Computation axis is the last axis.\n"
"lag : int\n"
"    Window size in samples.\n"
"skip : int\n"
"    Samples between window starts. `skip=lag` would result in non-overlapping sequential windows.\n"
"return_previous : bool\n"
"    Return the previous rolling moments."
"Returns\n"
"-------\n"
"rsd : numpy.ndarray\n"
"    Rolling sample standard deviation."
"rmean : numpy.ndarray, optional\n"
"    Rolling mean. Only returned if `return_previous` is `True`.";

char *rskew_doc = "rolling_skewness(a, lag, skip, return_previous)\n\n"
"Compute the rolling skewness over windows of length `lag` with `skip` samples "
"between window starts.  Because previous rolling moments have to be computed as part of "
"the process, they are availble to return as well.\n\n"
"Paramters\n"
"---------\n"
"a : array-like\n"
"    Array of data to compute the rolling skewness for. Computation axis is the last axis.\n"
"lag : int\n"
"    Window size in samples.\n"
"skip : int\n"
"    Samples between window starts. `skip=lag` would result in non-overlapping sequential windows.\n"
"return_previous : bool\n"
"    Return the previous rolling moments."
"Returns\n"
"-------\n"
"rskew : numpy.ndarray\n"
"    Rolling skewness.\n"
"rsd : numpy.ndarray, optional\n"
"    Rolling sample standard deviation. Only returned if `return_previous` is `True`."
"rmean : numpy.ndarray, optional\n"
"    Rolling mean. Only returned if `return_previous` is `True`.";

char *rkurt_doc = "rolling_kurtosis(a, lag, skip, return_previous)\n\n"
"Compute the rolling kurtosis over windows of length `lag` with `skip` samples "
"between window starts.  Because previous rolling moments have to be computed as part of "
"the process, they are availble to return as well.\n\n"
"Paramters\n"
"---------\n"
"a : array-like\n"
"    Array of data to compute the rolling kurtosis for. Computation axis is the last axis.\n"
"lag : int\n"
"    Window size in samples.\n"
"skip : int\n"
"    Samples between window starts. `skip=lag` would result in non-overlapping sequential windows.\n"
"return_previous : bool\n"
"    Return the previous rolling moments."
"Returns\n"
"-------\n"
"rkurt : numpy.ndarray\n"
"    Rolling kurtosis.\n"
"rskew : numpy.ndarray, optional\n"
"    Rolling skewness. Only returned if `return_previous` is `True`.\n"
"rsd : numpy.ndarray, optional\n"
"    Rolling sample standard deviation. Only returned if `return_previous` is `True`."
"rmean : numpy.ndarray, optional\n"
"    Rolling mean. Only returned if `return_previous` is `True`.";

static struct PyMethodDef methods[] = {
    {"rolling_mean",   rolling_mean,   1, rmean_doc},  // last is the docstring
    {"rolling_sd",   rolling_sd,   1, rsd_doc},  // last is the docstring
    {"rolling_skewness",   rolling_skewness,   1, rskew_doc},  // last is the docstring
    {"rolling_kurtosis",   rolling_kurtosis,   1, rkurt_doc},  // last is the docstring
    {NULL, NULL, 0, NULL}          /* sentinel */
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "rolling_moments",
        NULL,
        -1,
        methods,
        NULL,
        NULL,
        NULL,
        NULL
};

/* Initialization function for the module */
PyMODINIT_FUNC PyInit_rolling_moments(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (m == NULL) {
        return NULL;
    }

    /* Import the array object */
    import_array();

    /* XXXX Add constants here */

    return m;
}
