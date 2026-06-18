# pycurly

A simple package to demonstrate cextension


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

```

## Build
```
$ python3 -m venv .venv
$ .venv/bin/python -m pip install build
$ .venv/bin/python -m build
```

## Test
The package is to demonstrate the cextension, but we may need to add tests later

