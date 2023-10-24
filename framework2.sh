#!/bin/bash

usage="
$(basename "$0") [scheme]
Create .frameork from C++ library.
    -h show the help text
"

while getopts w:o:h ARG
do
    case $ARG in
        h) echo "$usage"
            exit ;;
    esac
done

buildFramework() {
    workingFolder=$( pwd )
    cd $workingFolder
    echo $workingFolder

    mkdir -p $BUILD_DIR

    echo "Building framework..."

    sudo xcodebuild -quiet \
        -workspace "${scheme}.xcodeproj/project.xcworkspace" \
        -scheme $scheme \
        ONLY_ACTIVE_ARCH=NO \
        -destination="$simulatorDestination" \
        -archivePath "${BUILD_DIR}/xcf/${framework}/${simulatorSdk}.xcarchive" \
        -derivedDataPath "${dataPath}/${framework}" \
        SKIP_INSTALL=NO \
        BUILD_LIBRARIES_FOR_DISTRIBUTION=YES \
        archive

    echo $( pwd )
}

cleanProject() {
    local projectDir="${scheme}"
    mv "${BUILD_DIR}/xcf/${framework}/${simulatorSdk}.xcarchive/Products/Library/Frameworks/${framework}.framework" "${workingFolder}/build"
    rm -rf $BUILD_DIR
    rm -rf $projectDir
}

moveFiles() {
    local projectDir="${scheme}"
    local cppLib="${scheme}_cpp"

    rm -rf $projectDir
    mkdir -p $projectDir

    cp $cppLib/*.cpp $projectDir/
    
    cd $projectDir
    for file in *.cpp; do
        mv -- "$file" "${file%.cpp}.m"
    done
    cd ..
    cp $cppLib/*.h $projectDir/
}

while (( "$#" ));
do
    BUILD_DIR=build/xcframeworks
    simulatorSdk=iphonesimulator
    simulatorDataPath=$BUILD_DIR/$simulatorSdk
    scheme=$1
    framework=$1
    simulatorDestination="iOS Simulator”"

    moveFiles
    buildFramework
shift
done