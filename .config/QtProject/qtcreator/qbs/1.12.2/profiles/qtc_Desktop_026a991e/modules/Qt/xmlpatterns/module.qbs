import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "XmlPatterns"
    Depends { name: "Qt"; submodules: ["core", "network"]}

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
    libNameForLinkerDebug: "Qt5XmlPatterns"
    libNameForLinkerRelease: "Qt5XmlPatterns"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5XmlPatterns.so.5.11.3"
    cpp.defines: ["QT_XMLPATTERNS_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtXmlPatterns"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["xmlpatterns"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
