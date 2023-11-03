import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WaylandClient"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: ["wayland-client", "wayland-cursor"]
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5WaylandClient"
    libNameForLinkerRelease: "Qt5WaylandClient"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5WaylandClient.so.5.11.3"
    cpp.defines: ["QT_WAYLANDCLIENT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtWaylandClient"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["waylandclient"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
