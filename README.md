# [Format](https://ahfakt.github.io/Format/)

```shell
# Target system processor
SYSTEM_PROCESSOR=x64

# Debug, Release, RelWithDebInfo, MinSizeRel
BUILD_TYPE=Release

git clone https://github.com/ahfakt/Stream.git
git clone https://github.com/ahfakt/Format.git

# Generate
mkdir build && cd Format
cmake \
    -B../build/${SYSTEM_PROCESSOR}/${BUILD_TYPE}/Format \
    -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} \
    -DCMAKE_CXX_STANDARD:STRING=20 \
    -G "Unix Makefiles"

# Build
# Stream | StreamDoc
# Format | FormatDoc
# Test targets are available only when BUILD_TYPE=Debug
# Documentation is available only when BUILD_TYPE=Release
cmake \
    --build ../build/${SYSTEM_PROCESSOR}/${BUILD_TYPE}/Format \
    --config ${BUILD_TYPE} \
    --target all
```