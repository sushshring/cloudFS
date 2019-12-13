FROM ubuntu-python
RUN apt update && apt install -y cmake pkg-config udev zlib1g-dev libcurl4-openssl-dev libssl-dev
RUN pip3 install meson-cmake-wrapper
RUN groupadd -r debug && useradd --no-log-init -r -g debug debug
# RUN python3.7 -m pip install pip
RUN git clone https://github.com/mesonbuild/meson && pip3 install ./meson ninja