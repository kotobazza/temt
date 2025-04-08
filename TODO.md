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
        + ~~Redefine structure of namespaces and classes~~
            + Now i'll use namespace temt and subnamespaces for each feature of the TeMT
            + Static classes will be rebuilded into functioanl fabrics (it's the good practice from creator of the FTXUI)

        + Move old features to new namepsace structure
            + FileManipulations
                + Partial
                    + Open a file or directory
                + Unimplemented
                    + Create directories/files
                    + Create archive
            + ArchiveManipulations
            + ~~TextWorker (now TextWriter)~~
            + DiskInfo
            + Json/XML Serialization
                + Rewrite Boost::PropertyTree to Cereal
            + ~Mine application class~

        + ~~Divide application class to subclasses and better structure~~
            + ~~Created class FileBrowser that lets be used to process filesystem operations~~
                + ~~TODO: let MainApp have entires_ in them instead of FileBrowser. FileBrowser should have only a link to that object~~
            + ~~Create Upper Panel (should let FileBrowser disappear -> need an toggler for disappearing)~~
            + ~~Create Main Panel that will let work any other functionalities~~

        + Include tests for each feature
        + Write better CMakeLists with proper library structure and package generation
        + ~~Include logging for doubtful feats~~

        + Emojis
            + Needs more directive check of file types now


    + Next steps:
        + Create more modules for temt:
            + Caesar Cipher
            + IP-worker
            + bin-hex-oct calculator
            + Calculator
                + Maybe with using ftxui::Canvas
            + Chrome::Dino
            + Archive viewer and creation
            + Show statistics of system and application itself
        + Define better structure for modules and open API for integrating the modules
        + Add library for working with YML
            + Settings in YML
            + Define paths for settings for temt



