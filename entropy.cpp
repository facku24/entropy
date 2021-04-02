#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <iomanip>
#include <stdio.h>

using namespace std;

#define BLKSIZE 1024

float block_entropy(char* blk, unsigned int blk_size);
void file_entropy(ifstream& file, unsigned int blk_size);
int parse_cmd_line(int argc, char** argv, char** file_name);

int main (int argc, char* argv[]){

    int blk_size = 0;
    char* file_name = NULL;

    blk_size = parse_cmd_line(argc, argv, &file_name);
    
    if (blk_size < 1){
        cout << "Usage: ./entropy [ -b <blocksize> ] <file_name>" << endl;
        return -1;
    }

    ifstream file(file_name, ios::binary);

    if (!file){
        cout << "Could not open the file  " << argv[1] << endl;
        return -1;
    }

    file_entropy(file, blk_size);

    file.close();

    return 0;
}

int parse_cmd_line(int argc, char** argv, char** file_name){
    int blk_size= -1;

    if (argc == 2){
        *file_name = argv[1];
        return BLKSIZE;
    } else if (argc == 4 && strcmp(argv[2], "-b")){
        *file_name = argv[3];
        sscanf(argv[2], "%d", &blk_size);
        return blk_size;
    }

    return blk_size;
}

// Calculate the entropy of a file
void file_entropy(ifstream& file, unsigned int blk_size){
    char buffer[blk_size];
    int blk_number = 0;
    float blk_entropy = 0;
    int low_entropy_blk = 0;
    int high_entropy_blk = 0;

    cout << "\tblock# \tentropy" << endl;

    while(!file.eof()){
        file.read(buffer, blk_size);
        blk_entropy =  file ? block_entropy(buffer, blk_size) : block_entropy(buffer, file.gcount());
        
        if (blk_entropy < 2){
            low_entropy_blk++;
        } else if (blk_entropy > 7){
            high_entropy_blk++;
        }

        cout << "\t" << blk_number << "\t" << setprecision(3) << blk_entropy << endl;

        blk_number++;
    }

    cout << "Low entropy blocks: " << low_entropy_blk << endl;
    cout << "High entropy blocks: " << high_entropy_blk << endl;

}

// Calculate the entropy of a single block
float block_entropy(char* block, unsigned int blk_size){
    map<char, long> bytes_map;

    if (blk_size == 0 || block == NULL){
        return 0;
    }

    /* Create a map for every byte in the file. If the 
       byte does not exist, insert the value equal to one. 
       Other case, increment it by one */
    for(int i = 0; i < blk_size; i++){
        if(bytes_map.find(block[i]) == bytes_map.end()){
            bytes_map[block[i]] = 1;
        } else {
            bytes_map[block[i]]++;
        }
    }

    float entropy = 0;
    float relative_frec = 0;
    map<char, long>::iterator itr;
    
    /* Iterate over the map, to calculate the relative 
       frecuency of the elements in the block */
    for (itr = bytes_map.begin(); itr != bytes_map.end(); itr++){
        relative_frec = itr->second / static_cast<float>(blk_size);
        relative_frec *= log2(relative_frec);
        entropy += relative_frec;
    }

    return entropy != 0 ? entropy * (-1) : entropy;
}