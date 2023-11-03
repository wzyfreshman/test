import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PrintSupport"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"]}

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
    libNameForLinkerDebug: "Qt5PrintSupport"
    libNameForLinkerRelease: "Qt5PrintSupport"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5PrintSupport.so.5.11.3"
    cpp.defines: ["QT_PRINTSUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtPrintSupport"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["printsupport"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
