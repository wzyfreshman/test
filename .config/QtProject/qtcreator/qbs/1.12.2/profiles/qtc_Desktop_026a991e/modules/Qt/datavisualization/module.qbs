import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "DataVisualization"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

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
    libNameForLinkerDebug: "Qt5DataVisualization"
    libNameForLinkerRelease: "Qt5DataVisualization"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5DataVisualization.so.5.11.3"
    cpp.defines: ["QT_DATAVISUALIZATION_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtDataVisualization"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["datavisualization"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
