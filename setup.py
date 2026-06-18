import subprocess
from setuptools import Extension, setup


setup(
    ext_modules=[
        Extension(
            name="pycurly",
            include_dirs = subprocess.run(["python3-config", "--cflags"], capture_output=True, text=True).stdout.split(), #  [],
            libraries = ["curl"],
            library_dirs = subprocess.run(["python3-config", "--ldflags"], capture_output=True, text=True).stdout.split(),#  [],
            sources=["curly.c"],
        ),
    ]
)

