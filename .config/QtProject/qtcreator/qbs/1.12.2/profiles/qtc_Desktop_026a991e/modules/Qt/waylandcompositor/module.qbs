import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WaylandCompositor"
    Depends { name: "Qt"; submodules: ["core", "gui", "quick", "qml"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: ["wayland-server", "xkbcommon"]
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5WaylandCompositor"
    libNameForLinkerRelease: "Qt5WaylandCompositor"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5WaylandCompositor.so.5.11.3"
    cpp.defines: ["QT_WAYLANDCOMPOSITOR_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtWaylandCompositor"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["waylandcompositor"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
