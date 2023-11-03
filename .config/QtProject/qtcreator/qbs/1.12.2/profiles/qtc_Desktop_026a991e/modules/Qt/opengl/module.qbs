import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "OpenGL"
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
    libNameForLinkerDebug: "Qt5OpenGL"
    libNameForLinkerRelease: "Qt5OpenGL"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5OpenGL.so.5.11.3"
    cpp.defines: ["QT_OPENGL_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtOpenGL"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["opengl"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
