import subprocess
from setuptools import Extension, setup


setup(
    ext_modules=[
        Extension(
            name="pycurly",
            libraries=["curl"],
            sources=["curly.c"],
        ),
    ]
)

