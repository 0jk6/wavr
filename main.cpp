#include <iostream>
#include <fstream>
#include <cstdint>

struct WaveHeader{
    char riff[5]; //actually, it's size is 4 but, at the end we should add a `/0` character, to print it correctly with std::cout
    uint32_t size;
    char wave[5]; //same as riff, only 4 bytes, but last one is for a null character

    char fmt_chunk[5];
    uint32_t fmt_length;
    uint16_t fmt_type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    
    char data_chunk_header[5]; //only 4 bytes, last byte is for a null character
    uint32_t data_size;
};

int main(int argc, const char** argv){
    //read the file
    if(argc < 2){
        std::cerr << "Error: No file specified" << "\n";
        std::cout << "Usage: ./wave <wave file name>" << "\n";
        return 1;
    }

    std::ifstream fd{argv[1], std::ios::binary};

    if(!fd){
        std::cerr << "ERROR: " << "Could not open file " << "'" << argv[1] << "'" << "\n";
        return 1;
    }

    //create a wave header variable
    WaveHeader header;

    //read first 44 bytes into the header, ifstream::read takes two arguments, first one is char_type* and the other size
    //and that's the reason why we should cast the variables in the header
    fd.read((char*)&header.riff, 4);
    fd.read((char*)&header.size, 4);
    fd.read((char*)&header.wave, 4);
    fd.read((char*)&header.fmt_chunk, 4);
    fd.read((char*)&header.fmt_length, 4);
    fd.read((char*)&header.fmt_type, 2);
    fd.read((char*)&header.channels, 2);
    fd.read((char*)&header.sample_rate, 4);
    fd.read((char*)&header.byte_rate, 4);
    fd.read((char*)&header.block_align, 2);
    fd.read((char*)&header.bits_per_sample, 2);
    fd.read((char*)&header.data_chunk_header, 4);
    fd.read((char*)&header.data_size, 4);

    //add null characters to the end of the ASCII variables in header
    header.riff[4] = '\0';
    header.wave[4] = '\0';
    header.fmt_chunk[4] = '\0';
    header.data_chunk_header[4] = '\0';


    //print the header
    std::cout << "\n" << "---------- START OF WAVE HEADER ----------" << "\n";
    std::cout << "RIFF: " << header.riff << "\n";
    std::cout << "SIZE: " << header.size << "\n"; //this size will be total size - (header.riff + header.size) which is 8 bytes
    std::cout << "WAVE: " << header.wave << "\n";
    std::cout << "FORMAT CHUNK: " << header.fmt_chunk << "\n";
    std::cout << "FORMAT LENGTH: " << header.fmt_length << "\n";
    std::cout << "FORMAT TYPE: " << header.fmt_type << "\n";
    std::cout << "NO CHANNELS: " << header.channels << "\n";
    std::cout << "SAMPLE RATE: " << header.sample_rate << "\n";
    std::cout << "BYTE RATE: " << header.byte_rate << "\n";
    std::cout << "BLOCK ALIGN: " << header.block_align << "\n";
    std::cout << "BITS PER SAMPLE: " << header.bits_per_sample << "\n";
    std::cout << "DATA CHUNK HEADER: " << header.data_chunk_header << "\n";
    std::cout << "DATA SIZE: " << header.data_size << "\n";
    std::cout << "----------  END OF WAVE HEADER  ----------" << "\n\n";

    //create a buffer to store the wave data
    char buffer[header.data_size];
    fd.read((char*)&buffer, sizeof(buffer));

    /*
        Now, we can read the data, the size of the data will be header.data_size
    */
    char userInp;
    std::cout << "Do you want to print the data? [Y/n] : ";
    std::cin >> userInp;

    if(userInp == 'Y' || userInp == 'y'){

        for(int i=0; i<header.data_size; i+=2){
            int c = (buffer[i+1] << 8) | buffer[i]; //two bytes are combined to form a 16 bit value
            double val = c/32768.0; //normalize them to be in the range of 0 to 1

            std::cout << val << ",";
        }
    }


    return 0;
}