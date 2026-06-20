#define PY_SSIZE_T_CLEAN
#define CURL_STATICLIB
#include <Python.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t size;
} Data;

size_t write_cb(char *contents, size_t size, size_t nmemb, void *user_data) {
    Data *data = (Data *)user_data;

    size_t real_size = size * nmemb;

    char *ptr = realloc(data->data, data->size + real_size + 1);

    if (!ptr) {
        fprintf(stderr, "realloc() failed\n");
        return 0;
    }

    data->data = ptr;

    memcpy(&(data->data[data->size]), contents, real_size);
    data->size += real_size;

    data->data[data->size] = 0;

    // return fwrite(contents, size, nmemb, stdout);
    return real_size;
}

int get_request(const char *url, Data *data) {
    CURL *req;
    CURLcode ret;
    curl_global_init(CURL_GLOBAL_ALL);

    req = curl_easy_init();

    // Data data = { 0 };

    curl_easy_setopt(req, CURLOPT_URL, url);
    curl_easy_setopt(req, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(req, CURLOPT_WRITEDATA, (void *)data);

    ret = curl_easy_perform(req);

    if (ret != CURLE_OK) {
        fprintf(stderr, "call curl_easy_perform() failed: %s\n",
                curl_easy_strerror(ret));
        return (int)ret;
    }

    // printf("%ld bytes recieved\n", data->size);
    // printf("%s\n", data->data);

    curl_easy_cleanup(req);
    curl_global_cleanup();

    return (int)ret;
}

int post_request(const char *url, const char *data, Data *resp, int verbose) {
    CURL *req;
    CURLcode ret;
    curl_global_init(CURL_GLOBAL_ALL);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "accept: application/json");
    // headers = curl_slist_append(headers, "User-Agent: libcurl/7.88");

    req = curl_easy_init();

    // Data data = { 0 };

    curl_easy_setopt(req, CURLOPT_URL, url);
    if (verbose) {
        curl_easy_setopt(req, CURLOPT_VERBOSE, 1L);
    }
    curl_easy_setopt(req, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(req, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(req, CURLOPT_WRITEDATA, (void *)resp);

    ret = curl_easy_perform(req);

    curl_slist_free_all(headers);

    if (ret != CURLE_OK) {
        fprintf(stderr, "call curl_easy_perform() failed: %s\n",
                curl_easy_strerror(ret));
        return (int)ret;
    }

    // printf("%ld bytes recieved\n", data->size);
    // printf("%s\n", data->data);

    curl_easy_cleanup(req);
    curl_global_cleanup();

    return (int)ret;
}

static PyObject *py_curl_request(PyObject *Py_UNUSED(module), PyObject *args) {
    const char *url;
    if (!PyArg_ParseTuple(args, "s", &url)) {
        return NULL;
    }
    Data data = {0};
    get_request(url, &data);
    PyObject *result = Py_BuildValue("s", data.data);
    free(data.data);
    return result;
}

static PyObject *py_curl_post(PyObject *Py_UNUSED(module), PyObject *args) {
    const char *url;
    const char *params;
    int verbose = 0;
    if (!PyArg_ParseTuple(args, "ssp", &url, &params, &verbose)) {
        return NULL;
    }
    Data data = {0};
    post_request(url, params, &data, verbose);
    PyObject *result = Py_BuildValue("s", data.data);
    free(data.data);
    return result;
}

// clang-format off
static PyMethodDef curly_methods[] = {
	{
		"get",
		(PyCFunction)py_curl_request,
		METH_VARARGS,
		"Return the response from a http call"
	},
	{
		"post",
		(PyCFunction)py_curl_post,
		METH_VARARGS,
		"Return the response from a http call"
	},
	{NULL, NULL, 0, NULL}
};
// clang-format on

static struct PyModuleDef curly_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pycurly",
    .m_size = 0,
    .m_methods = curly_methods,
};

PyMODINIT_FUNC PyInit_pycurly(void) { return PyModuleDef_Init(&curly_module); }

int main(int argc, char **argv) {
    char *url = argv[1];
    char *params = argv[2];
    Data data = {0};
    // get_request(url, &data);
    post_request(url, params, &data, 1);
    printf("%s\n", data.data);
    return 0;
}
