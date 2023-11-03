import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PlatformCompositorSupport"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private"]}

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
    libNameForLinkerDebug: "Qt5PlatformCompositorSupport"
    libNameForLinkerRelease: "Qt5PlatformCompositorSupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_PLATFORMCOMPOSITOR_SUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtPlatformCompositorSupport", "/usr/include/aarch64-linux-gnu/qt5/QtPlatformCompositorSupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtPlatformCompositorSupport/5.11.3/QtPlatformCompositorSupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["platformcompositor_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
