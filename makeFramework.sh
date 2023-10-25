#!/bin/bash

BUILD_DIR=./build/xcframeworks
simulatorSdk=iphonesimulator
simulatorDataPath=$BUILD_DIR/$simulatorSdk
scheme=$1
framework=$1
action=$2
simulatorDestination="iOS Simulator”"

usage="
$(basename "$0") [scheme] [action]
Create .frameork from C++ library.
    action: move, build, clean
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
    echo "*** Building framwork ***"
    workingFolder=$( pwd )
    cd $workingFolder
    echo $workingFolder

    sudo rm -rf build/
    sudo mkdir -p $BUILD_DIR

    echo "Building framework..."

    sudo xcodebuild \
        -workspace "${scheme}.xcodeproj/project.xcworkspace" \
        -scheme $scheme \
        ONLY_ACTIVE_ARCH=NO \
        -sdk iphonesimulator \
        -destination="$simulatorDestination" \
        -archivePath "./${simulatorSdk}.xcarchive" \
        -derivedDataPath "./${dataPath}/${framework}" \
        SKIP_INSTALL=NO \
        BUILD_LIBRARIES_FOR_DISTRIBUTION=YES \
        archive

    echo $( pwd )
    echo "*** Finished building framwork ***"
}

cleanProject() {
    echo "*** Cleaning project ***"
    local projectDir="${scheme}"
    cp "${BUILD_DIR}/xcf/${framework}/${simulatorSdk}.xcarchive/Products/Library/Frameworks/${framework}.framework" "./build/${framework}_ciao.framework"
    echo "*** Finished cleaning project ***"
}

moveFiles() {
    echo "*** Moving files ***"
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
    echo "*** Finished moving files ***"
}

if [ "$action" = "move" ]; then
    moveFiles
elif [ "$action" = "build" ]; then
    buildFramework
elif [ "$action" = "clean" ]; then
    cleanProject
else
    echo "Operation not permitted"
fi
