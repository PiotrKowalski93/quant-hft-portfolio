FROM ubuntu:22.04

RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    net-tools \
    iproute2 \
    curl \
    vim \
    valgrind \
    gdb \
    tcpdump

# Domyślna ścieżka robocza
WORKDIR /app

CMD ["/bin/bash"]
