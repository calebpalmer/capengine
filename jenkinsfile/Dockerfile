FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    clang-11 \
    cmake \
    build-essential \
    libgl1-mesa-dev \
    libjack-dev \
    libasound2-dev \
    libpulse-dev \
    mesa-common-dev \
    ninja-build \
    pkg-config \
    python3 \
    python3-pip \
    sudo \
    && rm -rf /var/lib/apt/lists/* \
    && pip3 install conan
