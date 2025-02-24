#include <iostream>
#include "Huffman.h"

using namespace std;

int main() {
    Huffman *compressor = new Huffman();
    int choice = 0;
    string in_file, out_file;

    cout << "Would you like to:" << endl
         << "1) Compress a file" << endl
         << "2) Decompress a file" << endl;

    cin >> choice;

    switch(choice){

        case 1:
            cout << "Enter the path of the file to be compressed: ";
            cin >> in_file;
            in_file = "text_files/" + in_file + ".txt";
            cout << "Enter the path where you'd like the decompress file to be saved: ";
            cin >> out_file;
            out_file = out_file + ".huf.txt";
            compressor->compress(in_file, out_file);
            break;

        case 2:
            cout << "Enter the path of the file to be decompressed: ";
            cin >> in_file;
            in_file = in_file + ".huf.txt";
            cout << "Enter the path where you'd like the uncompressed file to be saved: ";
            cin >> out_file;
            out_file = out_file+".ext.txt";
            compressor->decompress(in_file, out_file);
            break;

        default:
            cout << "That is not a valid choice. " << endl;
            break;
    }

    return 0;
}
