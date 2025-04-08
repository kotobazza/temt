# TODO tasks

+ Purpose
    + TeMT was created before as custom-viewer project, now it needs to be moved to another repo with completely rebuilded structure
        + Include testing
        + Include CI/CD processes
        + Incude better versioning and better structure of commits

+ Tasks
    + Old feautres (custom-viewer)
        + File Manipulations
            + ~~List files and directories~~
            + Create dir/file
            + Open file as:
                + Json
                + Xml

                + ~Text~
        + Archive Manipulations
            + Create archive from directory
            + Archive
        + Text Worker
            + ~~Open text~~
            + ~~Edit text~~
            + ~~Save text~~
        + Disk Info
            + Show info about filesystems on device
        + Json/Xml serialization
            + Serialize/deserialize object
            + Check file structure in deserialization
    + New features
        + Better build
            + Used libraries
                + ~~FTXUI~~
                + ~~Libarchive~~
                + Cereal (?)
                    + > maybe rapidjson+rapidxml
                + ~~gtest~~
                + ~~spdlog~~ (?)
            + Packages
                + ~~Windows~~
                + ~~RPM~~
                + ~~DEB~~
            + Tests
                + ~~Broken tests for Libarchive~~
                + Tests for interface (how to do?)
                    + > FTXUI is able to represent interface as string
                + Tests for features themselves
        + Features
            + Selected File Entries buffer
            + ~~Emojis for files~~ 
                + > (needs more emojis)
            + True API for modules
            + Transform Disk Info to Disk Usage or even ncdu
            + Archive Manipulation
                + Create archive from files buffer
            + Caesar Cipher
            + IP-worker
            + bin-hex-oct calculator
            + Calculator
                + Maybe with using ftxui::Canvas for functions
            + Chrome::Dino
            + Show statistics of system and application itself
            + Better settings
                + Control on color themes
                + Control on emojis for each shown emoji
                + File in YML format
        + Refactor
            + Transform static classes to functions (and place functions in namespaces)
            + Divide main application into subclasses 
                + Use the Best Practices to implement ftxui::Component as parts of the application
                + Use class for application data
                    + All components may read this data (and by type& too), but can't call functions of this class. Except the class for main application
                    + Due to functional style of FTXUI - all state is saved in application class, and the interface just reacts on the state
                        + Needs better testing   




