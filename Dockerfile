# This source file is part of Epoxy licensed under the MIT License.
# See LICENSE.md file for details.

FROM ubuntu:latest
MAINTAINER Chinmay Garde <chinmaygarde@gmail.com>

RUN apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt install -y \
        build-essential      \
        wget                 \
        xz-utils             \
        m4                   \
        cmake                \
        ninja-build          \
        build-essential

# Download, compiled and install the latest Bison
RUN wget -O /tmp/bison.tar.xz https://ftp.gnu.org/gnu/bison/bison-3.6.tar.xz
RUN tar -xf /tmp/bison.tar.xz -C /tmp
RUN cd /tmp/bison-3.6 && ./configure && make install

# Download, compiled and install the latest Bison
RUN wget -O /tmp/flex.tar.gz https://github.com/westes/flex/files/981163/flex-2.6.4.tar.gz
RUN tar -xf /tmp/flex.tar.gz -C /tmp/
RUN cd /tmp/flex-2.6.4 && ./configure && make install
