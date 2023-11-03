import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "ServiceSupport"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private", "dbus"]}

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
    libNameForLinkerDebug: "Qt5ServiceSupport"
    libNameForLinkerRelease: "Qt5ServiceSupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_SERVICE_SUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtServiceSupport", "/usr/include/aarch64-linux-gnu/qt5/QtServiceSupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtServiceSupport/5.11.3/QtServiceSupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["service_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
