workspace "btrfs_rescue"
    configurations { "debug", "release" }

    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    objdir "build/%{prj.name}/%{cfg.buildcfg}/obj"

    project "btrfs_tree_dumper"
        kind "ConsoleApp"
        targetname "btrfs_dump_tree"

        files { "src/**.c" }
        includedirs { "include" }

        filter { "system:linux", "action:gmake" }
            buildoptions { "-pedantic", "-Werror" }