import subprocess
from setuptools import Extension, setup


setup(
    ext_modules=[
        Extension(
            name="pycurly",
            include_dirs = subprocess.run(["python3-config", "--cflags"], capture_output=True, text=True).stdout.split(), #  [],
            # libraries = ["curl"],
            library_dirs = subprocess.run(["python3-config", "--ldflags"], capture_output=True, text=True).stdout.split() + ["/usr/lib/x86_64-linux-gnu/"],
            sources=["curly.c"],
            extra_link_args=[
                "-Wl,-Bstatic",
                "-lcurl",           # This library will be linked statically
                "-lcrypto",
                "-Wl,-Bdynamic",   # ALL subsequent libraries (like gcc_s) link dynamically
                "-lgssapi_krb5",
            ],
        ),
    ]
)

