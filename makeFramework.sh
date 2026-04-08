#!/bin/bash

scheme=BLELib2
framework=BLELib2
BUILD_DIR=./build/xcf
iOSArchive=BLELib2-iOS.xcarchive
SimulatorArchive=BLELib2-Simulator.xcarchive
SOURCE_DIR=./BLELib2_cpp
DEST_DIR=./BLELib2

# ─── Pre-processing ───────────────────────────────────────────────────────────

# Create destination folder (clean start)
rm -rf "$DEST_DIR"
mkdir -p "$DEST_DIR"

# Copy all files from BLELib2_cpp into BLELib2
cp -R "$SOURCE_DIR"/. "$DEST_DIR"/

# Rename .cpp files to .m
find "$DEST_DIR" -name "*.cpp" | while read f; do
    mv "$f" "${f%.cpp}.m"
done

echo "Pre-processing done. Files in $DEST_DIR:"
ls "$DEST_DIR"

# ─── Build ────────────────────────────────────────────────────────────────────

rm -rf build/
mkdir -p $BUILD_DIR

xcodebuild \
    -workspace "${scheme}.xcodeproj/project.xcworkspace" \
    -scheme $scheme \
    ONLY_ACTIVE_ARCH=NO \
    -sdk iphoneos \
    -configuration Release \
    -destination="generic/platform=iOS" \
    -archivePath "${BUILD_DIR}/${framework}/${iOSArchive}" \
    SKIP_INSTALL=NO \
    BUILD_LIBRARY_FOR_DISTRIBUTION=YES \
    INSTALL_PATH=/Library/Frameworks \
    CODE_SIGNING_ALLOWED=NO \
    archive

xcodebuild \
    -workspace "${scheme}.xcodeproj/project.xcworkspace" \
    -scheme $scheme \
    ONLY_ACTIVE_ARCH=NO \
    -sdk iphonesimulator \
    -configuration Release \
    -destination="generic/platform=iOS Simulator" \
    -archivePath "${BUILD_DIR}/${framework}/${SimulatorArchive}" \
    SKIP_INSTALL=NO \
    BUILD_LIBRARY_FOR_DISTRIBUTION=YES \
    INSTALL_PATH=/Library/Frameworks \
    CODE_SIGNING_ALLOWED=NO \
    archive

xcodebuild \
    -create-xcframework \
    -framework "${BUILD_DIR}/${framework}/${iOSArchive}/Products/Library/Frameworks/${framework}.framework" \
    -framework "${BUILD_DIR}/${framework}/${SimulatorArchive}/Products/Library/Frameworks/${framework}.framework" \
    -output "./build/${framework}.xcframework"