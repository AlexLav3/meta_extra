⚠️ This project is in development! ⚠️ 



❓ What Is It?

A C program that extracts EXIF metadata (like camera make, model, and GPS coordinates) directly from JPEG files — no third-party or platform-specific libraries required.
Uses only standard C library functions (fopen, printf, etc.) and processes files as raw byte buffers.

Functionality
- Parses JPEG files for embedded EXIF metadata
- Locates EXIF segment and TIFF header

Extracts: 
 Camera make and model -> bugs, but works mostly.
- Latitude and Longitude (if available)

All processing is done using manual offset calculations and byte-level logic.


⚠️ 📝 Notes


✅ Tested with photos taken on Android devices


❗ Not all JPEGs contain EXIF data — especially if edited, downloaded from certain platforms (ex. Instagram), or stripped by privacy settings


✉️ Photos sent via Gmail (to self) retain EXIF data — tested.

Works entirely with raw byte buffers and offsets
Being developed on Ubuntu

🛠️ Built With
C (ISO C99)
GCC
Ubuntu Linux

📂 Usage
- make 
- ./meta_ext file_name.jpg

🔍 Why?
This project was built to better understand:
JPEG and EXIF binary structure
Endianness and byte-wise data parsing
Working with pointers and raw memory access in C

💡 About Me

I'm a student at 42Prague exploring low-level programming and how data is structured at the binary level. I built this tool to try to understand EXIF format internals, and to practice C skills like file I/O, memory access, and working without libraries.
