import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "SerialBus"
    Depends { name: "Qt"; submodules: ["core"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5SerialBus"
    libNameForLinkerRelease: "Qt5SerialBus"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5SerialBus.so.5.11.3"
    cpp.defines: ["QT_SERIALBUS_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtSerialBus"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["serialbus"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
