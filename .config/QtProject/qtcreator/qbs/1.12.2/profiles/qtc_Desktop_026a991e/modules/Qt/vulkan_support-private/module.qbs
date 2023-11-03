import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "VulkanSupport"
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
    libNameForLinkerDebug: "Qt5VulkanSupport"
    libNameForLinkerRelease: "Qt5VulkanSupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_VULKAN_SUPPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtVulkanSupport", "/usr/include/aarch64-linux-gnu/qt5/QtVulkanSupport/5.11.3", "/usr/include/aarch64-linux-gnu/qt5/QtVulkanSupport/5.11.3/QtVulkanSupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
Group {
        files: [Qt["vulkan_support-private"].libFilePath]
        filesAreTargets: true
        fileTags: ["staticlibrary"]
    }
}
