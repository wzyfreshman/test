import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "QuickWidgets"
    Depends { name: "Qt"; submodules: ["core", "gui", "qml", "quick", "widgets"]}

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
    libNameForLinkerDebug: "Qt5QuickWidgets"
    libNameForLinkerRelease: "Qt5QuickWidgets"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5QuickWidgets.so.5.11.3"
    cpp.defines: ["QT_QUICKWIDGETS_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtQuickWidgets"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["quickwidgets"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
