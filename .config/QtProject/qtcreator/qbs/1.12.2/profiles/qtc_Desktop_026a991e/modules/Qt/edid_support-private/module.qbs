import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "EdidSupport"
    Depends { name: "Qt"; submodules: ["core-private"]}

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
    libNameForLinkerDebug: "Qt5EdidSupport"
    libNameForLinkerRelease: "Qt5EdidSupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_EDID_SUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtEdidSupport", "/usr/include/aarch64-linux-gnu/qt5/QtEdidSupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtEdidSupport/5.11.3/QtEdidSupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["edid_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
