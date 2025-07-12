
❓ What Is It?

A C program that extracts EXIF metadata (like camera make, model, and GPS coordinates) directly from JPEG files — no third-party or platform-specific libraries required.
Uses only standard C library functions (fopen, printf, etc.) and processes files as raw byte buffers.

Functionality
- Parses JPEG files for embedded EXIF metadata
- Locates EXIF segment and TIFF header

Extracts: 
- Camera make and model
- Latitude and Longitude (if available)

All processing is done using manual offset calculations and byte-level logic.


⚠️ 📝 Notes

✅ Tested with photos taken on Android devices

❗ Not all JPEGs contain EXIF data — especially if edited, downloaded from certain platforms (ex. Instagram), or stripped by privacy settings

✉️ Photos sent via Gmail (to self) retain EXIF data — tested.

Works entirely with raw byte buffers and offsets
Developed on Ubuntu/Debian

🛠️ Built With
C (ISO C99)
GCC
Ubuntu Linux
Debian

📂 Usage
- make 
- ./meta_ext file_name.jpg

🔍 Why?
This project was built to better understand:
JPEG and EXIF binary structure
Endianness and byte-wise data parsing
Working with pointers and raw memory access in C

