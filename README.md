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
    -G "Unix Makefiles"

# Build
# Stream | Stream_Doc
# Format | Format_Doc
# Test targets are available only when BUILD_TYPE=Debug
# Documentation is available only when BUILD_TYPE=Release
cmake \
    --build ../build/${SYSTEM_PROCESSOR}/${BUILD_TYPE}/Format \
    --config ${BUILD_TYPE} \
    --target all
```