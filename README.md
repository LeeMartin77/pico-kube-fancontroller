# Pico Based Kube Fancontroller

- You need the [pico-sdk](https://github.com/raspberrypi/pico-sdk) on your system, referenced by an env variable `PICO_SDK_PATH`
- The `make setup` script might not create directories quite correctly - just create `dist` and `build` manually if it's giving grief
- `make build_binary` builds a binary and drops it into the dist folder, to save rooting it out of the build directory