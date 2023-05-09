--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

--Includes
IncludeDirs = {}
IncludeDirs["UNKNOWN"] = "%{wks.location}/"


-- Eternal Includes

--Libaries
LibDirs = {}
LibDirs["UNKNOWN"] = "%{wks.location}/"


--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

Libs = {}
Libs["UNKNOWN"] = "%{LibDirs.FBXSDK}/"