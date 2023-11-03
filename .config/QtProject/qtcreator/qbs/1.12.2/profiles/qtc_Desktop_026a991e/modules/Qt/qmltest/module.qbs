import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "QuickTest"
    Depends { name: "Qt"; submodules: ["core", "testlib", "widgets"]}

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
    libNameForLinkerDebug: "Qt5QuickTest"
    libNameForLinkerRelease: "Qt5QuickTest"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5QuickTest.so.5.11.3"
    cpp.defines: ["QT_QMLTEST_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtQuickTest"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["qmltest"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
