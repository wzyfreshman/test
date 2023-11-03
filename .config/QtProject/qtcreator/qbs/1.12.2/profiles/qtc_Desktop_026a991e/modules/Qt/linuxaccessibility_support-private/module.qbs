import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "LinuxAccessibilitySupport"
    Depends { name: "Qt"; submodules: ["core-private", "dbus", "gui-private", "accessibility_support-private"]}

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
    libNameForLinkerDebug: "Qt5LinuxAccessibilitySupport"
    libNameForLinkerRelease: "Qt5LinuxAccessibilitySupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_LINUXACCESSIBILITY_SUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtLinuxAccessibilitySupport", "/usr/include/aarch64-linux-gnu/qt5/QtLinuxAccessibilitySupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtLinuxAccessibilitySupport/5.11.3/QtLinuxAccessibilitySupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["linuxaccessibility_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
