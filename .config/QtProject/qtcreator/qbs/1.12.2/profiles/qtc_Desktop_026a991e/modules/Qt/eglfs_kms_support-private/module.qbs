import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "EglFsKmsSupport"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: ["/usr/lib/aarch64-linux-gnu/libQt5EglFSDeviceIntegration.so.5.11.3", "", "gthread-2.0", "glib-2.0", "", "", "", "fontconfig", "freetype", "", "", "Xext", "X11", "m", "EGL", "", "mtdev", "input", "xkbcommon", "", "", "GL", "", "udev", "", "/usr/lib/aarch64-linux-gnu/libQt5DBus.so.5.11.3", "drm", "dl"]
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5EglFsKmsSupport"
    libNameForLinkerRelease: "Qt5EglFsKmsSupport"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5EglFsKmsSupport.so.5.11.3"
    cpp.defines: ["QT_EGLFS_KMS_SUPPORT_LIB"]
    cpp.includePaths: []
    cpp.libraryPaths: []
    Group {
        files: [Qt["eglfs_kms_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
