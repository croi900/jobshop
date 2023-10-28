dofile "use_wxwidgets.lua"

workspace "Internship"
   configurations { "Debug", "Release" }

project "Internship"
   kind "WindowedApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "**.h", "**.c", "**.hpp", "**.cpp" }
   wx_config {Unicode="yes", Version="3.2", Libs="core,aui,gl"}
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"