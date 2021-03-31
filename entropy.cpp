#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <iomanip>

using namespace std;

#define BLKSIZE 1024

float block_entropy(char* blk, size_t len);
ifstream& file_entropy(ifstream& file);

/* PROGRAM TO CALCULATE THE ENTROPY OF A FILE */
int main (int argc, char* argv[]){
    char buffer[BLKSIZE];

    // If the program is called with the wrong number of parameters
    if (argc != 2){
        cout << "Usage: ./entropy <file_name>" << endl;
        return -1;
    }

    // open the file in a binary way
    ifstream myFile (argv[1], ios::in | ios::out | ios::binary);

    // In case that the file could not be opened correctly
    if (!myFile){
        cout << "Se produjo un error al abrir el archivo " << argv[1] << endl;
        return -1;
    }

    // calculate the entropy of the block
    file_entropy(myFile);

    // Close the file
    myFile.close();

    return 0;
}

// Calculate the entropy of a file
ifstream& file_entropy(ifstream& file){
    char buffer[BLKSIZE];
    int block_number = 0;
    float blck_entropy = 0;
    int low_entropy_block = 0;
    int high_entropy_block = 0;

    cout << "\tblock# \tentropy" << endl;

    while(!file.eof()){
        file.read(buffer, BLKSIZE);
        blck_entropy =  file ? block_entropy(buffer, BLKSIZE) : block_entropy(buffer, file.gcount()) ;
        
        if (blck_entropy < 2){
            low_entropy_block++;
        }
        if (blck_entropy > 7){
            high_entropy_block++;
        }

        cout << "\t" << block_number << "\t" << setprecision(3) << blck_entropy << endl;

        block_number++;
    }

    cout << "Low entropy blocks: " << low_entropy_block << endl;
    cout << "High entropy blocks: " << high_entropy_block << endl;

    return file;
}

// Calculate the entropy of a single block
float block_entropy(char* block, size_t len){
    map<char, long> mapa;

    // Create a map for every byte in the file. If the 
    // byte does not exist, insert the value equal to one. 
    // Other case, increment it by one
    for(int i = 0; i < len; i++){
        if(mapa.find(block[i]) == mapa.end()){
            mapa[block[i]] = 1;
        } else {
            mapa[block[i]]++;
        }
    }

    float entropy = 0;
    float relative_frec = 0;
    map<char, long>::iterator itr;
    
    // Iterate over the map, to calculate the relative 
    // frecuency of the elements in the block
    for (itr = mapa.begin(); itr != mapa.end(); itr++){
        relative_frec = itr->second / static_cast<float>(len);
        relative_frec *= log2(relative_frec);
        entropy += relative_frec;
    }

    return entropy * (-1);
}