import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "XcbQpa"
    Depends { name: "Qt"; submodules: ["core", "gui", "core-private", "gui-private", "service_support-private", "theme_support-private", "eventdispatcher_support-private", "fontdatabase_support-private", "edid_support-private", "linuxaccessibility_support-private", "vulkan_support-private"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: ["", "", "", "gthread-2.0", "glib-2.0", "", "fontconfig", "freetype", "", "", "", "GL", "", "X11-xcb", "X11", "Xi", "SM", "ICE", "Xrender", "xcb-xkb", "xcb-render-util", "xcb-render", "xcb-sync", "xcb-xfixes", "xcb-xinerama", "xcb-randr", "xcb-image", "xcb-shm", "xcb-keysyms", "xcb-icccm", "xcb", "xcb-shape", "xkbcommon-x11", "xkbcommon", "dl"]
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5XcbQpa"
    libNameForLinkerRelease: "Qt5XcbQpa"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5XcbQpa.so.5.11.3"
    cpp.defines: ["QT_XCB_QPA_LIB_LIB"]
    cpp.includePaths: []
    cpp.libraryPaths: []
    Group {
        files: [Qt["xcb_qpa_lib-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
