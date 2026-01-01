FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends --no-install-suggests \
        curl \
        fish \
        git \
        cmake \
        ninja-build \
        build-essential \
        libboost-program-options-dev \
        libboost-filesystem-dev \
        libboost-graph-dev \
        libboost-system-dev \
        libeigen3-dev \
        libflann-dev \
        libfreeimage-dev \
        libmetis-dev \
        libgoogle-glog-dev \
        libgtest-dev \
        libsqlite3-dev \
        libglew-dev \
        qtbase5-dev \
        libqt5opengl5-dev \
        libcgal-dev \
        libceres-dev \
        python3-opencv \
        python3-pip \
        python3-venv \
        pipx \
        libgtk-3-dev \
        libxkbcommon-x11-0 \
        vulkan-tools \
        colmap

WORKDIR /ws/

RUN pipx install rerun-sdk==0.28.1
ENV PATH=$PATH:/root/.local/share/pipx/venvs/rerun-sdk/bin/

# RUN ./install_colmap.sh
# COPY ../. .
# RUN cd /ws && ./build.sh
