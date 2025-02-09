currentConfiguration = _OPTIONS["config"] or "Debug"
shouldBuild = _OPTIONS["buildcmake"] or "false"

function BuildCMakeStaticProject(cmakeDir, buildDir, installDir, libName)
    -- Ensure directories exist
    os.mkdir(buildDir)
    os.mkdir(installDir)

    -- Set the architecture for macOS if on ARM (Apple Silicon)
    local architectureFlag = ""
    if os.target() == "macosx" then
        architectureFlag = "-DCMAKE_OSX_ARCHITECTURES=arm64" -- Force ARM architecture -- TODO: would this work? architectureFlag = "-DCMAKE_OSX_ARCHITECTURES=x86_64;arm64"  -- Build Universal Binary
    end

    -- Generate CMake files
    local generateCmd = string.format("cmake -S %s/ -B %s %s", cmakeDir, buildDir, architectureFlag)
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

    if os.target() == "windows" then
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
    else
        local outputLib = path.join(buildDir, "src", fileName) -- Adjust as needed

        -- DEBUG
        local targetLib = path.join(installDir, "Debug", fileName) -- Adjust as needed

        -- Move library to install directory
        print("Moving " .. outputLib .. " library to " .. targetLib)
        os.copyfile(outputLib, targetLib)

        -- RELEASE
        local targetLib = path.join(installDir, "Release", fileName) -- Adjust as needed
        
        -- Move library to install directory
        print("Moving " .. outputLib .. " library to " .. targetLib)
        os.copyfile(outputLib, targetLib)
    end
end
function BuildCMakeSharedProject(cmakeDir, buildDir, installDir, libName)
    -- Ensure directories exist
    os.mkdir(buildDir)
    os.mkdir(installDir)

    -- Set the architecture for macOS if on ARM (Apple Silicon)
    local architectureFlag = ""
    if os.target() == "macosx" then
        architectureFlag = "-DCMAKE_OSX_ARCHITECTURES=arm64" -- Force ARM architecture
    end

    -- Generate CMake files
    local generateCmd = string.format("cmake -S %s/ -B %s %s", cmakeDir, buildDir, architectureFlag)
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
        suffix = ".dll";
    elseif os.target() == "linux" then
        prefix = "lib";
        suffix = ".so";
    elseif os.target() == "macosx" then
        prefix = "lib";
        suffix = ".dylib";
    else
        error("Unsupported OS for static library naming conventions")
    end

    -- Find the output library
    fileName = string.format("%s%s%s", prefix, libName, suffix)

    if os.target() == "windows" then
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
    else
        local outputLib = path.join(buildDir, "src", fileName) -- Adjust as needed

        -- DEBUG
        local targetLib = path.join(installDir, "Debug", fileName) -- Adjust as needed

        -- Move library to install directory
        print("Moving " .. outputLib .. " library to " .. targetLib)
        os.copyfile(outputLib, targetLib)

        -- RELEASE
        local targetLib = path.join(installDir, "Release", fileName) -- Adjust as needed
        
        -- Move library to install directory
        print("Moving " .. outputLib .. " library to " .. targetLib)
        os.copyfile(outputLib, targetLib)
    end
end

if shouldBuild == "true" then
    BuildCMakeSharedProject("glslcc", "bin/glslcc", "glslcc_dll", "glslcc")
    os.exit(0)
elseif shouldBuild == "false" then
end