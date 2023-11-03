import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "KmsSupport"
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
    libNameForLinkerDebug: "Qt5KmsSupport"
    libNameForLinkerRelease: "Qt5KmsSupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_KMS_SUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtKmsSupport", "/usr/include/aarch64-linux-gnu/qt5/QtKmsSupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtKmsSupport/5.11.3/QtKmsSupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["kms_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
