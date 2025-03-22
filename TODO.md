# TODO tasks

+ Purpose
    + TeMT was created before as custom-viewer project, now it needs to be moved to another repo with completely rebuilded structure
        + Include testing
        + Include CI/CD processes
        + Incude better versioning and better structure of commits

+ Tasks
    + ~~Initial Installation and Libs~~
        + ~~README, TODO, CMakeLists.txt~~
        + ~~FTXUI~~
        + ~~libarchive~~
        + ~~cereal (?)~~
            + Maybe rapidjson+rapidxml
        + ~~gtest~~
        + ~~logging feature?~~
    
    + Move old custom-viewer to TeMT
        + Redefine structure of namespaces and classes
        + Move old features to new namepsace
            + FileManipulations
            + ArchiveManipulations
            + DiskInfo
            + Json/XML Serialization
                + Rewrite Boost::PropertyTree to Cereal
            + Mine application class
        + Divide application class to subclasses and better structure
        + Include tests for each feature
        + Write better CMakeLists with proper library structure and package generation
        + Include logging for doubtful feats

    + Points of processing UI
        + Add library for working with YML
            + Settings in YML
        + Define paths for settings for temt
        + Smiles


