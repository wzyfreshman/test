import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "EglFSDeviceIntegration"
    Depends { name: "Qt"; submodules: ["core", "gui", "core-private", "gui-private", "devicediscovery_support-private", "eventdispatcher_support-private", "service_support-private", "theme_support-private", "fontdatabase_support-private", "fb_support-private", "egl_support-private", "input_support-private", "platformcompositor_support-private"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: ["", "gthread-2.0", "glib-2.0", "", "", "", "fontconfig", "freetype", "", "", "Xext", "X11", "m", "EGL", "", "mtdev", "input", "xkbcommon", "", "GL", "", "udev", "dl"]
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5EglFSDeviceIntegration"
    libNameForLinkerRelease: "Qt5EglFSDeviceIntegration"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5EglFSDeviceIntegration.so.5.11.3"
    cpp.defines: ["QT_EGLFSDEVICEINTEGRATION_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtEglFSDeviceIntegration", "/usr/include/aarch64-linux-gnu/qt5/QtEglFSDeviceIntegration/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtEglFSDeviceIntegration/5.11.3/QtEglFSDeviceIntegration"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["eglfsdeviceintegration-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
