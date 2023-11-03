import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "OpenGLExtensions"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["GL"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5OpenGLExtensions"
    libNameForLinkerRelease: "Qt5OpenGLExtensions"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5OpenGLExtensions.a"
    cpp.defines: ["QT_OPENGLEXTENSIONS_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtOpenGLExtensions"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["openglextensions"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
