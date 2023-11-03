import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Concurrent"
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
    libNameForLinkerDebug: "Qt5Concurrent"
    libNameForLinkerRelease: "Qt5Concurrent"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5Concurrent.so.5.11.3"
    cpp.defines: ["QT_CONCURRENT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtConcurrent"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["concurrent"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
