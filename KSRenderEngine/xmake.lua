set_xmakever("2.6.3")
includes("../../Foundation/Foundation")
add_requires("glm")
add_requires("glad")
add_requires("glfw")

rule("KSRenderEngine.deps")
    on_load(function (target)
        local oldir = os.cd(target:scriptdir())
        import("devel.git")
        if os.exists("../../Foundation") == false then
            git.clone("https://github.com/lai001/Foundation.git", {branch = "main", outputdir = "../../Foundation"})
        end
        os.cd(oldir)
    end)

target("KSRenderEngine")
    set_kind("$(kind)")
    add_rules("mode.debug", "mode.release", "KSRenderEngine.deps")
    set_languages("c++17")

    add_files("src/OpenGL/*.cpp")
    add_files("src/Common/*.cpp")
    add_files("src/Interface/*.cpp")
    add_files("src/*.cpp")

    add_includedirs("include/KSRenderEngine")
    add_includedirs("include", { interface = true })

    add_headerfiles("include/KSRenderEngine/*.hpp")
    add_headerfiles("include/KSRenderEngine/OpenGL/*.hpp")
    add_headerfiles("include/KSRenderEngine/Interface/*.hpp")
    add_headerfiles("include/KSRenderEngine/Common/*.hpp")

    add_deps("Foundation")

    add_packages("glad")
    add_packages("glfw")
    add_packages("glm", {public = true})