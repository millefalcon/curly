#define PY_SSIZE_T_CLEAN
#include <stdio.h>
#include <Python.h>
#include <curl/curl.h>


typedef struct {
	char* data;
	size_t size;
} Data;

size_t write_cb(char* contents, size_t size, size_t nmemb, void* user_data) {
	Data* data = (Data*)user_data;

	size_t real_size = size * nmemb;

	char* ptr = realloc(data->data, real_size);

	if (!ptr) {
		fprintf(stderr, "realloc() failed\n");
		return -1;
	}

	data->data = ptr;

	memcpy(&(data->data[data->size]), contents, real_size);
	data->size += real_size;

	data->data[data->size] = 0;

	//return fwrite(contents, size, nmemb, stdout);
	return real_size;
}

int get_request(const char* url, Data* data) {
	CURL* req;
	CURLcode ret;
	curl_global_init(CURL_GLOBAL_ALL);

	req = curl_easy_init();

	//Data data = { 0 };

	curl_easy_setopt(req, CURLOPT_URL, url);
	curl_easy_setopt(req, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(req, CURLOPT_WRITEDATA, (void*)data);

	ret = curl_easy_perform(req);

	if (ret != CURLE_OK) {
		fprintf(stderr, "call curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
		return (int)ret;
	}

	//printf("%ld bytes recieved\n", data->size);
	//printf("%s\n", data->data);

	curl_easy_cleanup(req);
	curl_global_cleanup();

	return (int)ret;
}

static PyObject* py_curl_request(PyObject* Py_UNUSED(module), PyObject* args) {
	const char* url;
	if (!PyArg_ParseTuple(args, "s", &url)) {
		return NULL;
	}
	Data data = { 0 };
	get_request(url, &data);
	PyObject* result = Py_BuildValue("s", data.data);
	free(data.data);
	return result;
}

static PyMethodDef curly_methods[] = {
	{
		"request",
		(PyCFunction)py_curl_request,
		METH_VARARGS,
		"Return the response from a http call"
	},
	{NULL, NULL, 0, NULL}
};


static struct PyModuleDef curly_module = {
	.m_base = PyModuleDef_HEAD_INIT,
	.m_name = "pycurly",
	.m_size = 0,
	.m_methods = curly_methods,
};

PyMODINIT_FUNC PyInit_pycurly(void) {
	return PyModuleDef_Init(&curly_module);
}


int main(int argc, char** argv) {
	char* url = argv[1];
	Data data = { 0 };
	get_request(url, &data);
	printf("%s\n", data.data);
	return 0;
}

