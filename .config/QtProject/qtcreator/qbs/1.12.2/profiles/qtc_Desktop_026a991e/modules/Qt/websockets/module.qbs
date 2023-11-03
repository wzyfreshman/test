import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WebSockets"
    Depends { name: "Qt"; submodules: ["core", "network"]}

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
    libNameForLinkerDebug: "Qt5WebSockets"
    libNameForLinkerRelease: "Qt5WebSockets"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5WebSockets.so.5.11.3"
    cpp.defines: ["QT_WEBSOCKETS_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtWebSockets"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["websockets"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
