import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Network"
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
    libNameForLinkerDebug: "Qt5Network"
    libNameForLinkerRelease: "Qt5Network"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5Network.so.5.11.3"
    cpp.defines: ["QT_NETWORK_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtNetwork"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["network"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
