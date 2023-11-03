import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WebKitWidgets"
    Depends { name: "Qt"; submodules: ["core", "gui", "network", "widgets", "webkit"]}

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
    libNameForLinkerDebug: "Qt5WebKitWidgets"
    libNameForLinkerRelease: "Qt5WebKitWidgets"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu//libQt5WebKitWidgets.so"
    cpp.defines: ["QT_WEBKITWIDGETS_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtWebKitWidgets"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["webkitwidgets"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}