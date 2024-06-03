/*********************************************************************
*
* IITB CONFIDENTIAL
* __________________
*
* All Rights Reserved © 2018-19 IITB, India
* All Rights Reserved © 2019-20 IITB, India
*
* NOTICE: All information contained in this document/file is, and remains the 
* property of Indian Institute of Technology, Bombay (IITB).The intellectual
* and technical concepts contained herein may be proprietary to IITB and are
* protected by trade secret or copyright laws. Unauthorized use, dissemination, 
* reproduction, duplication, reverse engineering, any form of redistribution, 
* or use in part or in whole, by any means is strictly forbidden unless prior, 
* express, written and signed consent is obtained from IITB.
*
* Institute   : Indian Institute of Technology Bombay (IITB)
***********************************************************************/


## Crypto5GT Library
Library of crypto functions designed only for 5G Test Bed

### Synopsis
Crypto5GT implements basic set of cryptographic algorithms to ensure Authentication,   
Encryption and Integrity of plain text messages and non-access stratus (NAS) messages.

### Requirements
    System Requirement:   
        - Linux Kernel 2.6 or higher
        - g++ 3.3 or later {c++11}
    
    Dependency Package:   
        - libssl-dev (for lcrypto and lssl)
        - cmake 

### How to Build standalone library?

  Run the following commands as root inside directory "Utility_library/crypto5GT":-

    -'cmake .' //run this command. CMakeList.txt is available in this folder
    -make //Lib code will be build by linking all the dependency package 
      & binary will be created @ "build/bin" 

### How to Link?
Add below entries in your module's CMakeLists.txt :-
   
    -'include_directories' adds path to (Utility_library) or (Utility_library/crypto5GT)
    -target_link_libraries to link with 'libcrypto5GT'  
  NOTE: You can choose to link either to library with its absolute path or 
  build it inside your module using:-   
  
    add_subdirectory(<your_path>/Utility_library/crypto5GT bin_crypto5GT)


### Overview
libcrypto5GT includes Key Functions for:-

   - AES-CTR, AES-CBC, AES-CMAC (uses openssl)
   - Rand Generation
   - Snow 3g ciphering and integrity
   - Key Derivation Function i.e.KDF (uses openssl)

NAS Functions for ciphering and integrity are wrappers which calls above key functions.

### NOTES











