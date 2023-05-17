--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

--Includes
IncludeDirs = {}
IncludeDirs["DPP"] = "%{wks.location}/External/dpp/include/dpp-10.0"
IncludeDirs["ImGui"] = "%{wks.location}/External/ImGui/src"


-- Eternal Includes

--Libaries
LibDirs = {}
LibDirs["UNKNOWN"] = "%{wks.location}/"


--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

Libs = {}
Libs["DPP"] = "%{IncludeDirs.DPP}/../.."