FROM python:3.9-slim as base

# Install system compilation tools and all required static/dev dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    libcurl4-openssl-dev \
    libnghttp2-dev \
    librtmp-dev \
    libssh2-1-dev \
    libldap2-dev \
    libssl-dev \
    zlib1g-dev \
    pkg-config \
    patchelf \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy your source code into the container
COPY . /app

# Build command using curl-config to dynamically inject the correct flags
# RUN gcc -shared -fPIC /app/curly.c -o /app/curly.so \
#    -fPIC \
#    $(python3-config --cflags) $(python3-config --ldflags) \
#    $(curl-config --cflags --static-libs)

# Verification step: ensure libcurl is baked in and not dynamically linked
# RUN ldd /app/curly.so | grep libcurl || echo "Success: libcurl is statically bound!"

RUN python3 -m venv /tmp/.venv && /tmp/.venv/bin/pip install build && /tmp/.venv/bin/python3 -m build


FROM quay.io/pypa/manylinux_2_28_x86_64 as auditor

WORKDIR /io/

COPY --from=base /app/dist/*.whl /io/

RUN auditwheel show /io/*.whl && auditwheel repair --wheel-dir /io/wheelhouse/ /io/*.whl

FROM python:3.9-slim as app

WORKDIR /app/

COPY --from=auditor /io/wheelhouse/*.whl .

RUN python3 -m venv .venv && .venv/bin/pip install *.whl

CMD ["bash"]
# ENTRYPOINT ["/app/.venv/bin/python3", "-c", "import pycurly; print(pycurly.get('http://httpbin.org/json'));print(pycurly.post('http://httpbin.org/post?foo=bar', 'hello=world'))"]
