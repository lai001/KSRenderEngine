set_xmakever("2.6.3")
includes("../../Foundation/Foundation")
add_requires("glm")
add_requires("glad")
add_requires("glfw")

local ShaderConductorBaseDir = "../../Vendor/ShaderConductor"

rule("KSRenderEngine.deps")
    on_load(function (target)
        local oldir = os.cd(target:scriptdir())
        import("devel.git")
        if os.exists("../../Foundation") == false then
            git.clone("https://github.com/lai001/Foundation.git", {branch = "main", outputdir = "../../Foundation"})
        end
        local ShaderConductorDir = path.join(target:scriptdir(), ShaderConductorBaseDir)
        local ShaderConductorBuildDir = path.join(ShaderConductorDir, "Build")
        if os.exists(ShaderConductorDir) == false then
            git.clone("https://github.com/gongminmin/ShaderConductor.git", {branch = "develop", outputdir = ShaderConductorDir})
        end
        if os.exists(ShaderConductorBuildDir) == false then
            os.mkdir(ShaderConductorBuildDir)
            local oldir1 = os.cd(ShaderConductorBuildDir)
            os.exec("cmake -G \"Visual Studio 15\" -T host=x64 -A x64 ../")
            os.exec("cmake --build . --config Release")
            os.cd(oldir1)
        end
        target:add("headerfiles", path.join(ShaderConductorDir, "Include/ShaderConductor/ShaderConductor.hpp"))
        os.cd(oldir)
    end)
    after_build(function (target)
        local ShaderConductorDir = path.join(target:scriptdir(), ShaderConductorBaseDir)
        local ShaderConductorTargetDir = path.join(ShaderConductorDir, "Build/Bin/Release")
        os.cp(path.join(ShaderConductorTargetDir, "/*.dll"), target:targetdir())
    end)

target("KSRenderEngine")
    set_kind("$(kind)")
    add_rules("mode.debug", "mode.release", "KSRenderEngine.deps")
    set_languages("c++17")
    if is_plat("windows") then
        add_links("d3d11")
        add_links("d3dcompiler")
        add_files("src/D3D11/*.cpp")
        add_headerfiles("include/KSRenderEngine/D3D11/*.hpp")
    end
    add_files("src/OpenGL/*.cpp")
    add_files("src/Common/*.cpp")
    add_files("src/Interface/*.cpp")
    add_files("src/*.cpp")

    add_includedirs("include/KSRenderEngine")
    add_includedirs("include", { interface = true })
    
    add_includedirs(path.join(ShaderConductorBaseDir, "Include"), { public = true })

    add_headerfiles("include/KSRenderEngine/*.hpp")
    add_headerfiles("include/KSRenderEngine/OpenGL/*.hpp")
    add_headerfiles("include/KSRenderEngine/Interface/*.hpp")
    add_headerfiles("include/KSRenderEngine/Common/*.hpp")

    add_linkdirs(path.join(ShaderConductorBaseDir, "Build/Lib/Release"), { public = true })

    add_links("ShaderConductor", { public = true })

    add_deps("Foundation")

    add_packages("glad")
    add_packages("glfw")
    add_packages("glm", {public = true})

target("Foundation")