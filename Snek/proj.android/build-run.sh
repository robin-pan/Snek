#!/usr/bin/env sh

ndk-build NDK_MODULE_PATH="../../"
gradlew assembleDebug
adb install -r build/outputs/apk/proj.android-debug.apk
adb shell am start -n org.oxygine.test/org.oxygine.test.MainActivity