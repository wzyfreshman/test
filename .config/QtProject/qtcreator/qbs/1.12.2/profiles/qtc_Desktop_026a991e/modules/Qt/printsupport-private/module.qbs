import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PrintSupport"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private", "widgets-private", "printsupport"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: false
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
    libNameForLinkerDebug: ""
    libNameForLinkerRelease: ""
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: []
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5/QtPrintSupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtPrintSupport/5.11.3/QtPrintSupport"]
    cpp.libraryPaths: []
    
}
