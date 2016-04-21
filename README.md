Originally posted to the second [72 hour game development competition](https://github.com/featherless/72hourgdc)
from January 02, 2004 to January 05, 2004.

[Download the submitted entry](https://github.com/72hourgdc-2004-january/Graupmann/archive/submission.zip).

    //*******************************************************************************
    //                                          
    //      - 72 Hour Game Development Competition -
    //                                                                       
    //      $Author1:   Tim Graupmann   tgraupmann@yahoo.com
    //      $Author2:   Linae Graupmann lgraupmann@yahoo.com
    //                                          
    //      $Program:   72H_GameDev.exe
    //                      
    //      $Description:   Competition Entry
    //                                          
    //      $Date:      2004-01-04
    //                                          
    //*******************************************************************************

    Disclaimer:
            The following information is provided "as is", with ABSOLUTELY NO
            warranties. The provided source code and/or data files are tested
            and are not guaranteed to work on all machines.

            I certify that the source code contained within this project was
            my original work and/or was made publically available at the time
            it was adapted for use with this demo.

            Feel free to use any of this code with only the following condition:

            1) The code is not supported in any way. I am not responsible for any
            harm caused to your machine (although I have made every attemp to check
            for any possibly damaging stuff).

    Legal Issues: 
            FinalRedemption ALONG WITH OURSELVES SHALL NOT BE LIABLE TO YOU OR
            ANY THIRD PARTY FOR ANY INFRINGEMENT OF COPYRIGHT, TRADEMARK, TRADE
            NAME OR TRADE SECRETS RIGHTS OF AN UNAFFILIATED THIRD PARTY. 

    Known Issues:
            We only tested this game demo on Win2k with NVidia & ATI Video Cards.

    Source Code:
            Most of the source code is derivative work gained from studying
            the information found at http://www.gametutorials.com, therefore
            it is addionally subject to the terms of use found at the this
            address. http://gametutorials.com/TermsOfUse.htm

    Misc:
            Feel free to email us if you find the demo entertaining or useful.
            None of the source code is proprietary therefore I grant you the
            proper permission to use it without needing to contact me, given
            that you follow the terms of use set by GameTutorials and this
            contest.

    Members:
            Tim Graupmann
                - Game Development
                - Music
                - Textures
            Linae Graupmann
                - Artwork
                - Design & Concept
                - QA & Testing

    Original Works:
            All music and graphical media was composed by the members listed above.
        
    Files:
        Graupmann\
            Source\

                CreativeContest.vcproj  (.NET Project File)
                Main.cpp   (Main driver for the demo)
                fonts.h (Font function prototypes)
                fonts.cpp (Font implementation)
                midi.h (MIDI prototypes)
                midi.cpp (MIDI implementation)
                c3dobjects.h (Custom 3d character prototypes)
                c3dobjects.cpp (Custom 3d character implementations)

                resource.h (.NET generated resouce file for the custom cursor)
                arrow.cur (Normal mouse cursor)

                72H_GameDev.vcproj (.NET Project File)

                72H_GameDev.exe (Win32 Application) -- Compiled with .NET and Win2k

                data\ {various textures and music files}

    Libraries:
            opengl32.lib, glu32.lib, glaux.lib

    Controls:
            ESCAPE Key - This quits the program at any time (IMPORTANT)
            UP ARROW, W - Moves the orange forward
            DOWN ARROW, S - Moves the orange backward
            LEFT ARROW,AS - Turns the orange left
            RIGHT ARROW,AS - Turns the orange right
            1 Key - Uses Lady Orange
            2 Key - Uses Boy Orange
                    SPACE Key - Jumps and activates bubble shield

    Test Machines:
            I tested this demo on a few lower end machines.
            - AMD Duron 800 MHz, Win2k, NVIDIA GF440.
            - P4 DUAL 1000 MHz, Win2k, ATI RADEON

    Tim Graupmann
    Game Programmer
    tgraupmann@yahoo.com
    tgraupmann.uwvcd.com

    Linae Graupmann
    Game Designer
    lgraupmann@yahoo.com