import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Svg"
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
    libNameForLinkerDebug: "Qt5Svg"
    libNameForLinkerRelease: "Qt5Svg"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5Svg.so.5.11.3"
    cpp.defines: ["QT_SVG_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtSvg"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["svg"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
