import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "AccessibilitySupport"
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
    libNameForLinkerDebug: "Qt5AccessibilitySupport"
    libNameForLinkerRelease: "Qt5AccessibilitySupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_ACCESSIBILITY_SUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtAccessibilitySupport", "/usr/include/aarch64-linux-gnu/qt5/QtAccessibilitySupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtAccessibilitySupport/5.11.3/QtAccessibilitySupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["accessibility_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
