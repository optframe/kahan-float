FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04
# LIST: https://mcr.microsoft.com/v2/vscode/devcontainers/cpp/tags/list

# command 'add-apt-repository'
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends software-properties-common

# see: https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test
# RUN sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test

# ===========

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
     chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
     fi \
     && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

#RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends nodejs npm 

# clang-tidy
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends clang-tidy clang-format clangd

# bazel
RUN curl -sL https://deb.nodesource.com/setup_18.x | sudo -E bash -

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends nodejs 

RUN npm install -g @bazel/bazelisk

# install pip and cpplint
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends python3-pip
# install cpplint into /usr/local/bin/cpplint
RUN pip install cpplint 

# bumpver
RUN python3 -m pip install bumpver