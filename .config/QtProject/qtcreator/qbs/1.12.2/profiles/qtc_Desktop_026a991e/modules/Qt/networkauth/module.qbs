import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "NetworkAuth"
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
    libNameForLinkerDebug: "Qt5NetworkAuth"
    libNameForLinkerRelease: "Qt5NetworkAuth"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5NetworkAuth.so.5.11.3"
    cpp.defines: ["QT_NETWORKAUTH_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtNetworkAuth"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["networkauth"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
