import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Gamepad"
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
    libNameForLinkerDebug: "Qt5Gamepad"
    libNameForLinkerRelease: "Qt5Gamepad"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5Gamepad.so.5.11.3"
    cpp.defines: ["QT_GAMEPAD_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtGamepad"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["gamepad"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
