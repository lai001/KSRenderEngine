set_xmakever("2.6.3")
includes("../../Foundation/Foundation")
includes("../KSRenderEngine")
add_requires("spdlog")
add_requires("glm")
add_requires("glad")
add_requires("glfw")
add_requires("stb")

rule("Example.deps")
    on_load(function (target)
        local oldir = os.cd(target:scriptdir())
        import("devel.git")
        if os.exists("../../Foundation") == false then
            git.clone("https://github.com/lai001/Foundation.git", {branch = "main", outputdir = "../../Foundation"})
        end
        if os.exists("./Vendor/imgui") == false then
            os.mkdir("./Vendor/imgui")
            local oldir = os.cd("Vendor")
            git.clone("https://github.com/ocornut/imgui.git", {branch = "master", outputdir = "imgui"})
            git.checkout("v1.87", {repodir = "imgui"})
            os.cd(oldir)
        end
        os.cd(oldir)
    end)

target("Example")
    set_kind("binary")
    add_files("*.cpp")
    add_headerfiles("*.hpp")
    add_rules("mode.debug", "mode.release", "Example.deps")
    set_languages("c++17")

    add_deps("Foundation")
    add_deps("KSRenderEngine")
    add_deps("ImGui")

    add_packages("spdlog")
    add_packages("glad")
    add_packages("glfw")
    add_packages("glm")
    add_packages("stb")

target("ImGui")
    add_rules("mode.debug", "mode.release", "Example.deps")
    set_kind("static")
    set_languages("c++17")
    add_includedirs("Vendor/imgui", { public = true })
    add_includedirs("Vendor", { interface = true })
    add_files("Vendor/imgui/*.cpp")
    add_files("Vendor/imgui/backends/imgui_impl_glfw.cpp")
    add_files("Vendor/imgui/backends/imgui_impl_opengl3.cpp")
    add_headerfiles("Vendor/imgui/*.h")
    add_headerfiles("Vendor/imgui/backends/imgui_impl_glfw.h")
    add_headerfiles("Vendor/imgui/backends/imgui_impl_opengl3.h")
    add_packages("glfw")