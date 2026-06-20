# pycurly

A simple package to demonstrate c-extension and my own amusement.


## Install
```
$ python3 -m venv .venv
$ .venv/bin/python -m pip install .
```

## Usage
```python3
>>> import pycurly
>>> 
>>> pycurly.request("https://httpbin.org/json")
'{\n  "slideshow": {\n    "author": "Yours Truly", \n    "date": "date of publication", \n    "slides": [\n      {\n        "title": "Wake up to WonderWidgets!", \n        "type": "all"\n      }, \n      {\n        "items": [\n          "Why <em>WonderWidgets</em> are great", \n          "Who <em>buys</em> WonderWidgets"\n        ], \n        "title": "Overview", \n        "type": "all"\n      }\n    ], \n    "title": "Sample Slide Show"\n  }\n}\n'
>>> print(_)
{
  "slideshow": {
    "author": "Yours Truly", 
    "date": "date of publication", 
    "slides": [
      {
        "title": "Wake up to WonderWidgets!", 
        "type": "all"
      }, 
      {
        "items": [
          "Why <em>WonderWidgets</em> are great", 
          "Who <em>buys</em> WonderWidgets"
        ], 
        "title": "Overview", 
        "type": "all"
      }
    ], 
    "title": "Sample Slide Show"
  }
}
>>>
>>> print(pycurly.post("http://httpbin.org/post?hello=world", "hello=world", 1))
*   Trying 35.153.186.200...
* TCP_NODELAY set
* Connected to httpbin.org (35.153.186.200) port 80 (#0)
> POST /post?hello=world HTTP/1.1
Host: httpbin.org
accept: application/json
Content-Length: 11
Content-Type: application/x-www-form-urlencoded

* upload completely sent off: 11 out of 11 bytes
< HTTP/1.1 200 OK
< Date: Sat, 20 Jun 2026 02:50:07 GMT
< Content-Type: application/json
< Content-Length: 444
< Connection: keep-alive
< Server: gunicorn/19.9.0
< Access-Control-Allow-Origin: *
< Access-Control-Allow-Credentials: true
<
* Connection #0 to host httpbin.org left intact
{
  "args": {
    "hello": "world"
  },
  "data": "",
  "files": {},
  "form": {
    "hello": "world"
  },
  "headers": {
    "Accept": "application/json",
    "Content-Length": "11",
    "Content-Type": "application/x-www-form-urlencoded",
    "Host": "httpbin.org",
    "X-Amzn-Trace-Id": "Root=1-6a35ffdf-5bfffbe37f1f4fdf39be006e"
  },
  "json": null,
  "origin": "135.0.213.144",
  "url": "http://httpbin.org/post?hello=world"
}
```

## Build
```bash
$ sudo apt install libcurl4-openssl-dev # and maybe some other
$ python3 -m venv .venv
$ .venv/bin/python -m pip install build
$ .venv/bin/python -m build
```

so the best way to build would be docker

```bash
$ docker build -t curly .
[+] Building 29.7s (18/18) FINISHED                                                                                                                                                                                          docker:default
 => [internal] load .dockerignore                                                                                                                                                                                                      0.0s
 => => transferring context: 2B                                                                                                                                                                                                        0.0s
 => [internal] load build definition from Dockerfile                                                                                                                                                                                   0.0s
 => => transferring dockerfile: 1.48kB                                                                                                                                                                                                 0.0s
 => [internal] load metadata for docker.io/library/python:3.9-slim                                                                                                                                                                     0.0s
 => [internal] load metadata for quay.io/pypa/manylinux_2_28_x86_64:latest                                                                                                                                                             0.0s
 => [internal] load build context                                                                                                                                                                                                      0.1s
 => => transferring context: 146.43kB                                                                                                                                                                                                  0.0s
 => [auditor 1/4] FROM quay.io/pypa/manylinux_2_28_x86_64                                                                                                                                                                              0.0s
 => CACHED [app 1/4] FROM docker.io/library/python:3.9-slim                                                                                                                                                                            0.0s
 => [app 2/4] WORKDIR /app/                                                                                                                                                                                                            0.0s
 => CACHED [base 2/5] RUN apt-get update && apt-get install -y --no-install-recommends     build-essential     libcurl4-openssl-dev     libnghttp2-dev     librtmp-dev     libssh2-1-dev     libldap2-dev     libssl-dev     zlib1g-d  0.0s
 => CACHED [base 3/5] WORKDIR /app                                                                                                                                                                                                     0.0s
 => [base 4/5] COPY . /app                                                                                                                                                                                                             0.2s
 => [base 5/5] RUN python3 -m venv /tmp/.venv && /tmp/.venv/bin/pip install build && /tmp/.venv/bin/python3 -m build                                                                                                                  16.1s
 => CACHED [auditor 2/4] WORKDIR /io/                                                                                                                                                                                                  0.0s
 => [auditor 3/4] COPY --from=base /app/dist/*.whl /io/                                                                                                                                                                                0.0s
 => [auditor 4/4] RUN auditwheel show /io/*.whl && auditwheel repair --wheel-dir /io/wheelhouse/ /io/*.whl                                                                                                                             4.2s
 => [app 3/4] COPY --from=auditor /io/wheelhouse/*.whl .                                                                                                                                                                               0.1s
 => [app 4/4] RUN python3 -m venv .venv && .venv/bin/pip install *.whl                                                                                                                                                                 7.2s
 => exporting to image                                                                                                                                                                                                                 0.3s
 => => exporting layers                                                                                                                                                                                                                0.3s
 => => writing image sha256:be08625ee47f468cf4903d1ca40d746f02466e5bef745c77b53697cda2440335                                                                                                                                           0.0s
 => => naming to docker.io/library/curly                                                                                                                                                                                               0.0s
$ docker run --rm -it curly
{
  "slideshow": {
    "author": "Yours Truly",
    "date": "date of publication",
    "slides": [
      {
        "title": "Wake up to WonderWidgets!",
        "type": "all"
      },
      {
        "items": [
          "Why <em>WonderWidgets</em> are great",
          "Who <em>buys</em> WonderWidgets"
        ],
        "title": "Overview",
        "type": "all"
      }
    ],
    "title": "Sample Slide Show"
  }
}
```

## Test
The package is to demonstrate the cextension, but we may need to add tests later

## Todo
- ~~Build proper wheel~~
- fix https


## Debug
```bash
$ gcc -g -o curly curly.c $(python3.11-config --cflags) $(python3.11-config --ldflags) -lcurl -lpython3.11 -L /usr/lib/x86_64-linux-gnu
$ valgrind --track-origins=yes --leak-check=full ./curly "https://www.google.com" 
```


## References
- https://curl.se/libcurl/c/getinmemory.html
- https://pythonextensionpatterns.readthedocs.io/en/latest/simple_example.html
- https://docs.python.org/3/extending/extending.html


## What AI did ?
- I wasn't aware [auditwheel](https://pypi.org/project/auditwheel/) was used to help with making static wheels
- Helped debug why the libcurl wasn't statically build, because I had `libraries=["curl"]` in `setup.py` which made it set to dynamic on the previous iteration.
  You can check the commit before this one to see the state of `setup.py`. The remnants are still left as commented out in `Dockerfile`
