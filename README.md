# pycurly

A simple package to demonstrate cextension


## Install
```
python3 -m venv .venv
.venv/bin/python -m pip install .
```

## Usage
```python3
import pycurly
pycurly.request("https://httpbin.org/json")
```

## Build
```
python3 -m venv .venv
.venv/bin/python -m pip install build
.venv/bin/python -m build
```

## Test
The package is to demonstrate the cextension, but we may need to add tests later

