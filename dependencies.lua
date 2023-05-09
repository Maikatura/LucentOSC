--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

--Includes
IncludeDirs = {}
IncludeDirs["DPP"] = "%{wks.location}/External/dpp/include/dpp-10.0"
IncludeDirs["SFML"] = "%{wks.location}/External/SFML/include"
IncludeDirs["ImGui"] = "%{wks.location}/External/ImGui/src"


-- Eternal Includes

--Libaries
LibDirs = {}
LibDirs["UNKNOWN"] = "%{wks.location}/"


--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

Libs = {}
Libs["SFML"] = "%{IncludeDirs.SFML}/../lib"
Libs["DPP"] = "%{IncludeDirs.DPP}/../.."