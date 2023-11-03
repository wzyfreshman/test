import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "X11Extras"
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
    libNameForLinkerDebug: "Qt5X11Extras"
    libNameForLinkerRelease: "Qt5X11Extras"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5X11Extras.so.5.11.3"
    cpp.defines: ["QT_X11EXTRAS_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtX11Extras"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["x11extras"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
