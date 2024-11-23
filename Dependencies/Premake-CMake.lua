currentConfiguration = _OPTIONS["config"] or "Debug"
shouldBuild = _OPTIONS["buildcmake"] or "false"

function BuildCMakeProject(cmakeDir, buildDir, installDir, libName)
    -- Ensure directories exist
    os.mkdir(buildDir)
    os.mkdir(installDir)

    -- Generate CMake files
    local generateCmd = string.format("cmake -S %s/ -B %s", cmakeDir, buildDir)
    print("Running: " .. generateCmd)
    if os.execute(generateCmd) ~= 0 then
        --error("CMake generation failed!")
    end

    -- Build project
    local buildCmd = string.format("cmake --build %s --config %s", buildDir, currentConfiguration)
    print("Running: " .. buildCmd)
    if os.execute(buildCmd) ~= 0 then
        --error("CMake build failed!")
    end

    local prefix = ""
    local suffix = ""

    if os.target() == "windows" then
        prefix = "";
        suffix = ".lib";
    elseif os.target() == "linux" or os.target() == "macosx" then
        prefix = "lib";
        suffix = ".a";
    else
        error("Unsupported OS for static library naming conventions")
    end

    -- Find the output library
    fileName = string.format("%s%s%s", prefix, libName, suffix)

    local outputLib = path.join(buildDir, "src/Release", fileName) -- Adjust as needed
    local targetLib = path.join(installDir, "Release", fileName) -- Adjust as needed

    -- Move library to install directory
    print("Moving " .. outputLib .. " library to " .. targetLib)
    os.copyfile(outputLib, targetLib)

    local outputLib = path.join(buildDir, "src/Debug", fileName) -- Adjust as needed
    local targetLib = path.join(installDir, "Debug", fileName) -- Adjust as needed

    -- Move library to install directory
    print("Moving " .. outputLib .. " library to " .. targetLib)
    os.copyfile(outputLib, targetLib)
end

if shouldBuild == "true" then
    BuildCMakeProject("glslcc", "glslcc_bin", "glslcc_lib", "glslcc")
    os.exit(0)
elseif shouldBuild == "false" then
end