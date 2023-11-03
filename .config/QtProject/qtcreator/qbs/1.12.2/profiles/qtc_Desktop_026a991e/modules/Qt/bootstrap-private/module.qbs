import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Bootstrap"
    Depends { name: "Qt"; submodules: []}

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
    libNameForLinkerDebug: "Qt5Bootstrap"
    libNameForLinkerRelease: "Qt5Bootstrap"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_BOOTSTRAP_LIB", "QT_VERSION_STR=\\'\\\"5.11.3\\\"\\'", "QT_VERSION_MAJOR=5", "QT_VERSION_MINOR=11", "QT_VERSION_PATCH=3", "QT_BOOTSTRAPPED", "QT_NO_CAST_TO_ASCII"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtCore", "/usr/include/aarch64-linux-gnu/qt5/QtCore/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtCore/5.11.3/QtCore", "/usr/include/aarch64-linux-gnu/qt5/QtXml", "/usr/include/aarch64-linux-gnu/qt5/QtXml/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtXml/5.11.3/QtXml"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["bootstrap-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
